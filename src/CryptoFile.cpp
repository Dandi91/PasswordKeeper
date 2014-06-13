#include "CryptoFile.h"

#include <wx/wfstream.h>
#include <wx/stdpaths.h>

#include "CryptoWrap.h"

const char fieldDelim   = '\r';
const char recordDelim  = '\n';

const wxString errorMessages[5] = {"OK", "Login already exists", "Unknown login", "Wrong password"};

CCryptoFile* account = NULL;

CCryptoFile& CCryptoFile::Get()
{
  if (account == NULL)
    account = new CCryptoFile();
  return *account;
}

const int CCryptoFile::OpenFile(const wxString& login, const wxString& password, const bool createNew = false)
{
  wxString dataDir = wxStandardPaths::Get().GetUserLocalDataDir();
  wxFileName fileName(dataDir, login, "pkf");
  if (createNew)
  {
    if (!wxDirExists(dataDir))
      wxMkDir(dataDir);
    if (fileName.Exists())
    {
      fErrorCode = CF_ERROR_ALREADY_EXISTS;
      return fErrorCode;
    }
    fLogin = login;
    fFile = fileName;
    SHA256Digest(password, fPasswordHash);
  }
  else
  {
    if (!fileName.Exists())
    {
      fErrorCode = CF_ERROR_WRONG_LOGIN;
      return fErrorCode;
    }
    fLogin = login;
    fFile = fileName;
    SHA256Digest(password, fPasswordHash);
    fErrorCode = ReadFile() ? CF_ERROR_SUCCESS : CF_ERROR_WRONG_PASSWORD;
  }
  return fErrorCode;
}

CCryptoFile::CCryptoFile()
{
  fErrorCode = CF_ERROR_SUCCESS;
  fIsSaved = false;
}

const wxString& CCryptoFile::GetErrorMessage() const
{
  return errorMessages[fErrorCode];
}

void CCryptoFile::MergeWith(const CCryptoFile& second)
{
  for (size_t i = 0; i < second.content.GetCount(); ++i)
  {
    if (content.Find(second.content[i]) == -1)
    {
      // If key is new, write it as is
      content.Add(second.content[i]);
      fIsSaved = false;
    }
  }
}

bool CCryptoFile::ReadFile()
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
  finput.Read(buff.GetData(), len);

  // decrypt
  wxMemoryBuffer iv;
  SHA256Digest(fLogin, iv);
  XORDigestIV(iv);
  AES256CTREncrypt(decrBuff, buff, len, fPasswordHash, iv);

  // CRC
  uint32_t fileCRC32, calcCRC32;
  fileCRC32 = *(((char*)decrBuff.GetData()) + len - sizeof(uint32_t));
  CRC32Sum(decrBuff, len - sizeof(uint32_t), &calcCRC32);
  if (calcCRC32 != fileCRC32)
    return false;

  // parse to string array
  wxString s = wxString::FromUTF8((char*)decrBuff.GetData(), len - sizeof(uint32_t));

  // parse to CContent
  while (s.Len() > 0)
  {
    long res = s.Find(recordDelim);
    wxString entry = s.SubString(0, res - 1);
    wxString value;
    CRecord rec;
    rec.name = entry.BeforeFirst(fieldDelim, &value);
    rec.login = value.BeforeFirst(fieldDelim, &entry);
    rec.email = entry.BeforeFirst(fieldDelim, &rec.password);
    content.Add(rec);
    s.Remove(0, res + 1);
  }

  fIsSaved = true;
  return true;
}

bool CCryptoFile::WriteFile()
{
  content.Sort();

  // parse to string
  wxString s;
  for (size_t i = 0; i < content.GetCount(); ++i)
  {
    CRecord* rec = &content[i];
    s += rec->name + fieldDelim + rec->login + fieldDelim +
         rec->email + fieldDelim + rec->password;
    if (i != content.GetCount() - 1)
      s.Append(recordDelim);
  }

  // convert to char buffer
  wxScopedCharBuffer charbuff;
  charbuff = s.ToUTF8();
  size_t len = charbuff.length();

  // copy to buffer
  wxMemoryBuffer buff(len);
  memcpy(buff.GetData(), charbuff, len);

  // CRC
  uint32_t calcCRC32;
  CRC32Sum(buff, len, &calcCRC32);
  buff.AppendData(&calcCRC32, sizeof(uint32_t));
  len += sizeof(uint32_t);

  // encrypt
  wxMemoryBuffer iv;
  wxMemoryBuffer encrBuff(len);
  SHA256Digest(fLogin, iv);
  XORDigestIV(iv);
  AES256CTREncrypt(encrBuff, buff, len, fPasswordHash, iv);

  // open file
  wxFileOutputStream foutput(fFile.GetFullPath());
  if (!foutput.IsOk())
    return false;

  // write to file
  foutput.Write(encrBuff.GetData(), len);

  fIsSaved = true;
  return true;
}
