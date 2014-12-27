#include "Net.h"

#include "HostList.h"
#include <wx/wfstream.h>
#include <wx/app.h>
#include <osrng.h>

///////////////////////////////////////////////////////
// CCryptoProvider implementation
///////////////////////////////////////////////////////

void CCryptoProvider::GenerateAESValues()
{
  CryptoPP::AutoSeededRandomPool rng;

  rng.GenerateBlock((byte*)aesKey.GetAppendBuf(AES256_KEY_SIZE), AES256_KEY_SIZE);
  rng.GenerateBlock((byte*)aesIV.GetAppendBuf(AES256_IV_SIZE), AES256_IV_SIZE);
}

void CCryptoProvider::Encrypt(wxMemoryBuffer& dest, const wxMemoryBuffer& source)
{
  // Adding CRC sum before encrypting
  unsigned long crc = 0;
  ::CRCSum(source, &crc);
  wxMemoryBuffer plain;
  plain.AppendData(source.GetData(), source.GetDataLen());
  plain.AppendData(&crc, sizeof(crc));
  ::AESEncrypt(dest, plain, source.GetDataLen() + sizeof(crc), aesKey, aesIV);
}

bool CCryptoProvider::Decrypt(wxMemoryBuffer& dest, const wxMemoryBuffer& source)
{
  wxMemoryBuffer cipherData, signature;

  // Separating
  unsigned long packetLen, signatureLen;
  unsigned long sizeOfLen = sizeof(unsigned long);
  char* p = (char*)source.GetData();
  // Data size
  packetLen = *(unsigned long*)p;
  p += sizeOfLen;
  // Copy data
  cipherData.AppendData(p, packetLen);
  p += packetLen;
  // Signature size
  signatureLen = *(unsigned long*)p;
  p += sizeOfLen;
  // Copy signature
  signature.AppendData(p, signatureLen);

  // RSA verifying
  bool retVal = RSAVerify(cipherData, signature);
  if (!retVal)
    return false;

  // AES decrypting
  ::AESEncrypt(dest, cipherData, packetLen, aesKey, aesIV);

  // Separating CRC
  unsigned long crcTaken, crcCalced = 0, dataLen;
  dataLen = packetLen - sizeof(crcTaken);
  crcTaken = *(unsigned long*)((char*)dest.GetData() + dataLen);
  dest.SetDataLen(dataLen);

  // CRC verifying
  ::CRCSum(dest, &crcCalced);
  return crcCalced == crcTaken;
}

void CCryptoProvider::GenerateHandshake(wxMemoryBuffer& buffer)
{
  wxMemoryBuffer plain;
  // Key
  unsigned long size = AES256_KEY_SIZE;
  plain.AppendData(&size, sizeof(size));
  plain.AppendData(aesKey.GetData(), aesKey.GetDataLen());
  // iv
  size = AES256_IV_SIZE;
  plain.AppendData(&size, sizeof(size));
  plain.AppendData(aesIV.GetData(), aesIV.GetDataLen());
  // CRC
  ::CRCSum(plain, &size);
  plain.AppendData(&size, sizeof(size));
  // RSA
  RSAEncrypt(buffer, plain);
}

void CCryptoProvider::PutStringDigest(const wxString& string, wxMemoryBuffer& buffer)
{
  wxMemoryBuffer digest;
  ::SHADigest(string, digest);
  unsigned long len = digest.GetDataLen();
  buffer.AppendData(&len, sizeof(len));
  buffer.AppendData(digest.GetData(), len);
}

///////////////////////////////////////////////////////
// CNetThread implementation
///////////////////////////////////////////////////////

CNetThread::CNetThread(const wxString& host, const unsigned short port, wxEvtHandler* listener, wxSemaphore* semaphore) :
        wxThread(wxTHREAD_DETACHED), hostName(host), service(port), mainThreadListener(listener), doneSemaphore(semaphore)
{
  isPacketReady = false;
}

void CNetThread::SendPacket(const wxMemoryBuffer& content)
{
  wxCriticalSectionLocker lock(dataCS);
  sendingPacket.Clear();
  sendingPacket.AppendData(content.GetData(), content.GetDataLen());
  isPacketReady = true;
}

void CNetThread::ResendPacket()
{
  wxCriticalSectionLocker lock(dataCS);
  isPacketReady = true;
}

void CNetThread::ReceivePacket(wxMemoryBuffer& content)
{
  wxCriticalSectionLocker lock(dataCS);
  content.Clear();
  content.AppendData(receivingPacket.GetData(), receivingPacket.GetDataLen());
}

void CNetThread::NotifyError(const NetError error)
{
  NotifyMainThread(NET_EVENT_ERROR, error);
}

