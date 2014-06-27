#include "Saver.h"

CSaver& CSaver::Get()
{
  static CSaver saver;
  saver.SetPath("//PasswordKeeper");
  return saver;
}

void CSaver::LoadWindow(wxTopLevelWindow& window, const wxString& name)
{
  int top, left, width, height;
  bool maximized = window.IsMaximized();
  // Current settings
  wxPoint windowPos = window.GetPosition();
  wxSize windowSize = window.GetSize();
  left = windowPos.x;
  top = windowPos.y;
  width = windowSize.GetWidth();
  height = windowSize.GetHeight();
  // Reading
  SetPath("//" + name);
  Read("Top", &top);
  Read("Left", &left);
  Read("Width", &width);
  Read("Height", &height);
  Read("IsMaximized", &maximized);
  // Set new sizes
  window.SetSize(left, top, width, height);
  window.Maximize(maximized);
}

void CSaver::SaveWindow(const wxTopLevelWindow& window, const wxString& name)
{
  int top, left, width, height;
  bool maximized = window.IsMaximized();
  // Get values
  wxPoint windowPos = window.GetPosition();
  wxSize windowSize = window.GetSize();
  left = windowPos.x;
  top = windowPos.y;
  width = windowSize.GetWidth();
  height = windowSize.GetHeight();
  // Writing
  SetPath("//" + name);
  Write("Top", top);
  Write("Left", left);
  Write("Width", width);
  Write("Height", height);
  Write("IsMaximized", maximized);
}

wxString CSaver::ReadEx(const wxString& key, const wxString& defValue) const
{
  wxString retVal = defValue;
  Read(key, &retVal);
  return retVal;
}

bool CSaver::ReadEx(const wxString& key, const bool defValue) const
{
  bool retVal = defValue;
  Read(key, &retVal);
  return retVal;
}

int CSaver::ReadEx(const wxString& key, const int defValue) const
{
  int retVal = defValue;
  Read(key, &retVal);
  return retVal;
}
