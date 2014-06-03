#ifndef PROPDIALOG_H
#define PROPDIALOG_H

//(*Headers(PropDialog)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
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

		const int ShowModalEx(wxString& name, CRecord& rec, const ShowMode mode);

		//(*Declarations(PropDialog)
		wxTextCtrl* edName;
		wxStaticText* StaticText2;
		wxTextCtrl* edEmail;
		wxBoxSizer* BoxSizer;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxTextCtrl* edPass;
		wxStdDialogButtonSizer* dbSizer;
		wxTextCtrl* edLogin;
		wxStaticText* StaticText4;
		//*)

	protected:

		//(*Identifiers(PropDialog)
		static const long ID_STATICTEXT2;
		static const long ID_EDNAME;
		static const long ID_STATICTEXT3;
		static const long ID_EDLOGIN;
		static const long ID_STATICTEXT4;
		static const long ID_EDEMAIL;
		static const long ID_STATICTEXT1;
		static const long ID_EDPASS;
		//*)

	private:

		//(*Handlers(PropDialog)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
