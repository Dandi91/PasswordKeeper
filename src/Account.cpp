#include "Account.h"

#include <wx/wfstream.h>
#include <wx/stdpaths.h>

#include "CryptoWrap.h"
#include "ContentParser.h"

const wxString errorMessages[] = {"OK",
                                   "Login already exists",
                                   "Unknown login",
                                   "Wrong login/password",
                                   "Writing error",
                                   "Unathorizated access prohibited"};

CAccount* account = NULL;

CAccount& CAccount::Get()
{
  if (account == NULL)
    account = new CAccount();
  return *account;
}

const int CAccount::Authorize(const wxString& login, const wxString& password, const bool createNew = false)
{
  if (fIsAuthorized)
    if (Deauthorize() != AC_ERROR_SUCCESS)
      return fErrorCode;
  fErrorCode = AC_ERROR_SUCCESS;
  wxString dataDir = wxStandardPaths::Get().GetUserLocalDataDir();
  wxFileName fileName(dataDir, login, "pkf");
  if (createNew)
  {
    if (!wxDirExists(dataDir))
      wxMkDir(dataDir);
    if (fileName.Exists())
    {
      fErrorCode = AC_ERROR_ALREADY_EXISTS;
      return fErrorCode;
    }
    fLogin = login;
    fFile = fileName;
    CCryptoWrap::SHA256Digest(password, fPasswordHash);
  }
  else
  {
    if (!fileName.Exists())
    {
      fErrorCode = AC_ERROR_WRONG_LOGIN;
      return fErrorCode;
    }
    fLogin = login;
    fFile = fileName;
    CCryptoWrap::SHA256Digest(password, fPasswordHash);
    fErrorCode = ReadFile() ? AC_ERROR_SUCCESS : AC_ERROR_WRONG_PASSWORD;
  }
  if (IsOk())
    fIsAuthorized = true;
  return fErrorCode;
}

const int CAccount::Deauthorize()
{
  fErrorCode = AC_ERROR_SUCCESS;
  if (fIsAuthorized)
  {
    if (!WriteFile())
      fErrorCode = AC_ERROR_WRITING;
    fLogin.Clear();
    fPasswordHash.Clear();
    fContent.Clear();
    fIsAuthorized = false;
  }
  return fErrorCode;
}

const wxString& CAccount::GetErrorMessage() const
{
  return errorMessages[fErrorCode];
}

const int CAccount::SetPassword(const wxString& value)
{
  if (fIsAuthorized)
  {
    CCryptoWrap::SHA256Digest(value, fPasswordHash);
    fErrorCode = AC_ERROR_SUCCESS;
  }
  else
    fErrorCode = AC_ERROR_UNAUTHORIZED;
  return fErrorCode;
}

const int CAccount::SaveContent()
{
  if (fIsAuthorized)
    fErrorCode = WriteFile() ? AC_ERROR_SUCCESS : AC_ERROR_WRITING;
  else
    fErrorCode = AC_ERROR_UNAUTHORIZED;
  return fErrorCode;
}

bool CAccount::ReadFile()
{
  // open file
  wxFileInputStream finput(fFile.GetFullPath());
  if (!finput.IsOk())
    return false;

  // prepare buffers
  finput.SeekI(0, wxFromEnd);
  size_t len = finput.TellI();
  wxMemoryBuffer buff(len);
  wxMemoryBuffer decrBuff(len);

  // read to buffer
  finput.SeekI(0);
  finput.Read(buff.GetAppendBuf(len), len);

  // decrypt
  wxMemoryBuffer iv;
  CCryptoWrap::SHA256Digest(fLogin, iv);
  CCryptoWrap::XORDigestIV(iv);
  CCryptoWrap::AES256CTREncrypt(decrBuff, buff, len, fPasswordHash, iv);

  // CRC
  uint32_t fileCRC32, calcCRC32 = 0;
  len -= sizeof(fileCRC32);
  char* CRC32position = (char*)decrBuff.GetData() + len;
  fileCRC32 = *(uint32_t*)CRC32position;
  decrBuff.SetDataLen(len);
  CCryptoWrap::CRC32Sum(decrBuff, &calcCRC32);
  if (calcCRC32 != fileCRC32)
    return false;

  // parse to CContent
  CContentParser::ParseFromBuffer(decrBuff, fContent);

  fIsSaved = true;
  return true;
}

bool CAccount::WriteFile()
{
  fContent.Sort();

  // parse to buffer
  wxMemoryBuffer buff;
  CContentParser::ParseToBuffer(fContent, buff);

  // CRC
  uint32_t calcCRC32 = 0;
  CCryptoWrap::CRC32Sum(buff, &calcCRC32);
  buff.AppendData(&calcCRC32, sizeof(calcCRC32));
  size_t len = buff.GetDataLen();

  // encrypt
  wxMemoryBuffer iv;
  wxMemoryBuffer encrBuff(len);
  CCryptoWrap::SHA256Digest(fLogin, iv);
  CCryptoWrap::XORDigestIV(iv);
  CCryptoWrap::AES256CTREncrypt(encrBuff, buff, len, fPasswordHash, iv);

  // open file
  wxFileOutputStream foutput(fFile.GetFullPath());
  if (!foutput.IsOk())
    return false;

  // write to file
  foutput.Write(encrBuff.GetData(), len);

  fIsSaved = true;
  return true;
}