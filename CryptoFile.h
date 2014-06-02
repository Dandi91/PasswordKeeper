#ifndef CRYPTOFILE_H
#define CRYPTOFILE_H

#include <wx/string.h>
#include <wx/hashmap.h>

typedef wxStringToStringHashMap CContent;

class CCryptoFile
{
private:
  wxString fpath;
  wxString fname;

public:
  bool isSaved;
  CContent content;

  CCryptoFile();
  CCryptoFile(const wxString& path);
  void MergeWith(const CCryptoFile& second);
  bool ReadFile();
  bool WriteFile(bool isUnicode);

  const wxString& GetFileName();
  const wxString& GetFilePath();
  void SetFilePath(const wxString& path);
};

#endif // CRYPTOFILE_H
