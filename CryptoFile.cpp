
#include "CryptoFile.h"

#include <wx/wfstream.h>
#include <wx/dynarray.h>

typedef unsigned long key_t;
const key_t CLOSE_KEY = 0x13220F19;

CCryptoFile::CCryptoFile()
{
  isSaved = true;
}

CCryptoFile::CCryptoFile(const wxString& path)
{
  content = new CContent;
//  content->DeleteContents(true);
  fpath = path;
  fname = fpath.AfterLast('\\').BeforeLast('.');
  if (!path.Cmp('?'))
  {
    isSaved = true;
    fname = '?';
  }
  else
    isSaved = ReadFile();
}

CCryptoFile::~CCryptoFile()
{
  delete content;
}

void CCryptoFile::SetFilePath(const wxString& path)
{
  fpath = path;
  fname = fpath.AfterLast('\\').BeforeLast('.');
}

void CCryptoFile::MergeWith(const CCryptoFile& second)
{
  const CContent* content2 = second.content;
  CContent::const_iterator i;
  for (i = content2->begin(); i != content2->end(); ++i)
  {
    CContent::compatibility_iterator found = content->Find(*i);
    if (found == NULL)
    {
      // If key is new, write it as is
      content->Append(*i);
      isSaved = false;
    }
  }
}

bool CCryptoFile::ReadFile()
{
  // open file
  wxFileInputStream finput(fpath);
  if (!finput.IsOk())
    return false;

  // prepare buffer
  wxArrayLong buff;
  finput.SeekI(0, wxFromEnd);
  size_t len = finput.TellI();
  buff.SetCount(len / sizeof(key_t));
  len -= sizeof(key_t);

  // get key
  finput.SeekI(-(int)sizeof(key_t), wxFromEnd);
  key_t key;
  finput.Read(&key, sizeof(key_t));
  key ^= CLOSE_KEY;

  // read to buffer
  finput.SeekI(0);
  finput.Read(&buff[0], len);

  // decrypt
  for (size_t i = 0; i < buff.GetCount(); ++i)
    buff[i] ^= key;

  // detect format
  char* p = (char*)(&buff[0]);
  char format = *p;
  bool isNewFormat = false;
  bool isUnicode = false;
  if (format != '[')
  {
    isNewFormat = (format & CF_NEWFORMAT);
    isUnicode = (format & CF_UNICODE);
  }

  // parse to string array
  wxString s;
  if (isUnicode)
    s = wxString::FromUTF8(p + 1, len - 1);
  else
    s.Append(p, len);

  // parse to hash map
  while (s.Len() > 0)
  {
    long res = s.Find("\r\n");
    wxString entry = s.SubString(0, res - 1);
    wxString value;
    CRecord* rec = new CRecord;
    rec->name = entry.BeforeFirst('=', &value);
    if (isNewFormat)
    {
      rec->login = value.BeforeFirst('=', &entry);
      rec->email = entry.BeforeFirst('=', &rec->password);
    }
    else
      rec->password = value;
    if ((rec->name == "") || (rec->name == "[Main]"))
      delete rec;
    else
      content->Append(rec);
    s.Remove(0, res + 2);
  }
  return true;
}

key_t GenerateKey()
{
  return (((key_t)rand() << 16) | (key_t)rand()) * 2;
}

bool CCryptoFile::WriteFile(const bool isUnicode, const bool isNewFormat)
{
  // sorting
  SortContent();

  // parse to string
  wxString s;
  CContent::iterator i;
  for (i = content->begin(); i != content->end(); ++i)
  {
    CRecord* rec = *i;
    if (isNewFormat)
      s += rec->name + "=" + rec->login + "=" +
          rec->email + "=" + rec->password + "\r\n";
    else
      s += rec->name + "=" + rec->password + "\r\n";
  }
  s.Prepend("[Main]\r\n");
  s.Append("\r\n");

  // convert to char buffer
  wxScopedCharBuffer charbuff;
  if (isUnicode)
    charbuff = s.ToUTF8();
  else
    charbuff = s.char_str();
  size_t len = charbuff.length();
  if (isUnicode || isNewFormat)
    len++;  // for 1-byte new format mark

  // copy to buffer
  wxArrayLong buff;
  buff.SetCount(len / sizeof(key_t) + 1);
  char* p = (char*)&buff[0];
  if (isUnicode || isNewFormat)
  {
    memcpy(p + 1, charbuff, len - 1);
    // format mark
    char format = 0;
    if (isUnicode)
      format |= CF_UNICODE;
    if (isNewFormat)
      format |= CF_NEWFORMAT;
    *p = format;
  }
  else
    memcpy(p, charbuff, len);

  // encrypt
  key_t key = GenerateKey();
  for (size_t i = 0; i < buff.GetCount(); ++i)
    buff[i] ^= key;

  // open file
  wxFileOutputStream foutput(fpath);
  if (!foutput.IsOk())
    return false;

  // write to file
  foutput.Write(&buff[0], len);

  // set key
  key ^= CLOSE_KEY;
  foutput.Write(&key, sizeof(key_t));

  isSaved = true;
  return true;
}
