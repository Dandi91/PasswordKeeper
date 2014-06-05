#ifndef CRYPTOFILE_H
#define CRYPTOFILE_H

#include <wx/string.h>
#include <deque>

const static char CF_UNICODE    = 0x01;
const static char CF_NEWFORMAT  = 0x02;

/*
WX_DECLARE_LIST(CRecord, CContent);

#include <wx/listimpl.cpp>
WX_DEFINE_LIST(CContent);

static int wxCMPFUNC_CONV ContentSort(const void* first, const void* second)
{
  CContent::iterator *it1=(CContent::iterator *)first;
  CContent::iterator *it2=(CContent::iterator *)second;

  CRecord *item1=**it1;
  CRecord *item2=**it2;

  return item1->name.CmpNoCase(item2->name);
}*/

typedef std::deque<CRecord> CContent;

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
  ~CCryptoFile();

  void MergeWith(const CCryptoFile& second);
  bool ReadFile();
  bool WriteFile(const bool isUnicode, const bool isNewFormat);

 // void SortContent() { content->Sort(ContentSort); }
  const wxString& GetFileName() { return fname; }
  const wxString& GetFilePath() { return fpath; }

  void SetFilePath(const wxString& path);
};

#endif // CRYPTOFILE_H
