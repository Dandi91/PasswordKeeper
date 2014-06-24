#ifndef CRYPTOLOGY_H
#define CRYPTOLOGY_H

// Cryptology.h provides interfaces for cryptopp library algorythms that are used by application

#define AES256_KEY_SIZE 32
#define AES256_IV_SIZE  16

#include <rsa.h>

#include <wx/string.h>
#include <wx/buffer.h>

// CRSAEncryption provides CryptoPP::RSA::PublicKey wrapping for more convinient use
class CRSAEncryption
{
private:
  bool isValid;
  CryptoPP::RSA::PublicKey rsaPublic;

public:
  CRSAEncryption() { isValid = false; };

  // RSA functions
  bool LoadPublicRSAKey(const wxString& fileName);
  void RSAEncrypt(wxMemoryBuffer& dest, const wxMemoryBuffer& source);
  bool RSAVerify(const wxMemoryBuffer& source, const wxMemoryBuffer& signature);
};

// Static functions for AES and digests

// AES two-way function
void AESEncrypt(wxMemoryBuffer& dest, const wxMemoryBuffer& source, const size_t length, const wxMemoryBuffer& key, const wxMemoryBuffer& iv);
void XORDigestIV(wxMemoryBuffer& buffer);  // Makes 128-bit IV from 256-bit digest

// Didgests
void SHADigest(const wxString& plane, wxMemoryBuffer& digest);
void CRCSum(const wxMemoryBuffer& buffer, unsigned long* lvalue);

#endif // CRYPTOLOGY_H
