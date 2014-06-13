#include "CryptoWrap.h"

#include <sha.h>
#include <aes.h>
#include <modes.h>
#include <crc.h>
#include <filters.h>

#define AES256_KEY_SIZE 32
#define AES256_IV_SIZE  16

namespace CryptoPP
{

void SHA256Digest(const wxString& plane, wxMemoryBuffer& digest)
{
  digest.SetBufSize(SHA256::DIGESTSIZE);
  SHA256().CalculateDigest((byte *)digest.GetData(), plane.c_str(), plane.Len());
}

void AES256CTREncrypt(wxMemoryBuffer& dest, const wxMemoryBuffer& source, const size_t length, const wxMemoryBuffer& key, const wxMemoryBuffer& iv)
{
  CTR_Mode<AES>::Encryption enc;
  enc.SetKeyWithIV((byte *)key.GetData(), AES256_KEY_SIZE, (byte *)iv.GetData());
  ArraySource((byte *)source.GetData(), length, true, new StreamTransformationFilter(enc, new ArraySink((byte *)dest.GetData(), length)));
}

void XORDigestIV(wxMemoryBuffer& buffer)
{
  char* dest = (char*)buffer.GetData();
  char* source = dest + AES256_IV_SIZE;
  for (size_t i = 0; i < AES256_IV_SIZE; ++i)
    *dest++ ^= *source++;
  buffer.SetDataLen(AES256_IV_SIZE);
}

void CRC32Sum(const wxMemoryBuffer& buffer, size_t length, uint32_t* lvalue)
{
  CRC32().CalculateDigest((byte *)lvalue, (byte *)buffer.GetData(), length);
}

}
