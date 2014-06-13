/***************************************************************
 * Name:      PasswordKeeperApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Dandi ()
 * Created:   2014-05-28
 * Copyright: Dandi ()
 * License:
 **************************************************************/

#include "PasswordKeeperApp.h"

//(*AppHeaders
#include "PasswordKeeperMain.h"
#include <wx/image.h>
//*)

#include "AuthDlg.h"
#include "CryptoFile.h"
#include <wx/msgdlg.h>

IMPLEMENT_APP(PasswordKeeperApp);

bool PasswordKeeperApp::OnInit()
{
  AuthDlg dlg(NULL);
  int res = dlg.ShowModal();
  while ((res == wxID_OK) || (res == ID_CREATENEW))
  {
    CCryptoFile::Get().OpenFile(dlg.edLogin->GetValue(), dlg.edPassword->GetValue(), res == ID_CREATENEW);
    if (CCryptoFile::Get().isOk())
    {
      //(*AppInitialize
      bool wxsOK = true;
      wxInitAllImageHandlers();
      if ( wxsOK )
      {
        PasswordKeeperFrame* Frame = new PasswordKeeperFrame(0);
        Frame->Show();
        SetTopWindow(Frame);
      }
      //*)
      return wxsOK;
    }
    wxMessageBox(CCryptoFile::Get().GetErrorMessage(), "Error");
    res = dlg.ShowModal();
  }
  return false;
}
