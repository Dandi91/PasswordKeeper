
#include "CryptoFile.h"

#include <wx/wfstream.h>
#include <wx/dynarray.h>

typedef unsigned long key_t;
const key_t CLOSE_KEY = 0x13220F19;

CCryptoFile::CCryptoFile()
{
  isSaved = false;
}

CCryptoFile::CCryptoFile(const wxString& path)
{
  fpath = path;
  fname = fpath.AfterLast('\\').BeforeLast('.');
  if (!path.Cmp('?'))
    isSaved = false;
  else
    isSaved = ReadFile();
}

const wxString& CCryptoFile::GetFileName()
{
  return fname;
}

const wxString& CCryptoFile::GetFilePath()
{
  return fpath;
}

void CCryptoFile::SetFilePath(const wxString& path)
{
  fpath = path;
  fname = fpath.AfterLast('\\').BeforeLast('.');
}

void CCryptoFile::MergeWith(const CCryptoFile& second)
{
  CContent* c1 = &this.content;
  const CContent* c2 = &second.content;
  CContent::const_iterator i;
  for (i = c2->begin(); i != c2->end(); ++i)
  {
    wxString s = i->first;
    if (c1->find(s) != c1->end())
      s += '2';
    (*c1)[s] = i->second;
  }
  isSaved = false;
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

  // detect utf-8
  char* p = (char*)(&buff[0]);
  bool isUnicode = (*p == 0x00);

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
    entry = entry.BeforeFirst('=', &value);
    content[entry] = value;
    s.Remove(0, res + 2);
  }
  content.erase("");
  content.erase("[Main]");

  return true;
}

key_t GenerateKey()
{
  return (((key_t)rand() << 16) | (key_t)rand()) * 2;
}

bool CCryptoFile::WriteFile(bool isUnicode)
{
  // parse to string
  wxString s;
  CContent::iterator it;
  for (it = content.begin(); it != content.end(); ++it)
    s += it->first + "=" + it->second + "\r\n";
  s.Prepend("[Main]\r\n");
  s.Append("\r\n");

  // convert to char buffer
  wxScopedCharBuffer charbuff;
  if (isUnicode)
    charbuff = s.ToUTF8();
  else
    charbuff = s.char_str();
  size_t len = charbuff.length();
  if (isUnicode)
    len++;  // for 1-byte utf-8 mark

  // copy to buffer
  wxArrayLong buff;
  buff.SetCount(len / sizeof(key_t) + 1);
  char* p = (char*)&buff[0];
  if (isUnicode)
  {
    memcpy(p + 1, charbuff, len - 1);
    // utf-8 mark
    *p = 0x00;
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
