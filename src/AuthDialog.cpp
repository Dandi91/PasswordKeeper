#include "AuthDialog.h"

//(*InternalHeaders(AuthDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(AuthDialog)
const long AuthDialog::ID_TEXTCTRL1 = wxNewId();
const long AuthDialog::ID_TEXTCTRL2 = wxNewId();
const long AuthDialog::ID_BUTTON1 = wxNewId();
//*)

const long AuthDialog::ID_BUTTON2 = wxNewId();

#include <wx/msgdlg.h>

BEGIN_EVENT_TABLE(AuthDialog,wxDialog)
	//(*EventTable(AuthDialog)
	//*)
END_EVENT_TABLE()

AuthDialog::AuthDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(AuthDialog)
	wxBoxSizer* BoxSizer1;

	Create(parent, id, _("Authorization"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(408,263));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	fgSizer = new wxFlexGridSizer(0, 2, 0, 0);
	lbLogin = new wxStaticText(this, wxID_ANY, _("Login"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	fgSizer->Add(lbLogin, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	edLogin = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	fgSizer->Add(edLogin, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	lbPassword = new wxStaticText(this, wxID_ANY, _("Password"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	fgSizer->Add(lbPassword, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	edPassword = new wxTextCtrl(this, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_PASSWORD, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	fgSizer->Add(edPassword, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	fgSizer->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bsButtons = new wxBoxSizer(wxHORIZONTAL);
	btNew = new wxButton(this, ID_BUTTON1, _("New account..."), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	bsButtons->Add(btNew, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	sbStdButtons = new wxStdDialogButtonSizer();
	sbStdButtons->Realize();
	bsButtons->Add(sbStdButtons, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	fgSizer->Add(bsButtons, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(fgSizer, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	SetSizer(BoxSizer1);
	Layout();
	Center();

	Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&AuthDialog::OnEnter);
	Connect(ID_TEXTCTRL2,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&AuthDialog::OnEnter);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AuthDialog::OnbtNewClick);
	//*)

  wxButton* newButton = new wxButton(this, wxID_OK);
  newButton->Bind(wxEVT_BUTTON, (wxObjectEventFunction)&AuthDialog::OnModalClose, this);
  sbStdButtons->AddButton(newButton);
  sbStdButtons->AddButton(new wxButton(this, wxID_CANCEL));
  sbStdButtons->Realize();
  fgSizer->Fit(this);

  wxSize windowSize = edLogin->GetSize();
  editorsSize = windowSize.GetWidth();
	edPasswordSec = NULL;
}

AuthDialog::~AuthDialog()
{
	//(*Destroy(AuthDialog)
	//*)
}

bool AuthDialog::CheckParams()
{
  if (edLogin->GetValue() == "")
  {
    wxMessageBox("Login can not be empty", "Error", wxOK | wxICON_ERROR);
    return false;
  }
  if (edPassword->GetValue() == "")
  {
    wxMessageBox("Password can not be empty", "Error", wxOK | wxICON_ERROR);
    return false;
  }
  if (edPasswordSec)
    if (edPassword->GetValue() != edPasswordSec->GetValue())
    {
      wxMessageBox("Passwords are not equal", "Error", wxOK | wxICON_ERROR);
      return false;
    }
  return true;
}

void AuthDialog::OnModalClose(wxCommandEvent& event)
{
  if (CheckParams())
    event.Skip();
}

void AuthDialog::OnBack(wxCommandEvent& event)
{
  // Labels
  SetLabel("Authorization");
  lbLogin->SetLabel("Login");
  lbPassword->SetLabel("Password");
  btNew->SetLabel("New account...");
  // Password repeat field
  fgSizer->Detach(edPasswordSec);
  fgSizer->Detach(newLabel);
  RemoveChild(edPasswordSec);
  RemoveChild(newLabel);
  wxDELETE(edPasswordSec);
  wxDELETE(newLabel);
  // Show standart buttons
  bsButtons->Add(sbStdButtons, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  bsButtons->Show(sbStdButtons);
  // Remove "Back" button
  bsButtons->Detach(btBack);
  RemoveChild(btBack);
  wxDELETE(btBack);
  // Sizers
  fgSizer->Fit(this);
  fgSizer->SetSizeHints(this);
  // Bindings
  edLogin->Unbind(wxEVT_TEXT_ENTER, (wxObjectEventFunction)&AuthDialog::OnbtNewClick, this);
  edLogin->Bind(wxEVT_TEXT_ENTER, (wxObjectEventFunction)&AuthDialog::OnEnter, this);
  edPassword->Unbind(wxEVT_TEXT_ENTER, (wxObjectEventFunction)&AuthDialog::OnbtNewClick, this);
  edPassword->Bind(wxEVT_TEXT_ENTER, (wxObjectEventFunction)&AuthDialog::OnEnter, this);
}

void AuthDialog::OnbtNewClick(wxCommandEvent& event)
{
  if (!edPasswordSec)
  {
    // Labels
    SetLabel("Create New Account");
    lbLogin->SetLabel("New login");
    lbPassword->SetLabel("New password");
    btNew->SetLabel("Create account");
    // Password repeat field
    wxSize size = wxDefaultSize;
    size.SetWidth(editorsSize);
    edPasswordSec = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, size, wxTE_PROCESS_ENTER | wxTE_PASSWORD, wxDefaultValidator, "wxID_ANY");
    newLabel = new wxStaticText(this, wxID_ANY, "Repeat password", wxDefaultPosition, wxDefaultSize, 0, "wxID_ANY");
    fgSizer->Insert(4, newLabel, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    fgSizer->Insert(5, edPasswordSec, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    // Hide standart buttons
    bsButtons->Hide(sbStdButtons);
    bsButtons->Detach(sbStdButtons);
    // Add "Back" button
    btBack = new wxButton(this, ID_BUTTON2, "Back", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "ID_BUTTON2");
    btBack->Bind(wxEVT_BUTTON, (wxObjectEventFunction)&AuthDialog::OnBack, this);
    bsButtons->Add(btBack, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    // Sizers
    fgSizer->Fit(this);
    fgSizer->SetSizeHints(this);
    // Tab order / focus
    edPasswordSec->MoveAfterInTabOrder(edPassword);
    edLogin->SetFocus();
    // Event binding
    edPasswordSec->Bind(wxEVT_TEXT_ENTER, (wxObjectEventFunction)&AuthDialog::OnbtNewClick, this);
    edLogin->Unbind(wxEVT_TEXT_ENTER, (wxObjectEventFunction)&AuthDialog::OnEnter, this);
    edLogin->Bind(wxEVT_TEXT_ENTER, (wxObjectEventFunction)&AuthDialog::OnbtNewClick, this);
    edPassword->Unbind(wxEVT_TEXT_ENTER, (wxObjectEventFunction)&AuthDialog::OnEnter, this);
    edPassword->Bind(wxEVT_TEXT_ENTER, (wxObjectEventFunction)&AuthDialog::OnbtNewClick, this);
  }
  else
  {
    if (CheckParams())
      EndModal(wxID_NEW);
  }
}

void AuthDialog::OnEnter(wxCommandEvent& event)
{
  EndModal(wxID_OK);
}
