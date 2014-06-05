#include "PropDialog.h"

//(*InternalHeaders(PropDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include <wx/msgdlg.h>

//(*IdInit(PropDialog)
const long PropDialog::ID_EDNAME = wxNewId();
const long PropDialog::ID_EDLOGIN = wxNewId();
const long PropDialog::ID_EDEMAIL = wxNewId();
const long PropDialog::ID_EDPASS = wxNewId();
const long PropDialog::ID_BTPASS = wxNewId();
//*)

BEGIN_EVENT_TABLE(PropDialog,wxDialog)
	//(*EventTable(PropDialog)
	//*)
END_EVENT_TABLE()

PropDialog::PropDialog(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(PropDialog)
	wxStaticText* StaticText2;
	wxStaticText* StaticText1;
	wxStaticText* StaticText3;
	wxBoxSizer* BoxSizer1;
	wxFlexGridSizer* FlexGridSizer1;
	wxStaticText* StaticText4;

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(322,108));
	FormSizer = new wxGridBagSizer(0, 0);
	FlexGridSizer1 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticText1 = new wxStaticText(this, wxID_ANY, _("Name"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	FlexGridSizer1->Add(StaticText1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	edName = new wxTextCtrl(this, ID_EDNAME, _("Text"), wxDefaultPosition, wxSize(321,21), 0, wxDefaultValidator, _T("ID_EDNAME"));
	FlexGridSizer1->Add(edName, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(this, wxID_ANY, _("Login"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	FlexGridSizer1->Add(StaticText2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	edLogin = new wxTextCtrl(this, ID_EDLOGIN, _("Text"), wxDefaultPosition, wxSize(321,21), 0, wxDefaultValidator, _T("ID_EDLOGIN"));
	FlexGridSizer1->Add(edLogin, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText3 = new wxStaticText(this, wxID_ANY, _("Email"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	FlexGridSizer1->Add(StaticText3, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	edEmail = new wxTextCtrl(this, ID_EDEMAIL, _("Text"), wxDefaultPosition, wxSize(321,21), 0, wxDefaultValidator, _T("ID_EDEMAIL"));
	FlexGridSizer1->Add(edEmail, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText4 = new wxStaticText(this, wxID_ANY, _("Password"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	FlexGridSizer1->Add(StaticText4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	edPass = new wxTextCtrl(this, ID_EDPASS, _("Text"), wxDefaultPosition, wxSize(321,21), wxTE_PASSWORD, wxDefaultValidator, _T("ID_EDPASS"));
	FlexGridSizer1->Add(edPass, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FormSizer->Add(FlexGridSizer1, wxGBPosition(0, 0), wxDefaultSpan, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	btGenerate = new wxButton(this, ID_BTPASS, _("Generate password..."), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BTPASS"));
	BoxSizer1->Add(btGenerate, 1, wxALL|wxSHAPED|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	dbSizer = new wxStdDialogButtonSizer();
	dbSizer->Realize();
	BoxSizer1->Add(dbSizer, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	FormSizer->Add(BoxSizer1, wxGBPosition(1, 0), wxDefaultSpan, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(FormSizer);
	SetSizer(FormSizer);
	Layout();
	Center();
	//*)
}

PropDialog::~PropDialog()
{
	//(*Destroy(PropDialog)
	//*)
}

void PropDialog::AddButtonToSizer(const wxWindowID id, const bool isAffirmative)
{
  wxButton* newButton = new wxButton(this, id);
  if (isAffirmative)
    newButton->Bind(wxEVT_BUTTON, (wxObjectEventFunction)&PropDialog::OnModalClose, this);
  dbSizer->AddButton(newButton);
}

const int PropDialog::ShowModalEx(CRecord& rec, const ShowMode mode)
{
  edName->SetValue(rec.name);
  edLogin->SetValue(rec.login);
  edEmail->SetValue(rec.email);
  edPass->SetValue(rec.password);
  if (mode == smVIEW)
  {
    this->SetTitle("View Record");
    edName->SetEditable(false);
    edLogin->SetEditable(false);
    edEmail->SetEditable(false);
    edPass->SetEditable(false);
    SendMessage((HWND)edPass->GetHandle(), EM_SETPASSWORDCHAR, 0, 0);
    AddButtonToSizer(wxID_OK, false);
  }
  if (mode == smADD)
  {
    this->SetTitle("Add Record");
    AddButtonToSizer(wxID_OK, true);
    AddButtonToSizer(wxID_CANCEL, false);
  }
  if (mode == smEDIT)
  {
    this->SetTitle("Edit Record");
    AddButtonToSizer(wxID_OK, true);
    AddButtonToSizer(wxID_CANCEL, false);
  }
  dbSizer->Realize();
  FormSizer->Fit(this);
  int res = ShowModal();
  rec.name = edName->GetValue();
  rec.login = edLogin->GetValue();
  rec.email = edEmail->GetValue();
  rec.password = edPass->GetValue();
  return res;
}

bool PropDialog::CheckParams()
{
  wxString msg;
  if (edName->GetValue().Contains("="))
    msg = "\"Name\"";
  else if (edLogin->GetValue().Contains("="))
    msg = "\"Login\"";
  else if (edEmail->GetValue().Contains("="))
    msg = "\"Email\"";
  if (!msg.IsEmpty())
  {
    msg.Prepend("The field ");
    msg.Append(" is containing an unacceptable character '='");
    wxMessageBox(msg, "Error", wxOK | wxICON_ERROR, this);
    return false;
  }
  else
    return true;
}

void PropDialog::OnModalClose(wxCommandEvent& event)
{
  if (CheckParams())
    event.Skip();
}
