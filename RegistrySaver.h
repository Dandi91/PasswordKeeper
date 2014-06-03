#ifndef REGISTRYSAVER_H
#define REGISTRYSAVER_H

#include <wx/string.h>
#include <wx/msw/registry.h>
#include <wx/toplevel.h>
#include <wx/arrstr.h>

class CRegistrySaver
{
private:
  wxRegKey* key;
public:
  CRegistrySaver(const wxString& path);
  void LoadWindow(wxTopLevelWindow& window, const wxString& name);
  void LoadMRU(wxArrayString& mru, int& active);
  void SaveWindow(wxTopLevelWindow& window, const wxString& name);
  void SaveMRU(const wxArrayString& mru, const int active);
};

#endif // REGISTRYSAVER_H
