#ifndef CRYPTOFILE_H
#define CRYPTOFILE_H

#include <wx/string.h>
#include <wx/hashmap.h>

const static char CF_UNICODE    = 0x01;
const static char CF_NEWFORMAT  = 0x02;

class CRecord
{
public:
  wxString login;
  wxString email;
  wxString password;

  CRecord& operator=(const CRecord& right)
  {
    if (&right == this)
      return *this;
    login = right.login;
    email = right.email;
    password = right.password;
    return *this;
  }

  friend bool operator==(const CRecord& left, const CRecord& right)
  {
    return (left.login == right.login) && (left.email == right.email) && (left.password == right.password);
  };

  friend bool operator!=(const CRecord& left, const CRecord& right)
  {
    return !(left == right);
  };
};

WX_DECLARE_STRING_HASH_MAP(CRecord, CContent);

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
  bool WriteFile(const bool isUnicode, const bool isNewFormat);

  const wxString& GetFileName() { return fname; }
  const wxString& GetFilePath() { return fpath; }

  void SetFilePath(const wxString& path);
};

#endif // CRYPTOFILE_H
