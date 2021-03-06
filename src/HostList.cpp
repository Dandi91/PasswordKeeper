#include "HostList.h"
#include <wx/fileconf.h>

CHostList& CHostList::Get()
{
  static CHostList list;
  return list;
}

void CHostList::Clear()
{
  for (size_t i = 0; i < array.size(); ++i)
    delete array[i];
  array.resize(0);
}

void CHostList::Load()
{
  Clear();
  wxFileConfig cfg(wxEmptyString, wxEmptyString,
                      #if defined(__WXMSW__)
                        "hosts.ini",
                      #elif defined(__UNIX__)
                        "hosts",
                      #endif
                      wxEmptyString, wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_SUBDIR);
  wxString group;
  long index = 0;
  cfg.GetFirstGroup(group, index);
  do
  {
    wxString address = group, path;
    long port;
    cfg.SetPath(group.Prepend('/'));
    cfg.Read("Port", &port);
    cfg.Read("KeyFile", &path);
    Add(address, port, path);
  }
  while (cfg.GetNextGroup(group, index));
}

void CHostList::Store()
{
  wxFileConfig cfg(wxEmptyString, wxEmptyString,
                      #if defined(__WXMSW__)
                        "hosts.ini",
                      #elif defined(__UNIX__)
                        "hosts",
                      #endif
                      wxEmptyString, wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_SUBDIR);
  wxString group;
  long index = 0;
  cfg.GetFirstGroup(group, index);
  do
    cfg.DeleteGroup(group);
  while (cfg.GetNextGroup(group, index));
  for (size_t i = 0; i < array.size(); ++i)
  {
    CHost* host = &GetHost(i);
    if (!host->IsInitialized())
      continue;
    group = host->GetAddress();
    cfg.SetPath(group.Prepend('/'));
    cfg.Write("Port", host->GetPort());
    cfg.Write("KeyFile", host->GetPublicKeyPath());
  }
}

void CHostList::Sort()
{
  if (array.size() < 2)
    return;
  for (size_t i = 0; i < array.size() - 1; ++i)
  {
    size_t max = i;
    for (size_t j = i + 1; j < array.size(); ++j)
      if (array[max]->GetAddress().CmpNoCase(array[j]->GetAddress()) > 0)
        max = j;
    Switch(i, max);
  }
}

size_t CHostList::Add(const wxString& address, const unsigned short service, const wxString& keyPath = wxEmptyString)
{
  array.push_back(new CHost(address, service, keyPath));
  return array.size() - 1;
}

void CHostList::Delete(const size_t index)
{
  delete array[index];
  array.erase(array.begin() + index);
}

void CHostList::Switch(const size_t indexA, const size_t indexB)
{
  if (indexA != indexB)
  {
    CHost* temp = array[indexA];
    array[indexA] = array[indexB];
    array[indexB] = temp;
  }
}
