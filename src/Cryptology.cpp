#include "Cryptology.h"

// CryptoPP headers
#include <sha.h>
#include <aes.h>
#include <modes.h>
#include <crc.h>
#include <filters.h>
#include <osrng.h>
#include <queue.h>
#include <files.h>

bool CRSAEncryption::LoadPublicRSAKey(const wxString& fileName)
{
  CryptoPP::FileSource file(fileName.c_str(), true);

  CryptoPP::ByteQueue queue;
  file.TransferTo(queue);
  queue.MessageEnd();

  rsaPublic.Load(queue);

  // Validation
  CryptoPP::AutoSeededRandomPool rng;
  isValid = rsaPublic.Validate(rng, 3);
  return isValid;
}

void CRSAEncryption::RSAEncrypt(wxMemoryBuffer& dest, const wxMemoryBuffer& source)
{
  if (!isValid)
    return;

  CryptoPP::RSAES_OAEP_SHA_Encryptor encryptor(rsaPublic);
  CryptoPP::AutoSeededRandomPool rng;
  size_t cipherTextSize = encryptor.CiphertextLength(source.GetDataLen());

  CryptoPP::ArraySource((byte*)source.GetData(), source.GetDataLen(), true,                                   // Source
                        new CryptoPP::PK_EncryptorFilter(rng, encryptor,                                      // Encryptor
                        new CryptoPP::ArraySink((byte*)dest.GetAppendBuf(cipherTextSize), cipherTextSize)));  // Destination
}

bool CRSAEncryption::RSAVerify(const wxMemoryBuffer& source, const wxMemoryBuffer& signature)
{
  if (!isValid)
    return false;

  CryptoPP::RSASSA_PKCS1v15_SHA_Verifier verifier(rsaPublic);
  return verifier.VerifyMessage((const byte*)source.GetData(), source.GetDataLen(), (const byte*)signature.GetData(), signature.GetDataLen());
}

void SHADigest(const wxString& plane, wxMemoryBuffer& digest)
{
  CryptoPP::SHA256().CalculateDigest((byte*)digest.GetAppendBuf(CryptoPP::SHA256::DIGESTSIZE), plane.c_str(), plane.Len());
}

void AESEncrypt(wxMemoryBuffer& dest, const wxMemoryBuffer& source, const size_t length, const wxMemoryBuffer& key, const wxMemoryBuffer& iv)
{
  CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption enc;
  enc.SetKeyWithIV((byte*)key.GetData(), AES256_KEY_SIZE, (byte*)iv.GetData());
  CryptoPP::ArraySource((byte*)source.GetData(), length, true,                                  // Source
                        new CryptoPP::StreamTransformationFilter(enc,                           // Encryption
                        new CryptoPP::ArraySink((byte*)dest.GetAppendBuf(length), length)));    // Destination
}

void XORDigestIV(wxMemoryBuffer& buffer)
{
  char* dest = (char*)buffer.GetData();
  char* source = dest + AES256_IV_SIZE;
  for (size_t i = 0; i < AES256_IV_SIZE; ++i)
    *dest++ ^= *source++;
  buffer.SetDataLen(AES256_IV_SIZE);
}

void CRCSum(const wxMemoryBuffer& buffer, unsigned long* lvalue)
{
  CryptoPP::CRC32().CalculateDigest((byte*)lvalue, (byte*)buffer.GetData(), buffer.GetDataLen());
}
