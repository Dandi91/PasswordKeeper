#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "Content.h"

#include <wx/string.h>
#include <wx/filename.h>
#include <wx/buffer.h>

const static int AC_ERROR_SUCCESS           = 0;
const static int AC_ERROR_ALREADY_EXISTS    = 1;
const static int AC_ERROR_WRONG_LOGIN       = 2;
const static int AC_ERROR_WRONG_PASSWORD    = 3;
const static int AC_ERROR_WRITING           = 4;
const static int AC_ERROR_UNAUTHORIZED      = 5;

class CAccount
{
private:
  CContent fContent;

  wxString fLogin;
  wxMemoryBuffer fPasswordHash;
  wxFileName fFile;

  int fErrorCode;
  bool fIsSaved;
  bool fIsAuthorized;
public:
  // Static getter for single-instance use
  static CAccount& Get();

  // Authorization functions
  const int Authorize(const wxString& login, const wxString& password, const bool createNew);
  const int Deauthorize();

  // Content-related functions
  CContent* GetContent() { return &fContent; };
  const int SaveContent();

  // Error handling
  const bool IsOk() const { return fErrorCode == AC_ERROR_SUCCESS; };
  const int GetErrorCode() const { return fErrorCode; };
  const wxString& GetErrorMessage() const;

  // Properties
  const wxString& GetLogin() const { return fLogin; };
  const int SetLogin(const wxString& value)
  {
    if (fIsAuthorized)
    {
      fLogin = value;
      fErrorCode = AC_ERROR_SUCCESS;
    }
    else
      fErrorCode = AC_ERROR_UNAUTHORIZED;
    return fErrorCode;
  };

  void SetSaved(const bool value) { fIsSaved = value; };
  const int SetPassword(const wxString& value);

  const bool IsAuthorized() const { return fIsAuthorized; };
  const bool IsSaved() const { return fIsSaved; };

protected:
  CAccount() : fErrorCode(AC_ERROR_SUCCESS), fIsSaved(false), fIsAuthorized(false) {};

  bool ReadFile();
  bool WriteFile();
};

#endif // ACCOUNT_H
