#ifndef CRYPTOFILE_H
#define CRYPTOFILE_H

#include <wx/string.h>
#include "Content.h"

const static char CF_UNICODE    = 0x01;
const static char CF_NEWFORMAT  = 0x02;

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

  const wxString& GetFileName() const { return fname; };
  const wxString& GetFilePath() const { return fpath; };

  void SetFilePath(const wxString& path);
};

#endif // CRYPTOFILE_H