void CNetThread::NotifyMainThread(const NetEvent event, const NetError error = NET_ERROR_SUCCESS)
{
  NetMsg msg = {event, error};
  ClientEvent evt(this, msg);
  wxQueueEvent(mainThreadListener, evt.Clone());
}

wxThread::ExitCode CNetThread::Entry()
{
  socket = new wxSocketClient(wxSOCKET_WAITALL | wxSOCKET_BLOCK);
  if (!socket->IsOk())
  {
    // Socket creation error. Probably, sockets are not available
    NotifyError(NET_ERROR_SOCKETS_UNAVAILABLE);
    Cleanup();
    return 0;
  }
  wxIPV4address address;
  address.Hostname(hostName);
  address.Service(service);
  socket->SetTimeout(NET_CLIENT_TIMEOUT);
  if (!socket->Connect(address))
  {
    // Connection failed
    NotifyError(NET_ERROR_CONNECTING_TO_HOST);
    Cleanup();
    return 0;
  }
  else
    NotifyMainThread(NET_EVENT_CONNECTED);
  while (!TestDestroy())
  {
    this->Sleep(10);
    wxCriticalSectionLocker lock(dataCS);
    if (isPacketReady)
    {
      isPacketReady = false;
      unsigned long dataLen = sendingPacket.GetDataLen();
      // Sending data length
      socket->Write(&dataLen, sizeof(dataLen));
      if (socket->Error() || (socket->GetLastIOWriteSize() != sizeof(dataLen)))
      {
        // Sending failed
        NotifyError(NET_ERROR_SENDING_FAILED);
        continue;
      }
      // Sending data
      socket->Write(sendingPacket.GetData(), dataLen);
      if (socket->Error() || (socket->GetLastIOWriteSize() != dataLen))
      {
        // Sending failed
        NotifyError(NET_ERROR_SENDING_FAILED);
        continue;
      }
      dataLen = 0;
      // Receiving data length
      socket->Read(&dataLen, sizeof(dataLen));
      if (socket->Error() || (socket->GetLastIOReadSize() != sizeof(dataLen)))
      {
        // Receiving failed
        NotifyError(NET_ERROR_RECEIVING_FAILED);
        continue;
      }
      // Receiving data
      socket->Read(receivingPacket.GetAppendBuf(dataLen), dataLen);
      if (socket->Error() || (socket->GetLastIOReadSize() != dataLen))
      {
        // Receiving failed
        NotifyError(NET_ERROR_RECEIVING_FAILED);
        continue;
      }
      NotifyMainThread(NET_EVENT_TASK_COMPLETED);
    }
  }
  Cleanup();
  doneSemaphore->Post();
  NotifyMainThread(NET_EVENT_DISCONNECTED);
  return 0;
}

void CNetThread::Cleanup()
{
  socket->Close();
  socket->Destroy();
  socket = NULL;
}

///////////////////////////////////////////////////////
// CNet implementation
///////////////////////////////////////////////////////

void CNet::SendNotification(const NetError code)
{
  errorCode = code;
//  wxTheApp->AddPendingEvent(event);
}

bool CNet::ValidateKey()
{
  bool res = cryptoProvider.LoadPublicRSAKey(CurrentHost->GetPublicKeyPath());
  if (!res)
  {
    StopThread();
    SendNotification(NET_ERROR_INVALID_KEY);
  }
  return res;
}

void CNet::SavePublicKey()
{
  // Saving key received from the server
  wxMemoryBuffer key;
  thread->ReceivePacket(key);
  wxFileOutputStream output(CurrentHost->ConstructNewKeyPath());
  output.Write(key.GetData(), key.GetDataLen());
}

