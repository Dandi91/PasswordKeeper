#ifndef CRYPTOFILE_H
#define CRYPTOFILE_H

#include <wx/string.h>
#include <wx/filename.h>
#include <wx/buffer.h>
#include "Content.h"

const static char CF_UNICODE    = 0x01;
const static char CF_NEWFORMAT  = 0x02;

const static int CF_ERROR_SUCCESS           = 0;
const static int CF_ERROR_ALREADY_EXISTS    = 1;
const static int CF_ERROR_WRONG_LOGIN       = 2;
const static int CF_ERROR_WRONG_PASSWORD    = 3;

class CCryptoFile
{
private:
  wxString fLogin;
  wxMemoryBuffer fPasswordHash;
  wxFileName fFile;
  int fErrorCode;
  bool fIsSaved;

public:
  CContent content;

  static CCryptoFile& Get();
  const int OpenFile(const wxString& login, const wxString& password, const bool createNew);

  const bool isOk() const { return fErrorCode == CF_ERROR_SUCCESS; };
  const int GetErrorCode() const { return fErrorCode; };
  const wxString& GetErrorMessage() const;

  void MergeWith(const CCryptoFile& second);
  bool WriteFile();

  const bool IsSaved() const { return fIsSaved; };
  const wxString& GetLogin() const { return fLogin; };

protected:
  CCryptoFile();

  bool ReadFile();
};

#endif // CRYPTOFILE_H
