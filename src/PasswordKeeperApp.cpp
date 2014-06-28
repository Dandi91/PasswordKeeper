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

#include "AuthDialog.h"
#include "Account.h"
#include "Saver.h"
#include <wx/msgdlg.h>

IMPLEMENT_APP(PasswordKeeperApp);

bool PasswordKeeperApp::OnInit()
{
  CSaver::EnsureDirExistance();
  AuthDialog dlg(NULL);
  int res;
  do
  {
    res = dlg.ShowModal();
    if ((res == wxID_OK) || (res == wxID_NEW))
    {
      CAccount::Get().Authorize(dlg.edLogin->GetValue(), dlg.edPassword->GetValue(), res == wxID_NEW);
      if (CAccount::Get().IsOk())
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
      else
        wxMessageBox(CAccount::Get().GetErrorMessage(), "Error", wxOK | wxICON_ERROR);
    }
  }
  while ((res == wxID_OK) || (res == wxID_NEW));
  return false;
}
