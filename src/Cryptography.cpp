#include "Cryptography.h"

// CryptoPP headers
#include <sha.h>
#include <aes.h>
#include <modes.h>
#include <crc.h>
#include <filters.h>
#include <osrng.h>
#include <queue.h>
#include <files.h>

bool CRSAEncryption::LoadPublicRSAKey(const char* fileName)
{
  CryptoPP::FileSource file(fileName, true);

  CryptoPP::ByteQueue queue;
  file.TransferTo(queue);
  queue.MessageEnd();

  rsaPublic.Load(queue);

  // Validation
  CryptoPP::AutoSeededRandomPool rng;
  isValid = rsaPublic.Validate(rng, 3);
  return isValid;
}

size_t CRSAEncryption::RSAGetLength(const size_t length)
{
  if (!isValid)
    return 0;
  CryptoPP::RSAES_OAEP_SHA_Encryptor encryptor(rsaPublic);
  return encryptor.CiphertextLength(length);
}

void CRSAEncryption::RSAEncrypt(u_char* dest, const size_t destLength, const u_char* source, const size_t sourceLength)
{
  if (!isValid)
    return;

  CryptoPP::RSAES_OAEP_SHA_Encryptor encryptor(rsaPublic);
  CryptoPP::AutoSeededRandomPool rng;

  CryptoPP::ArraySource(source, sourceLength, true,  // Source
    new CryptoPP::PK_EncryptorFilter(rng, encryptor,  // Encryption
    new CryptoPP::ArraySink(dest, destLength)));     // Destination
}

bool CRSAEncryption::RSAVerify(const u_char* source, const size_t sourceLength, const u_char* signature, const size_t sigLength)
{
  if (!isValid)
    return false;

  CryptoPP::RSASSA_PKCS1v15_SHA_Verifier verifier(rsaPublic);
  return verifier.VerifyMessage(source, sourceLength, signature, sigLength);
}

void SHADigest(const u_char* plane, const size_t length, u_char* digest)
{
  CryptoPP::SHA256().CalculateDigest(digest, plane, length);
}

void AESEncrypt(u_char* dest, const u_char* source, const size_t length, const u_char* key, const u_char* iv)
{
  CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption enc;
  enc.SetKeyWithIV(key, AES256_KEY_SIZE, iv);
  CryptoPP::ArraySource(source, length, true,       // Source
    new CryptoPP::StreamTransformationFilter(enc,   // Encryption
    new CryptoPP::ArraySink(dest, length)));        // Destination
}

void XORDigestIV(const u_char* source, u_char* dest)
{
  const u_char* src1 = source;
  const u_char* src2 = source + AES256_IV_SIZE;
  for (size_t i = 0; i < AES256_IV_SIZE; ++i)
    *dest++ = *src1++ ^ *src2++;
}

void CRCSum(const u_char* buffer, const size_t length, unsigned long* lvalue)
{
  CryptoPP::CRC32().CalculateDigest((byte*)lvalue, buffer, length);
}
