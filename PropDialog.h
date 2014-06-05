#ifndef PROPDIALOG_H
#define PROPDIALOG_H

//(*Headers(PropDialog)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/gbsizer.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

#include <wx/string.h>
#include "CryptoFile.h"

typedef enum
{
  smVIEW = 0,
  smADD,
  smEDIT
} ShowMode;

class PropDialog: public wxDialog
{
	public:

		PropDialog(wxWindow* parent,wxWindowID id=wxID_ANY);
		virtual ~PropDialog();

		const int ShowModalEx(CRecord& rec, const ShowMode mode);

		//(*Declarations(PropDialog)
		wxTextCtrl* edName;
		wxTextCtrl* edEmail;
		wxGridBagSizer* FormSizer;
		wxTextCtrl* edPass;
		wxStdDialogButtonSizer* dbSizer;
		wxTextCtrl* edLogin;
		wxButton* btGenerate;
		//*)

	protected:

		//(*Identifiers(PropDialog)
		static const long ID_EDNAME;
		static const long ID_EDLOGIN;
		static const long ID_EDEMAIL;
		static const long ID_EDPASS;
		static const long ID_BTPASS;
		//*)

	private:
		void OnModalClose(wxCommandEvent& event);
    bool CheckParams();
    void AddButtonToSizer(const wxWindowID id, const bool isAffirmative);

		DECLARE_EVENT_TABLE()
};

#endif
