#include "PropDialog.h"

//(*InternalHeaders(PropDialog)
#include <wx/intl.h>
#include <wx/button.h>
#include <wx/string.h>
//*)

//(*IdInit(PropDialog)
const long PropDialog::ID_STATICTEXT2 = wxNewId();
const long PropDialog::ID_EDNAME = wxNewId();
const long PropDialog::ID_STATICTEXT3 = wxNewId();
const long PropDialog::ID_EDLOGIN = wxNewId();
const long PropDialog::ID_STATICTEXT4 = wxNewId();
const long PropDialog::ID_EDEMAIL = wxNewId();
const long PropDialog::ID_STATICTEXT1 = wxNewId();
const long PropDialog::ID_EDPASS = wxNewId();
//*)

BEGIN_EVENT_TABLE(PropDialog,wxDialog)
	//(*EventTable(PropDialog)
	//*)
END_EVENT_TABLE()

PropDialog::PropDialog(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(PropDialog)
	wxFlexGridSizer* FlexGridSizer1;

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	BoxSizer = new wxBoxSizer(wxHORIZONTAL);
	FlexGridSizer1 = new wxFlexGridSizer(5, 2, 0, 0);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Name"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer1->Add(StaticText2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	edName = new wxTextCtrl(this, ID_EDNAME, _("Text"), wxDefaultPosition, wxSize(318,21), 0, wxDefaultValidator, _T("ID_EDNAME"));
	FlexGridSizer1->Add(edName, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Login"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer1->Add(StaticText3, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	edLogin = new wxTextCtrl(this, ID_EDLOGIN, _("Text"), wxDefaultPosition, wxSize(318,21), 0, wxDefaultValidator, _T("ID_EDLOGIN"));
	FlexGridSizer1->Add(edLogin, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Email"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer1->Add(StaticText4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	edEmail = new wxTextCtrl(this, ID_EDEMAIL, _("Text"), wxDefaultPosition, wxSize(318,21), 0, wxDefaultValidator, _T("ID_EDEMAIL"));
	FlexGridSizer1->Add(edEmail, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Password"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer1->Add(StaticText1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	edPass = new wxTextCtrl(this, ID_EDPASS, _("Text"), wxDefaultPosition, wxSize(318,21), wxTE_PASSWORD, wxDefaultValidator, _T("ID_EDPASS"));
	FlexGridSizer1->Add(edPass, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(-1,-1,1, wxALL|wxFIXED_MINSIZE|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	dbSizer = new wxStdDialogButtonSizer();
	dbSizer->Realize();
	FlexGridSizer1->Add(dbSizer, 1, wxALL|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer->Add(FlexGridSizer1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 8);
	SetSizer(BoxSizer);
	BoxSizer->Fit(this);
	BoxSizer->SetSizeHints(this);
	Center();
	//*)
}

PropDialog::~PropDialog()
{
	//(*Destroy(PropDialog)
	//*)
}

const int PropDialog::ShowModalEx(wxString& name, CRecord& rec, const ShowMode mode)
{
  if (mode == smVIEW)
  {
    this->SetTitle("View Record");
    edName->SetEditable(false);
    edName->SetValue(name);
    edLogin->SetEditable(false);
    edLogin->SetValue(rec.login);
    edEmail->SetEditable(false);
    edEmail->SetValue(rec.email);
    edPass->SetEditable(false);
    SendMessage((HWND)edPass->GetHandle(), EM_SETPASSWORDCHAR, 0, 0);
    edPass->SetValue(rec.password);
    dbSizer->AddButton(new wxButton(this, wxID_OK));
  }
  if (mode == smADD)
  {
    this->SetTitle("Add Record");
    edName->SetValue(name);
    edLogin->SetValue(rec.login);
    edEmail->SetValue(rec.email);
    edPass->SetValue(rec.password);
    dbSizer->AddButton(new wxButton(this, wxID_OK));
    dbSizer->AddButton(new wxButton(this, wxID_CANCEL));
  }
  if (mode == smEDIT)
  {
    this->SetTitle("Edit Record");
    edName->SetValue(name);
    edLogin->SetValue(rec.login);
    edEmail->SetValue(rec.email);
    edPass->SetValue(rec.password);
    dbSizer->AddButton(new wxButton(this, wxID_OK));
    dbSizer->AddButton(new wxButton(this, wxID_CANCEL));
  }
  dbSizer->Realize();
  BoxSizer->Fit(this);
  int res = ShowModal();
  name = edName->GetValue();
  rec.login = edLogin->GetValue();
  rec.email = edEmail->GetValue();
  rec.password = edPass->GetValue();
  return res;
}
