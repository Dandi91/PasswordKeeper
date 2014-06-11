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
  ~CRegistrySaver();

  long LoadLong(const wxString& name, const long def);
  bool LoadBool(const wxString& name, const bool def);
  wxString LoadStringW(const wxString& name, const wxString& def);
  void LoadWindow(wxTopLevelWindow& window, const wxString& name);
  void LoadMRU(wxArrayString& mru, int& active);

  void SaveLong(const long value, const wxString& name);
  void SaveBool(const bool value, const wxString& name);
  void SaveStringW(const wxString& value, const wxString& name);
  void SaveWindow(wxTopLevelWindow& window, const wxString& name);
  void SaveMRU(const wxArrayString& mru, const int active);
};

#endif // REGISTRYSAVER_H
