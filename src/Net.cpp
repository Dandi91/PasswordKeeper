#include "Net.h"

#include "HostList.h"
#include <osrng.h>

void CCryptoProvider::GenerateAESKey()
{
  CryptoPP::AutoSeededRandomPool rng;

  rng.GenerateBlock((byte*)aesKey.GetAppendBuf(AES256_KEY_SIZE), AES256_KEY_SIZE);
  rng.GenerateBlock((byte*)aesIV.GetAppendBuf(AES256_IV_SIZE), AES256_IV_SIZE);
}

void CCryptoProvider::AESEncrypt(wxMemoryBuffer& dest, const wxMemoryBuffer& source, const size_t length)
{
  ::AESEncrypt(dest, source, length, aesKey, aesIV);
}

///////////////////////////////////////////////////////
// CNetThread implementation
///////////////////////////////////////////////////////

CNetThread::CNetThread(const wxString& host, wxEvtHandler* listener) : wxThread(wxTHREAD_DETACHED)
{
  hostName = host;
  mainThreadListener = listener;
  isPacketReady = false;
}

void CNetThread::SendPacket(const wxMemoryBuffer& content)
{
  wxCriticalSectionLocker lock(dataCS);
  packet.Clear();
  packet.AppendData(content.GetData(), content.GetDataLen());
  isPacketReady = true;
}

void CNetThread::NotifyError(const NetError error)
{
  NotifyMainThread(NET_EVENT_ERROR, error);
}

void CNetThread::NotifyMainThread(const NetEvent event, const NetError error)
{
  NetMsg msg = {event, error};
  ClientEvent evt(this, msg);
  mainThreadListener->AddPendingEvent(evt);
}

wxThread::ExitCode CNetThread::Entry()
{
  socket = new wxSocketClient(wxSOCKET_WAITALL | wxSOCKET_BLOCK);
  wxIPV4address address;
  address.Hostname(hostName);
  address.Service(NET_SERVER_PORT);
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
      unsigned long dataLen = packet.GetDataLen();
      // Sending data length
      socket->Write(&dataLen, sizeof(dataLen));
      if (socket->Error() || (socket->GetLastIOWriteSize() != sizeof(dataLen)))
      {
        // Sending failed
        NotifyError(NET_ERROR_SENDING_FAILED);
        continue;
      }
      // Sending data
      socket->Write(packet.GetData(), dataLen);
      if (socket->Error() || (socket->GetLastIOWriteSize() != dataLen))
      {
        // Sending failed
        NotifyError(NET_ERROR_SENDING_FAILED);
        continue;
      }
      packet.Clear();
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
      socket->Read(packet.GetAppendBuf(dataLen), dataLen);
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
// CNetSocket implementation
///////////////////////////////////////////////////////

CNetSocket::CNetSocket()
{
  task = ntNothing;
  errorCode = wxSocketBase::Initialize() ? NET_ERROR_SUCCESS : NET_ERROR_SOCKETS_UNAVAILABLE;
  if (IsOk())
  {
    // Set notification system
    socket.SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_OUTPUT_FLAG | wxSOCKET_CONNECTION_FLAG | wxSOCKET_LOST_FLAG);
    socket.SetEventHandler(*this);
    Bind(wxEVT_SOCKET, (wxObjectEventFunction)&CNetSocket::OnSocketEvent, this);
    socket.Notify(true);
  }
}

void CNetSocket::OnSocketEvent(wxSocketEvent& event)
{
  switch (event.GetSocketEvent())
  {
    case wxSOCKET_CONNECTION:
    {

    }
  }
}

void CNetSocket::Connect(const wxString& address)
{

}

void CNetSocket::SendPacket(const wxMemoryBuffer& packet)
{

}


void CNet::OnSocketEvent(wxSocketEvent& event)
{
  wxSocketBase* socket = event.GetSocket();
  switch (event.GetSocketEvent())
  {
    case wxSOCKET_CONNECTION:
    {
      if (!CurrentHost->HasKey())
      {
        RequestPublicKey();
        break;
      }
      else if (!cryptoProvider.LoadPublicRSAKey(CurrentHost->GetPublicKeyPath()))
      {
        // Validation error
        errorCode = NET_ERROR_INVALID_KEY;
        return false;
      }
    }
  }
}

CNet& CNet::Get()
{
  static CNet net;
  return net;
}

CNet::CNet()
{
  isConnected = false;
  task = ntNothing;

}

CNet::~CNet()
{
  wxSocketBase::Shutdown();
}

bool CNet::InitializeConnect()
{
  if (CurrentHost)
  {
    wxIPV4address addr;
    addr.Hostname(CurrentHost->GetHost());
    task = ntConnecting;
    socket.Connect(addr, false);
  }
  else
  {
    errorCode = NET_ERROR_HOST_NOT_SELECTED;
    return false;
  }
}

void CNet::RequestPublicKey()
{
  task = ntGettingKey;
  // Send request to server
}

void CNet::RequestHandshake()
{

}

void CNet::RequestRegistration()
{

}

void CNet::RequestAuthorization()
{

}

void CNet::RequestSynchronization()
{

}