void CNet::OnClientEvent(ClientEvent& event)
{
  if (event.sender != thread || !IsThreadAlive())
    return;
  switch (event.GetMessage().event)
  {
    case NET_EVENT_CONNECTED:
    {
      task = ntNothing;
      if (!CurrentHost->IsInitialized())
        RequestPublicKey();   // If we don't have key yet
      else
      {
        if (ValidateKey())    // If we already have key
          DoHandshake();
      }
      break;
    }
    case NET_EVENT_TASK_COMPLETED:
    {
      if (task == ntGettingKey)
      {
        task = ntNothing;
        // After receiving key from server
        SavePublicKey();
        if (ValidateKey())
          DoHandshake();
      }
      else if (task == ntHandshaking)
      {
        task = ntNothing;
        // Check handshake result
        wxMemoryBuffer packet, data;
        thread->ReceivePacket(packet);
        if (cryptoProvider.Decrypt(data, packet))
        {
          wxString answer;
          answer.Alloc(data.GetDataLen());
          answer.Append((char*)data.GetData());
          handshaked = answer == "OK";
          handshaked ? SendNotification(NET_ERROR_SUCCESS) : SendNotification(NET_ERROR_HANDSHAKE_SERVER_FAILED);
        }
        else
        {
          StopThread();
          SendNotification(NET_ERROR_HANDSHAKE_CLIENT_FAILED);
        }
      }
      else if (task == ntTransmission)
      {
        task = ntNothing;
        wxMemoryBuffer packet, data;
        thread->ReceivePacket(packet);
        // Check transmission result
        if (cryptoProvider.Decrypt(data, packet))
        {

        }
        else
        {
          StopThread();
          SendNotification(NET_ERROR_VERIFYING_FAILED);
        }
      }
      break;
    }
    case NET_EVENT_DISCONNECTED:
    {
      CleanThread();
      break;
    }
    case NET_EVENT_ERROR:
    {
      if ((event.GetMessage().error == NET_ERROR_SENDING_FAILED) ||
          (event.GetMessage().error == NET_ERROR_RECEIVING_FAILED))
      {
        thread->ResendPacket();
      }
      else
      {
        CleanThread();
        SendNotification(event.GetMessage().error);
      }
      break;
    }
  }
}

CNet& CNet::Get()
{
  // CNet is a singleton
  static CNet net;
  return net;
}

CNet::CNet()
{
  errorCode = NET_ERROR_SUCCESS;
  task = ntNothing;
  handshaked = false;
  terminating = false;
  thread = NULL;
  doneSemaphore = NULL;
}

CNet::~CNet()
{
  if (IsThreadAlive())
  {
    StopThread();
    // Wait until thread sends message about it's deletion
    doneSemaphore->Wait();
  }
  delete doneSemaphore;
}

void CNet::StopThread()
{
  if (IsThreadAlive())
  {
    terminating = true;
    thread->Delete();
  }
}

void CNet::CleanThread()
{
  if (!IsThreadAlive())
  {
    if (doneSemaphore)
    {
      doneSemaphore->Wait();
      delete doneSemaphore;
      doneSemaphore = NULL;
    }
    thread = NULL;
    task = ntNothing;
    handshaked = false;
    terminating = false;
  }
}

bool CNet::InitializeConnect()
{
  if (!CurrentHost)
  {
    SendNotification(NET_ERROR_HOST_NOT_SELECTED);
    return false;
  }
  if (!IsThreadAlive())
  {
    CleanThread();
    task = ntConnecting;
    doneSemaphore = new wxSemaphore();
    thread = new CNetThread(CurrentHost->GetAddress(), CurrentHost->GetPort(), this, doneSemaphore);
    Bind(wxEVT_CLIENT, (wxObjectEventFunction)&CNet::OnClientEvent, this);
    if (thread->Run() != wxTHREAD_NO_ERROR)
    {
      // Something goes wrong and we can't run the thread
      SendNotification(NET_ERROR_CANT_RUN_THREAD);
      return false;
    }
    return true;
  }
  else
  {
    SendNotification(NET_ERROR_ALREADY_CONNECTED);
    return false;
  }
}

void CNet::RequestPublicKey()
{
  if (IsThreadAlive() && (task == ntNothing))
  {
    // Building key request packet
    wxMemoryBuffer packet;
    PutString("Public key, please", packet); // Do not localize!
    task = ntGettingKey;
    thread->SendPacket(packet);
  }
}

void CNet::DoHandshake()
{
  if (IsThreadAlive() && (task == ntNothing))
  {
    wxMemoryBuffer packet;
    PutString("Handshake", packet); // Do not localize!
    cryptoProvider.GenerateAESValues();
    cryptoProvider.GenerateHandshake(packet);
    task = ntHandshaking;
    thread->SendPacket(packet);
  }
}

void CNet::RequestRegistration(const wxString& login, const wxString& password)
{
  if (IsThreadAlive() && (task == ntNothing) && handshaked)
  {
    wxMemoryBuffer packet;
    PutString("Registration", packet); // Do not localize!
    PutString(login, packet);
    cryptoProvider.PutStringDigest(password, packet);
    task = ntTransmission;
    thread->SendPacket(packet);
  }
}

void CNet::RequestAuthorization(const wxString& login, const wxString& password)
{
  if (IsThreadAlive() && (task == ntNothing) && handshaked)
  {
    wxMemoryBuffer packet;
    PutString("Authorization", packet); // Do not localize!
    PutString(login, packet);
    cryptoProvider.PutStringDigest(password, packet);
    task = ntTransmission;
    thread->SendPacket(packet);
  }
}

void CNet::RequestSynchronization()
{

}
