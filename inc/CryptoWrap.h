#ifndef CRYPTOWRAP_H
#define CRYPTOWRAP_H

#include <wx/string.h>
#include <wx/buffer.h>

class CCryptoWrap
{
public:
  static void SHA256Digest(const wxString& plane, wxMemoryBuffer& digest);
  static void AES256CTREncrypt(wxMemoryBuffer& dest, const wxMemoryBuffer& source, const size_t length, const wxMemoryBuffer& key, const wxMemoryBuffer& iv);
  static void XORDigestIV(wxMemoryBuffer& buffer);
  static void CRC32Sum(const wxMemoryBuffer& buffer, uint32_t* lvalue);
};

#endif // CRYPTOWRAP_H
