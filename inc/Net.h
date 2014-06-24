#ifndef NET_H
#define NET_H

#include <wx/socket.h>
#include <wx/thread.h>
#include <wx/event.h>
#include <wx/buffer.h>

#include "Cryptology.h"
#include "Account.h"

#define NET_SERVER_PORT     35100
#define NET_CLIENT_TIMEOUT  60

// CCryptoProvider is a helper class that provides cryptographic functions for CNet class
class CCryptoProvider : public CRSAEncryption
{
private:
  wxMemoryBuffer aesKey, aesIV;
  void GenerateAESKey();

public:
  CCryptoProvider() { GenerateAESKey(); };
  void AESEncrypt(wxMemoryBuffer& dest, const wxMemoryBuffer& source, const size_t length);
};

enum NetError
{
  NET_ERROR_SUCCESS = 0,
  NET_ERROR_SOCKETS_UNAVAILABLE,
  NET_ERROR_INVALID_KEY,
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
  wxCriticalSection dataCS;
  wxEvtHandler* mainThreadListener;

  void NotifyError(const NetError error);
  void NotifyMainThread(const NetEvent event, const NetError error = NET_ERROR_SUCCESS);

public:
  CNetThread(const wxString& host, wxEvtHandler* listener);
  void SendPacket(const wxMemoryBuffer& content);

protected:
  virtual wxThread::ExitCode Entry();
  void Cleanup();
};

// CNetSocket implements low-level socket functions
class CNetSocket : public wxEvtHandler
{
private:
  enum NetTask  {
                  ntNothing = 0,
                  ntConnecting,
                  ntGettingKey,
                  ntHandshaking,
                  ntSending,
                  ntReceiving
                };

  wxSocketClient socket;
  NetTask task;
  int errorCode;
  void OnSocketEvent(wxSocketEvent& event);

public:
  CNetSocket();

  void Connect(const wxString& address);
  void SendPacket(const wxMemoryBuffer& packet);

  int GetErrorCode() const { return errorCode; };
  bool IsOk() const { return errorCode == NET_ERROR_SUCCESS; };

protected:
  CNetThread *thread;

};

// CNet implements high-level net functions
class CNet
{
private:


  bool isConnected;

  int errorCode;
  CCryptoProvider cryptoProvider;



public:
  static CNet& Get();
  virtual ~CNet();

  bool InitializeConnect();
  void RequestPublicKey();
  void RequestHandshake();

  void RequestRegistration();
  void RequestAuthorization();

  void RequestSynchronization();


protected:
  CNet();
};

#endif // NET_H
