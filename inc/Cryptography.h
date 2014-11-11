#ifndef CRYPTOGRAPHY_H
#define CRYPTOGRAPHY_H

// Cryptography.h provides interfaces for cryptopp library algorythms that are used by application

#define AES256_KEY_SIZE 32
#define AES256_IV_SIZE  16

#include <rsa.h>

// CRSAEncryption provides CryptoPP::RSA::PublicKey wrapping for more convenient use
class CRSAEncryption
{
private:
  bool isValid;
  CryptoPP::RSA::PublicKey rsaPublic;

public:
  CRSAEncryption() { isValid = false; };

  // RSA functions
  bool LoadPublicRSAKey(const char* fileName);
  size_t RSAGetLength(const size_t length);
  void RSAEncrypt(u_char* dest, const size_t destLength, const u_char* source, const size_t sourceLength);
  bool RSAVerify(const u_char* source, const size_t length, const u_char* signature, const size_t sigLength);
};

// Static functions for AES and digests

// AES two-way function
void AESEncrypt(u_char* dest, const u_char* source, const size_t length, const u_char* key, const u_char* iv);
void XORDigestIV(const u_char* source, u_char* dest);  // Makes 128-bit IV from 256-bit digest

// Digests
void SHADigest(const u_char* plane, const size_t length, u_char* digest);
#define SHAGetLength() (CryptoPP::SHA256::DIGESTSIZE)
void CRCSum(const u_char* buffer, const size_t length, unsigned long* lvalue);

#endif // CRYPTOGRAPHY_H
