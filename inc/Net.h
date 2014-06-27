#ifndef NET_H
#define NET_H

#include <wx/socket.h>
#include <wx/thread.h>
#include <wx/event.h>
#include <wx/buffer.h>

#include "Cryptology.h"

#define NET_CLIENT_TIMEOUT  60

// CCryptoProvider is a helper class that provides cryptographic functions for CNet class
class CCryptoProvider : public CRSAEncryption
{
private:
  wxMemoryBuffer aesKey, aesIV;

public:
  // Default constructor.
  CCryptoProvider() {};
  // Generates random key and iv for AES encryption. Should be called before each new handshake
  void GenerateAESValues();
  // Encrypts buffer with AES algorithm using randomly generated key and iv for the session. Adds CRC checksum for validating
  void Encrypt(wxMemoryBuffer& dest, const wxMemoryBuffer& source);
  // Separates RSA signature and validates packet. Decrypts packet with AES algorithm using key and iv. Validates CRC checksum
  bool Decrypt(wxMemoryBuffer& dest, const wxMemoryBuffer& source);
  // Fully creates handshake packet with AES key and iv and applies RSA encryption to it
  void GenerateHandshake(wxMemoryBuffer& buffer);
  // Appends SHA digest to buffer
  void PutStringDigest(const wxString& string, wxMemoryBuffer& buffer);
};

enum NetError
{
  NET_ERROR_SUCCESS = 0,
  NET_ERROR_SOCKETS_UNAVAILABLE,
  NET_ERROR_CANT_RUN_THREAD,
  NET_ERROR_INVALID_KEY,
  NET_ERROR_HANDSHAKE_CLIENT_FAILED,
  NET_ERROR_HANDSHAKE_SERVER_FAILED,
  NET_ERROR_VERIFYING_FAILED,
  NET_ERROR_HOST_NOT_SELECTED,
  NET_ERROR_CONNECTING_TO_HOST,
  NET_ERROR_SENDING_FAILED,
  NET_ERROR_RECEIVING_FAILED
};

enum NetEvent
{
  NET_EVENT_CONNECTED = 1,
  NET_EVENT_TASK_COMPLETED,
  NET_EVENT_DISCONNECTED,
  NET_EVENT_ERROR
};

struct NetMsg
{
  NetEvent event;
  NetError error;
};

const wxEventType wxEVT_CLIENT = wxNewEventType();

// ClientEvent is an event class for notifying the main thread about working thread's events
class ClientEvent : public wxEvent
{
public:
  ClientEvent(void* pSender, NetMsg message)
  {
    SetId(-1);
    SetEventType(wxEVT_CLIENT);
    sender = pSender;
    msg = message;
  };

  void SetMessage(const NetMsg message) { msg = message; };
  NetMsg GetMessage() const { return msg; };

  virtual wxEvent* Clone() const { return new ClientEvent(*this); };

  void* sender;
  NetMsg msg;
};

// Working thread
class CNetThread : public wxThread
{
private:
  wxMemoryBuffer packet;
  bool isPacketReady;
  wxSocketClient* socket;
  wxString hostName;
  unsigned short service;
  wxCriticalSection dataCS;
  wxEvtHandler* mainThreadListener;

  void NotifyError(const NetError error);
  void NotifyMainThread(const NetEvent event, const NetError error);

public:
  CNetThread(const wxString& host, const unsigned short port, wxEvtHandler* listener);
  void SendPacket(const wxMemoryBuffer& content);
  void ResendPacket();
  void ReceivePacket(wxMemoryBuffer& content);

protected:
  virtual wxThread::ExitCode Entry();
  void Cleanup();
};

// CNet implements high-level net functions
class CNet : public wxEvtHandler
{
private:
  enum NetTask
  {
    ntNothing = 0,
    ntConnecting,
    ntGettingKey,
    ntHandshaking,
    ntTransmission
  };

  NetTask task;
  NetError errorCode;
  bool handshaked;
  CCryptoProvider cryptoProvider;
  void OnClientEvent(ClientEvent& event);
  void SendNotification(const NetError code);

  void SavePublicKey();

  void PutString(const wxString& string, wxMemoryBuffer& packet)
  {
    unsigned long len = string.Len();
    packet.AppendData(&len, sizeof(len));
    packet.AppendData(string.c_str(), len);
  };

  char* GetString(wxString& string, char* pointer)
  {
    unsigned long len = *(unsigned long*)pointer;
    pointer += sizeof(len);
    string.Append(pointer, len);
    return pointer + len;
  }

public:
  static CNet& Get();
  virtual ~CNet();

  // This function does the whole sequence of connection, requesting public key (if needed), and handshaking
  bool InitializeConnect();

  void RequestRegistration(const wxString& login, const wxString& password);
  void RequestAuthorization(const wxString& login, const wxString& password);
  void RequestSynchronization();

  // Error handling
  NetError GetError() const { return errorCode; };
  bool IsOk() const { return errorCode == NET_ERROR_SUCCESS; };

protected:
  CNet();

  bool ValidateKey();
  void RequestPublicKey();
  void DoHandshake();

  CNetThread* thread;
};

#endif // NET_H
