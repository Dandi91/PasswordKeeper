#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#if defined(__WXGTK__) || defined(__WXMOTIF__)
  // Application icon
  #include "icon_p2.xpm"
#endif

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

    void ChangeBehavior();

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
		static const long ID_EDLOGIN;
		static const long ID_EDPASS;
		static const long ID_BTNEW;
		//*)

		static const long ID_BTBACK;

	private:

    int editorsSize;
    bool behavior;

		//(*Handlers(AuthDialog)
		void OnbtNewClick(wxCommandEvent& event);
		void OnEnter(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()

		void OnModalClose(wxCommandEvent& event);
		void OnBack(wxCommandEvent& event);
		bool CheckBeforeClose();
		void ChangeViewNewAccount(const bool state);
};

#endif
