#include "RegistrySaver.h"

CRegistrySaver::CRegistrySaver(const wxString& path)
{
  key = new wxRegKey(wxRegKey::HKCU, "Software\\" + path);
  if (!key->Exists())
    key->Create();
  key->Open();
}

void CRegistrySaver::LoadWindow(wxTopLevelWindow& window, const wxString& name)
{
  long top, left, width, height, maximized = 0;
  wxSize windowSize = window.GetSize();
  wxPoint windowPos = window.GetPosition();
  top = windowPos.x;
  left = windowPos.y;
  width = windowSize.GetWidth();
  height = windowSize.GetHeight();
  if (key->HasValue(name + "_to"))
  {
    key->QueryValue(name + "_to", &top);
    key->QueryValue(name + "_le", &left);
    key->QueryValue(name + "_wi", &width);
    key->QueryValue(name + "_he", &height);
    key->QueryValue(name + "_ma", &maximized);
  }
  window.SetSize(top, left, width, height);
  if (maximized)
    window.Maximize(true);
}

void CRegistrySaver::LoadMRU(wxArrayString& mru, int& active)
{
  if (key->HasValue("MRUCount"))
  {
    long mruCount = 0, i = 0, mruActive = -1;
    key->QueryValue("MRUActive", &mruActive);
    active = mruActive;
    key->QueryValue("MRUCount", &mruCount);
    while (i < mruCount)
    {
      wxString value, name = "MRU";
      name << i++;
      key->QueryValue(name, value);
      mru.Add(value);
    }
  }
}

void CRegistrySaver::SaveWindow(wxTopLevelWindow& window, const wxString& name)
{
  long top, left, width, height, maximized = 0;
  if (window.IsMaximized())
  {
    maximized = 1;
    window.Maximize(false);
  }
  wxSize windowSize = window.GetSize();
  wxPoint windowPos = window.GetPosition();
  top = windowPos.x;
  left = windowPos.y;
  width = windowSize.GetWidth();
  height = windowSize.GetHeight();
  key->SetValue(name + "_to", top);
  key->SetValue(name + "_le", left);
  key->SetValue(name + "_wi", width);
  key->SetValue(name + "_he", height);
  key->SetValue(name + "_ma", maximized);
}

void CRegistrySaver::SaveMRU(const wxArrayString& mru, const int active)
{
  size_t i = 0;
  key->SetValue("MRUActive", active);
  key->SetValue("MRUCount", mru.Count());
  while (i < mru.Count())
  {
    wxString name = "MRU";
    name << i;
    key->SetValue(name, mru[i++]);
  }
}
