#include "wxCryptography.h"

bool wxRSAEncryption::LoadPublicRSAKey(const wxString& fileName)
{
  return LoadPublicRSAKey(fileName.c_str());
}

void wxRSAEncryption::RSAEncrypt(wxMemoryBuffer& dest, const wxMemoryBuffer& source)
{
  size_t destLength = RSAGetLength(source.GetDataLen());
  CRSAEncryption::RSAEncrypt((u_char*)dest.GetAppendBuf(destLength), destLength, (u_char*)source.GetData(), source.GetDataLen());
}

bool wxRSAEncryption::RSAVerify(const wxMemoryBuffer& source, const wxMemoryBuffer& signature)
{
  return CRSAEncryption::RSAVerify((u_char*)source.GetData(), source.GetDataLen(), (u_char*)signature.GetData(), signature.GetDataLen());
}

void AESEncrypt(wxMemoryBuffer& dest, const wxMemoryBuffer& source, const size_t length, const wxMemoryBuffer& key, const wxMemoryBuffer& iv)
{
  AESEncrypt((u_char*)dest.GetAppendBuf(length), (u_char*)source.GetData(), length, (u_char*)key.GetData(), (u_char*)iv.GetData());
}

void XORDigestIV(wxMemoryBuffer& buffer)
{
  XORDigestIV((u_char*)buffer.GetData(), (u_char*)buffer.GetData());
}

void SHADigest(const wxString& plane, wxMemoryBuffer& digest)
{
  SHADigest(plane.c_str(), plane.length(), (u_char*)digest.GetAppendBuf(SHAGetLength()));
}

void CRCSum(const wxMemoryBuffer& buffer, unsigned long* lvalue)
{
  CRCSum((u_char*)buffer.GetData(), buffer.GetDataLen(), lvalue);
}
