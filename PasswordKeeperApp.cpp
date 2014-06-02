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

IMPLEMENT_APP(PasswordKeeperApp);

bool PasswordKeeperApp::OnInit()
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
