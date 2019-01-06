#include "AuthDialog.h"
#include "Saver.h"

//(*InternalHeaders(AuthDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(AuthDialog)
const long AuthDialog::ID_EDLOGIN = wxNewId();
const long AuthDialog::ID_EDPASS = wxNewId();
const long AuthDialog::ID_BTNEW = wxNewId();
//*)

const long AuthDialog::ID_BTBACK = wxNewId();
const long AuthDialog::ID_BTRELOCATE = wxNewId();

#include <wx/msgdlg.h>
#include <wx/dirdlg.h>
#include <wx/stdpaths.h>

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
	edLogin = new wxTextCtrl(this, ID_EDLOGIN, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, wxDefaultValidator, _T("ID_EDLOGIN"));
	fgSizer->Add(edLogin, 1, wxALL|wxEXPAND, 5);
	lbPassword = new wxStaticText(this, wxID_ANY, _("Password"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	fgSizer->Add(lbPassword, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	edPassword = new wxTextCtrl(this, ID_EDPASS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_PASSWORD, wxDefaultValidator, _T("ID_EDPASS"));
	fgSizer->Add(edPassword, 1, wxALL|wxEXPAND, 5);
	fgSizer->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bsButtons = new wxBoxSizer(wxHORIZONTAL);
	btNew = new wxButton(this, ID_BTNEW, _("New account..."), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BTNEW"));
	bsButtons->Add(btNew, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	sbStdButtons = new wxStdDialogButtonSizer();
	sbStdButtons->Realize();
	bsButtons->Add(sbStdButtons, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	fgSizer->Add(bsButtons, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(fgSizer, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	SetSizer(BoxSizer1);
	Layout();
	Center();

	Connect(ID_EDLOGIN,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&AuthDialog::OnEnter);
	Connect(ID_EDPASS,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&AuthDialog::OnEnter);
	Connect(ID_BTNEW,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AuthDialog::OnbtNewClick);
	//*)


  btRelocate = new wxButton(this, ID_BTRELOCATE, "Relocate...", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BTRELOCATE"));
  btRelocate->Bind(wxEVT_BUTTON, (wxObjectEventFunction)&AuthDialog::OnRelocate, this);
  bsButtons->Insert(0, btRelocate, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

  wxButton* newButton = new wxButton(this, wxID_OK);
  newButton->Bind(wxEVT_BUTTON, (wxObjectEventFunction)&AuthDialog::OnModalClose, this);
  sbStdButtons->AddButton(newButton);
  sbStdButtons->AddButton(new wxButton(this, wxID_CANCEL));
  sbStdButtons->Realize();
  fgSizer->Fit(this);

  wxSize windowSize = edLogin->GetSize();
  editorsSize = windowSize.GetWidth();

  edPasswordSec = NULL;
  behavior = dbLogin;

  wxString accountName;
  CSaver::Get().Read("Account", &accountName);
  edLogin->SetValue(accountName);

#if defined(__WXGTK__) || defined(__WXMOTIF__)
  // Single icon from xpm for X
  wxIcon appIcon(icon_p2);
  SetIcon(appIcon);
#endif // (__WXGTK__) || (__WXMOTIF__)
}

AuthDialog::~AuthDialog()
{
	//(*Destroy(AuthDialog)
	//*)
}

bool AuthDialog::CheckBeforeClose()
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
  if (behavior == dbLogin)
    CSaver::Get().Write("Account", edLogin->GetValue());
  return true;
}

void AuthDialog::OnModalClose(wxCommandEvent& event)
{
  if (CheckBeforeClose())
    event.Skip();
}

void AuthDialog::ChangeViewNewAccount(const bool state)
{
  // Labels
  SetLabel(state ? "Create New Account" : "Authorization");
  lbLogin->SetLabel(state ? "New login" : "Login");
  lbPassword->SetLabel(state ? "New password" : "Password");
  btNew->SetLabel(state ? "Create account" : "New account...");
  if (state)
  {
    // Create password repeat field and label
    wxSize size = wxDefaultSize;
    size.SetWidth(editorsSize);
    edPasswordSec = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, size, wxTE_PROCESS_ENTER | wxTE_PASSWORD, wxDefaultValidator, "wxID_ANY");
    newLabel = new wxStaticText(this, wxID_ANY, "Repeat password", wxDefaultPosition, wxDefaultSize, 0, "wxID_ANY");
    fgSizer->Insert(4, newLabel, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    fgSizer->Insert(5, edPasswordSec, 1, wxALL|wxEXPAND, 5);
    // Hide standard buttons
    bsButtons->Hide(sbStdButtons);
    bsButtons->Detach(sbStdButtons);
    // Add "Back" button
    btBack = new wxButton(this, ID_BTBACK, "Back", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, "ID_BTBACK");
    btBack->Bind(wxEVT_BUTTON, (wxObjectEventFunction)&AuthDialog::OnBack, this);
    bsButtons->Add(btBack, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    // Resize
    fgSizer->Fit(this);
    fgSizer->SetSizeHints(this);
    // Tab order / focus
    edPasswordSec->MoveAfterInTabOrder(edPassword);
    edLogin->SetFocus();
    // Event binding
    edPasswordSec->Bind(wxEVT_TEXT_ENTER, (wxObjectEventFunction)&AuthDialog::OnEnter, this);
  }
  else
  {
    // Delete password repeat field and label
    fgSizer->Detach(edPasswordSec);
    fgSizer->Detach(newLabel);
    RemoveChild(edPasswordSec);
    RemoveChild(newLabel);
    wxDELETE(edPasswordSec);
    wxDELETE(newLabel);
    // Show standard buttons
    bsButtons->Add(sbStdButtons, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    bsButtons->Show(sbStdButtons);
    // Remove "Back" button
    bsButtons->Detach(btBack);
    RemoveChild(btBack);
    wxDELETE(btBack);
    // Sizers
    fgSizer->Fit(this);
    fgSizer->SetSizeHints(this);
  }
}

void AuthDialog::ChangeBehavior()
{
  ChangeViewNewAccount(true);
  SetLabel("Change Account Settings");
  btBack->SetLabel("Cancel");
  behavior = dbChange;
}

void AuthDialog::MergeBehavior(const wxString& fileName)
{
  SetLabel("Account Settings for \"" + fileName + "\"");
  edLogin->Clear();
  btNew->Hide();
  btRelocate->Hide();
  fgSizer->Fit(this);
  fgSizer->SetSizeHints(this);
  behavior = dbMerge;
}

void AuthDialog::OnBack(wxCommandEvent& event)
{
  switch (behavior)
  {
    case dbChange:
    case dbMerge:
      EndModal(wxID_CANCEL);
      break;
    case dbLogin:
      ChangeViewNewAccount(false);
      break;
  }
}

void AuthDialog::OnRelocate(wxCommandEvent& event)
{
  wxDirDialog dirDialog(this, "Select directory with accounts", CSaver::Get().Read("AccountDirectory", wxStandardPaths::Get().GetUserDataDir()));
  if (dirDialog.ShowModal() == wxID_OK)
  {
    wxString dir = dirDialog.GetPath();
    if (wxDirExists(dir))
    {
      CSaver::Get().Write("AccountDirectory", dir);
      CSaver::Get().Flush();
    }
    else
      wxMessageBox("Selected directory does not exist. Return to the default setting.", "Error", wxOK | wxICON_ERROR);
  }
}

void AuthDialog::OnbtNewClick(wxCommandEvent& event)
{
  if (!edPasswordSec)
    ChangeViewNewAccount(true);
  else
    if (CheckBeforeClose())
      EndModal(wxID_NEW);
}

void AuthDialog::OnEnter(wxCommandEvent& event)
{
  if (CheckBeforeClose())
    EndModal(edPasswordSec ? wxID_NEW : wxID_OK);
}
