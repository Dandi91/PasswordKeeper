#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

//(*Headers(AuthDialog)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class AuthDialog: public wxDialog
{
	public:

		AuthDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~AuthDialog();

		//(*Declarations(AuthDialog)
		wxStdDialogButtonSizer* sbStdButtons;
		wxFlexGridSizer* fgSizer;
		wxButton* btNew;
		wxBoxSizer* bsButtons;
		wxTextCtrl* edPassword;
		wxStaticText* lbLogin;
		wxTextCtrl* edLogin;
		wxStaticText* lbPassword;
		//*)

		wxTextCtrl* edPasswordSec;
		wxStaticText* newLabel;
		wxButton* btBack;

	protected:

		//(*Identifiers(AuthDialog)
		static const long ID_TEXTCTRL1;
		static const long ID_TEXTCTRL2;
		static const long ID_BUTTON1;
		//*)

		static const long ID_BUTTON2;

	private:

    int editorsSize;

		//(*Handlers(AuthDialog)
		void OnbtNewClick(wxCommandEvent& event);
		void OnEnter(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()

		void OnModalClose(wxCommandEvent& event);
		void OnBack(wxCommandEvent& event);
		bool CheckParams();
};

#endif
