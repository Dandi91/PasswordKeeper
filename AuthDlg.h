#ifndef AUTHDLG_H
#define AUTHDLG_H

//(*Headers(AuthDlg)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

const int ID_CREATENEW = wxNewId();

class AuthDlg: public wxDialog
{
	public:

		AuthDlg(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~AuthDlg();

		//(*Declarations(AuthDlg)
		wxButton* btNew;
		wxTextCtrl* edPassword;
		wxTextCtrl* edLogin;
		//*)

	protected:

		//(*Identifiers(AuthDlg)
		static const long ID_TEXTCTRL1;
		static const long ID_TEXTCTRL2;
		static const long ID_BUTTON1;
		//*)

	private:

		//(*Handlers(AuthDlg)
		void OnbtNewClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
