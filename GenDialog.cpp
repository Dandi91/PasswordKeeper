#include "GenDialog.h"

//(*InternalHeaders(GenDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(GenDialog)
//*)

BEGIN_EVENT_TABLE(GenDialog,wxDialog)
	//(*EventTable(GenDialog)
	//*)
END_EVENT_TABLE()

GenDialog::GenDialog(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(GenDialog)
	wxBoxSizer* BoxSizer1;

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(400,154));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	SetSizer(BoxSizer1);
	SetSizer(BoxSizer1);
	Layout();
	//*)
}

GenDialog::~GenDialog()
{
	//(*Destroy(GenDialog)
	//*)
}

