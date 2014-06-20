#ifndef NET_H
#define NET_H

#include <wx/socket.h>
#include <wx/buffer.h>

#include "Cryptology.h"
#include "Account.h"

class CCryptoProvider : public CRSAEncryption
{
private:
  wxMemoryBuffer aesKey, aesIV;
  void GeterateAESKey();

public:
  CCryptoProvider();

  void AESEncrypt(wxMemoryBuffer& dest, const wxMemoryBuffer& source, const size_t length);
};

class CNet
{
private:
  bool isConnected;
  CCryptoProvider cryptoProvider;
  wxSocketClient socket;

public:
  static CNet& Get();
  virtual ~CNet();

  void InitializeConnect(const wxIPV4address& host);
  void RequestPublicKey();
  void RequestHandshake();

  void RequestRegistration(const CAccount& account);
  void RequestAuthorization(const CAccount& account);

  void RequestSynchronization(const CAccount& account);
protected:
  CNet() { isConnected = false; };
};

#endif // NET_H
