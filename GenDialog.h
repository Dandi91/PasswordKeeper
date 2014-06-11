#ifndef GENDIALOG_H
#define GENDIALOG_H

//(*Headers(GenDialog)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

#include <wx/string.h>

class GenDialog: public wxDialog
{
	public:

		GenDialog(wxWindow* parent,wxWindowID id=wxID_ANY);
		virtual ~GenDialog();

		const int ShowModalEx(wxString& password);

		//(*Declarations(GenDialog)
		wxFlexGridSizer* FormSizer;
		wxFlexGridSizer* TopSizer;
		wxCheckBox* cbUpper;
		wxCheckBox* cbLower;
		wxTextCtrl* edPassword;
		wxTextCtrl* edMinLength;
		wxCheckBox* cbShow;
		wxTextCtrl* edMaxLength;
		wxCheckBox* cbDigits;
		wxCheckBox* cbSpecials;
		wxButton* btGenerate;
		//*)

	protected:

		//(*Identifiers(GenDialog)
		static const long ID_TEXTCTRL1;
		static const long ID_CHECKBOX1;
		static const long ID_CHECKBOX5;
		static const long ID_CHECKBOX2;
		static const long ID_CHECKBOX3;
		static const long ID_CHECKBOX4;
		static const long ID_TEXTCTRL2;
		static const long ID_TEXTCTRL3;
		static const long ID_BUTTON1;
		//*)

	private:

		//(*Handlers(GenDialog)
		void OnbtGenerateClick(wxCommandEvent& event);
		void OncbShowClick(wxCommandEvent& event);
		void OnInit(wxInitDialogEvent& event);
		void OnedPasswordTextEnter(wxCommandEvent& event);
		//*)

		void GeneratePassword();
		void PasswordVisibility();

		DECLARE_EVENT_TABLE()
};

#endif
