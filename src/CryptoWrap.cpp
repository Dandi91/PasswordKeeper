#include "CryptoWrap.h"

#include <sha.h>
#include <aes.h>
#include <modes.h>
#include <crc.h>
#include <filters.h>

#define AES256_KEY_SIZE 32
#define AES256_IV_SIZE  16

void CCryptoWrap::SHA256Digest(const wxString& plane, wxMemoryBuffer& digest)
{
  CryptoPP::SHA256().CalculateDigest((byte *)digest.GetAppendBuf(CryptoPP::SHA256::DIGESTSIZE), plane.c_str(), plane.Len());
}

void CCryptoWrap::AES256CTREncrypt(wxMemoryBuffer& dest, const wxMemoryBuffer& source, const size_t length, const wxMemoryBuffer& key, const wxMemoryBuffer& iv)
{
  CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption enc;
  enc.SetKeyWithIV((byte *)key.GetData(), AES256_KEY_SIZE, (byte *)iv.GetData());
  CryptoPP::ArraySource((byte *)source.GetData(), length, true, new CryptoPP::StreamTransformationFilter(enc, new CryptoPP::ArraySink((byte *)dest.GetAppendBuf(length), length)));
}

void CCryptoWrap::XORDigestIV(wxMemoryBuffer& buffer)
{
  char* dest = (char*)buffer.GetData();
  char* source = dest + AES256_IV_SIZE;
  for (size_t i = 0; i < AES256_IV_SIZE; ++i)
    *dest++ ^= *source++;
  buffer.SetDataLen(AES256_IV_SIZE);
}

void CCryptoWrap::CRC32Sum(const wxMemoryBuffer& buffer, uint32_t* lvalue)
{
  CryptoPP::CRC32().CalculateDigest((byte *)lvalue, (byte *)buffer.GetData(), buffer.GetDataLen());
}
