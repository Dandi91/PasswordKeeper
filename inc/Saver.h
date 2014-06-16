#ifndef SAVER_H
#define SAVER_H

#include <wx/string.h>
#include <wx/fileconf.h>
#include <wx/toplevel.h>

class CSaver : public wxFileConfig
{
public:
  // Static getter for single-instance use
  static CSaver& Get();

  void LoadWindow(wxTopLevelWindow& window, const wxString& name);
  void SaveWindow(const wxTopLevelWindow& window, const wxString& name);

  wxString ReadEx(const wxString& key, const wxString& defValue) const;
      bool ReadEx(const wxString& key, const bool defValue) const;
       int ReadEx(const wxString& key, const int defValue) const;

protected:
  CSaver() : wxFileConfig(wxEmptyString, wxEmptyString,
                      #if defined(__WXMSW__)
                        "settings.ini",
                      #elif defined(__UNIX__)
                        "settings",
                      #endif
                      wxEmptyString, wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_SUBDIR) {};
};

#endif // SAVER_H
