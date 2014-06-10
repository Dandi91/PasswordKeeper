#ifndef GENDIALOG_H
#define GENDIALOG_H

//(*Headers(GenDialog)
#include <wx/sizer.h>
#include <wx/dialog.h>
//*)

class GenDialog: public wxDialog
{
	public:

		GenDialog(wxWindow* parent,wxWindowID id=wxID_ANY);
		virtual ~GenDialog();

		//(*Declarations(GenDialog)
		//*)

	protected:

		//(*Identifiers(GenDialog)
		//*)

	private:

		//(*Handlers(GenDialog)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
