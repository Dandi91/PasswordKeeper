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

#include "Content.h"

class PropDialog: public wxDialog
{
	public:

    enum ShowMode
    {
      smVIEW = 0,
      smADD,
      smEDIT
    };

		PropDialog(wxWindow* parent,wxWindowID id=wxID_ANY);
		virtual ~PropDialog();

		const int ShowModalEx(CRecord& rec, const ShowMode mode);

		//(*Declarations(PropDialog)
		wxTextCtrl* edName;
		wxTextCtrl* edEmail;
		wxFlexGridSizer* fgSizer;
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

    //(*Handlers(PropDialog)
    void OnbtGenerateClick(wxCommandEvent& event);
    //*)

		DECLARE_EVENT_TABLE()

    void AddButtonToSizer(const wxWindowID id, const bool isAffirmative);
};

#endif
