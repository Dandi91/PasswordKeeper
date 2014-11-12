#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#if defined(__WXGTK__) || defined(__WXMOTIF__)
  // Ignoring warnings from using C code in C++ code
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wwrite-strings"
  #pragma GCC diagnostic ignored "-Wunused-variable"
  // Application icon
  #include "../icon_p2.xpm"
  // Restore warnings
  #pragma GCC diagnostic pop
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

	  typedef enum {
      dbLogin,
      dbChange,
      dbMerge
	  } DialogBehavior;

		AuthDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~AuthDialog();

    void ChangeBehavior();
    void MergeBehavior();

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
    DialogBehavior behavior;

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
