#ifndef HOSTLIST_H
#define HOSTLIST_H

#define NET_DEFAULT_PORT  35100

#include <vector>
#include <wx/string.h>
#include <wx/filefn.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>

// Class for keeping host address and path to the public key
class CHost
{
private:
  wxString addr;
  unsigned short port;
  wxString keyFullPath;
  bool isInitialized;
public:
  CHost() { isInitialized = false; port = NET_DEFAULT_PORT; };
  CHost(const wxString& address, const unsigned short service, const wxString& keyPath = wxEmptyString)
          { Initialize(address, service, keyPath); };

  bool Initialize(const wxString& address, const unsigned short service, const wxString& keyPath)
  {
    addr = address;
    port = service;
    keyFullPath = keyPath;
    isInitialized = (address != wxEmptyString) && (service) && (wxFileExists(keyPath));
    return isInitialized;
  };

  const wxString GetAddress() const { return addr; };
  unsigned short GetPort() const { return port; };
  const wxString GetPublicKeyPath() const { return keyFullPath; };
  const bool IsInitialized() const { return isInitialized; };

  const wxString ConstructNewKeyPath() const
  {
    if (addr.IsEmpty())
      return wxEmptyString;
    wxFileName file(wxStandardPaths::Get().GetUserDataDir(),  // path
                    addr /* name */, "der" /* extension */);
    return file.GetFullPath();
  }

  // Operations with addresses in <address>:<port> form
  // Extract port
  static unsigned short GetPortFromString(const wxString& string)
  {
    unsigned long retVal;
    string.AfterLast(':').ToULong(&retVal);
    return retVal;
  };
  // Extract address
  static wxString GetAddressFromString(const wxString& string)
  {
    return string.BeforeLast(':');
  };
  // Combine address and port
  static wxString GetStringFromAddressAndPort(const wxString& address, const unsigned short port)
  {
    wxString retVal = address;
    retVal << ':' << port;
    return retVal;
  };
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
  size_t Add(const wxString& address, const unsigned short service, const wxString& keyPath);
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
