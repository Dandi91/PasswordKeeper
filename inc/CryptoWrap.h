#ifndef CRYPTOWRAP_H
#define CRYPTOWRAP_H

#include <wx/string.h>
#include <wx/buffer.h>

void SHA256Digest(const wxString& plane, wxMemoryBuffer& digest);
void AES256CTREncrypt(wxMemoryBuffer& dest, const wxMemoryBuffer& source, const size_t length, const wxMemoryBuffer& key, const wxMemoryBuffer& iv);
void XORDigestIV(wxMemoryBuffer& buffer);
void CRC32Sum(const wxMemoryBuffer& buffer, size_t length, uint32_t* lvalue);

#endif // CRYPTOWRAP_H
