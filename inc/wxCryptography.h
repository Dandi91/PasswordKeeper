#ifndef WXCRYPTOGRAPHY_H
#define WXCRYPTOGRAPHY_H

#include "Cryptography.h"

#include <wx/string.h>
#include <wx/buffer.h>

class wxRSAEncryption: public CRSAEncryption
{
public:
  bool LoadPublicRSAKey(const wxString& fileName);
  void RSAEncrypt(wxMemoryBuffer& dest, const wxMemoryBuffer& source);
  bool RSAVerify(const wxMemoryBuffer& source, const wxMemoryBuffer& signature);
};

void AESEncrypt(wxMemoryBuffer& dest, const wxMemoryBuffer& source, const size_t length, const wxMemoryBuffer& key, const wxMemoryBuffer& iv);
void XORDigestIV(wxMemoryBuffer& buffer);
void SHADigest(const wxString& plane, wxMemoryBuffer& digest);
void CRCSum(const wxMemoryBuffer& buffer, unsigned long* lvalue);

#endif // WXCRYPTOGRAPHY_H
