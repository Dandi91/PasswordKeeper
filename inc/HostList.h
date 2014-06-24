#ifndef HOSTLIST_H
#define HOSTLIST_H

#include <vector>
#include <wx/string.h>
#include <wx/filefn.h>

// Class for keeping host address and path to the public key
class CHost
{
private:
  wxString addr;
  wxString keyFullPath;
  wxString serverName;
  bool isInitialized;
public:
  CHost() { isInitialized = false; };
  CHost(const wxString& address, const wxString& keyPath = wxEmptyString, const wxString& name = wxEmptyString)
          { Initialize(address, keyPath, name); };

  bool Initialize(const wxString& address, const wxString& keyPath, const wxString& name)
  {
    addr = address;
    keyFullPath = keyPath;
    serverName = name;
    isInitialized = (name != wxEmptyString) && (wxFileExists(keyPath));
    return isInitialized;
  };

  const wxString& GetAddress() const { return addr; };
  const wxString& GetPublicKeyPath() const { return keyFullPath; };
  const wxString& GetName() const { return serverName; };
  const bool IsInitialized() const { return isInitialized; };
};

class CHostList
{
private:
  std::vector <CHost*> array;
  int selected;
  void Switch(const size_t indexA, const size_t indexB);
public:
  static CHostList& Get();

  // Methods for the whole list
  void Clear();
  void Load();
  void Sort();
  void Store();

  // Elements' methods
  size_t Add(const wxString& address, const wxString& keyPath = wxEmptyString, const wxString& name = wxEmptyString);
  void Delete(const size_t index);
  CHost& GetHost(const size_t index) const { return *(array[index]); };

  // Properties
  size_t GetCount() const { return array.size(); };
  int GetSelected() const { return selected; };
  CHost* GetSelectedHost() { return selected != -1 ? array[selected] : NULL; };
  void SetSelected(const int index) { selected = index; };
protected:
  CHostList() { selected = -1; };
  ~CHostList() { Clear(); };
};

#define CurrentHost (CHostList::Get().GetSelectedHost())

#endif // HOSTLIST_H
