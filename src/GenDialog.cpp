#include "GenDialog.h"

//(*InternalHeaders(GenDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include <wx/msgdlg.h>
#include <vector>

#include "RegistrySaver.h"

//(*IdInit(GenDialog)
const long GenDialog::ID_TEXTCTRL1 = wxNewId();
const long GenDialog::ID_CHECKBOX1 = wxNewId();
const long GenDialog::ID_CHECKBOX5 = wxNewId();
const long GenDialog::ID_CHECKBOX2 = wxNewId();
const long GenDialog::ID_CHECKBOX3 = wxNewId();
const long GenDialog::ID_CHECKBOX4 = wxNewId();
const long GenDialog::ID_TEXTCTRL2 = wxNewId();
const long GenDialog::ID_TEXTCTRL3 = wxNewId();
const long GenDialog::ID_BUTTON1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(GenDialog,wxDialog)
	//(*EventTable(GenDialog)
	//*)
END_EVENT_TABLE()

GenDialog::GenDialog(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(GenDialog)
	wxStaticBoxSizer* StaticBoxSizer2;
	wxFlexGridSizer* FlexGridSizer4;
	wxStaticText* StaticText2;
	wxFlexGridSizer* FlexGridSizer3;
	wxFlexGridSizer* FlexGridSizer5;
	wxStaticText* StaticText1;
	wxStaticText* StaticText3;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxStdDialogButtonSizer* StdDialogButtonSizer1;

	Create(parent, id, _("Generate Password"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(400,154));
	FormSizer = new wxFlexGridSizer(0, 1, 0, 0);
	TopSizer = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText1 = new wxStaticText(this, wxID_ANY, _("Password"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	TopSizer->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	edPassword = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxSize(272,21), wxTE_PROCESS_ENTER|wxTE_PASSWORD, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	TopSizer->Add(edPassword, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	cbShow = new wxCheckBox(this, ID_CHECKBOX1, _("Show"), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE, wxDefaultValidator, _T("ID_CHECKBOX1"));
	cbShow->SetValue(false);
	TopSizer->Add(cbShow, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FormSizer->Add(TopSizer, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Char set"));
	cbLower = new wxCheckBox(this, ID_CHECKBOX5, _("Lower case"), wxDefaultPosition, wxSize(68,13), wxCHK_2STATE, wxDefaultValidator, _T("ID_CHECKBOX5"));
	cbLower->SetValue(true);
	StaticBoxSizer1->Add(cbLower, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	cbUpper = new wxCheckBox(this, ID_CHECKBOX2, _("Upper case"), wxDefaultPosition, wxSize(69,13), wxCHK_2STATE, wxDefaultValidator, _T("ID_CHECKBOX2"));
	cbUpper->SetValue(true);
	StaticBoxSizer1->Add(cbUpper, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	cbDigits = new wxCheckBox(this, ID_CHECKBOX3, _("Digits"), wxDefaultPosition, wxSize(66,16), wxCHK_2STATE, wxDefaultValidator, _T("ID_CHECKBOX3"));
	cbDigits->SetValue(true);
	StaticBoxSizer1->Add(cbDigits, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	cbSpecials = new wxCheckBox(this, ID_CHECKBOX4, _("Special chars"), wxDefaultPosition, wxSize(78,13), wxCHK_2STATE, wxDefaultValidator, _T("ID_CHECKBOX4"));
	cbSpecials->SetValue(true);
	StaticBoxSizer1->Add(cbSpecials, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FormSizer->Add(StaticBoxSizer1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Length"));
	FlexGridSizer3 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText2 = new wxStaticText(this, wxID_ANY, _("Minimal"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	FlexGridSizer3->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	edMinLength = new wxTextCtrl(this, ID_TEXTCTRL2, _("10"), wxDefaultPosition, wxSize(45,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	FlexGridSizer3->Add(edMinLength, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2->Add(FlexGridSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer4 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText3 = new wxStaticText(this, wxID_ANY, _("Maximal"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	FlexGridSizer4->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	edMaxLength = new wxTextCtrl(this, ID_TEXTCTRL3, _("20"), wxDefaultPosition, wxSize(45,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	FlexGridSizer4->Add(edMaxLength, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2->Add(FlexGridSizer4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FormSizer->Add(StaticBoxSizer2, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer5 = new wxFlexGridSizer(0, 3, 0, 0);
	btGenerate = new wxButton(this, ID_BUTTON1, _("Generate"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	FlexGridSizer5->Add(btGenerate, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer5->Add(92,20,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StdDialogButtonSizer1 = new wxStdDialogButtonSizer();
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
	StdDialogButtonSizer1->Realize();
	FlexGridSizer5->Add(StdDialogButtonSizer1, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	FormSizer->Add(FlexGridSizer5, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(FormSizer);
	SetSizer(FormSizer);
	Layout();
	Center();

	Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&GenDialog::OnedPasswordTextEnter);
	Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&GenDialog::OncbShowClick);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&GenDialog::OnbtGenerateClick);
	Connect(wxID_ANY,wxEVT_INIT_DIALOG,(wxObjectEventFunction)&GenDialog::OnInit);
	//*)
}

GenDialog::~GenDialog()
{
	//(*Destroy(GenDialog)
	//*)
}

unsigned int Random(unsigned int min_value, unsigned int max_value)
{
  if (min_value < max_value)
    return (rand() % (max_value - min_value)) + min_value;
  else
    return 0;
}

unsigned int Random(unsigned int max_value)
{
  return Random(0, max_value);
}

const int GenDialog::ShowModalEx(wxString& password)
{
  FormSizer->Fit(this);
  int res = ShowModal();
  password = edPassword->GetValue();

  if (res == wxID_OK)
  {
    CRegistrySaver saver("PasswordKeeper");
    saver.SaveBool(cbShow->GetValue(), "Show");
    saver.SaveBool(cbLower->GetValue(), "Lower");
    saver.SaveBool(cbUpper->GetValue(), "Upper");
    saver.SaveBool(cbDigits->GetValue(), "Digits");
    saver.SaveBool(cbSpecials->GetValue(), "Specials");
    saver.SaveStringW(edMinLength->GetValue(), "MinLength");
    saver.SaveStringW(edMaxLength->GetValue(), "MaxLength");
  }

  return res;
}

const int StrToInt(const wxString& str)
{
  unsigned long result;
  if (!str.ToULong(&result))
  {
    wxMessageBox("Value \"" + str + "\" can't be converted to number", "Error", wxOK | wxICON_ERROR);
    return -1;
  }
  else
    return result;
}

void GenDialog::OnbtGenerateClick(wxCommandEvent& event)
{
  GeneratePassword();
}

void GenDialog::OncbShowClick(wxCommandEvent& event)
{
  PasswordVisibility();
}

void GenDialog::OnInit(wxInitDialogEvent& event)
{
  CRegistrySaver saver("PasswordKeeper");
  cbShow->SetValue(saver.LoadBool("Show", cbShow->GetValue()));
  cbLower->SetValue(saver.LoadBool("Lower", cbLower->GetValue()));
  cbUpper->SetValue(saver.LoadBool("Upper", cbUpper->GetValue()));
  cbDigits->SetValue(saver.LoadBool("Digits", cbDigits->GetValue()));
  cbSpecials->SetValue(saver.LoadBool("Specials", cbSpecials->GetValue()));
  edMinLength->SetValue(saver.LoadStringW("MinLength", edMinLength->GetValue()));
  edMaxLength->SetValue(saver.LoadStringW("MaxLength", edMaxLength->GetValue()));

  GeneratePassword();
  PasswordVisibility();
}

enum CharType {
  ctUpper = 0,
  ctLower,
  ctDigit,
  ctSpecial
};

void GenDialog::PasswordVisibility()
{
  if (cbShow->GetValue())
    SendMessage((HWND)edPassword->GetHandle(), EM_SETPASSWORDCHAR, 0, 0);
  else
  {
    wxString temp = edPassword->GetValue();
    TopSizer->Detach(edPassword);
    RemoveChild(edPassword);
    delete edPassword;
    edPassword = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxSize(272,21), wxTE_PASSWORD, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    TopSizer->Insert(1, edPassword, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    edPassword->SetValue(temp);
    Layout();
  }
  edPassword->SetFocus();
}

void GenDialog::GeneratePassword()
{
  const int SPECIALS_COUNT = 28;
  char specials[SPECIALS_COUNT] = {'~', '!', '@', '#', '$', '%', '&', '^', '*', '(', ')', '-',
                                    '_', '+', '{', '}', '[', ']', '\\', '/', '.', ',', '<', '>',
                                    '?', ':', ';', '"'};

  unsigned long min_length = StrToInt(edMinLength->GetValue()), max_length = StrToInt(edMaxLength->GetValue());
  int length = Random(min_length, max_length + 1);

  std::vector <CharType> types;
  if (cbLower->GetValue())    types.push_back(ctLower);
  if (cbUpper->GetValue())    types.push_back(ctUpper);
  if (cbDigits->GetValue())   types.push_back(ctDigit);
  if (cbSpecials->GetValue()) types.push_back(ctSpecial);

  if (types.size() < 1)
  {
    wxMessageBox("At least one character type must be selected", "Error", wxOK | wxICON_ERROR);
    return;
  }

  wxString password;
  for (int i = 0; i < length; ++i)
  {
    CharType charType = types[Random(types.size())];
    char newChar = 'a';
    switch (charType)
    {
      case ctUpper:
        newChar = Random('A', 'Z');
        break;
      case ctLower:
        newChar = Random('a', 'z');
        break;
      case ctDigit:
        newChar = Random('0', '9');
        break;
      case ctSpecial:
        newChar = specials[Random(SPECIALS_COUNT)];
        break;
    }
    password.Append(newChar);
  }
  edPassword->SetValue(password);
}

void GenDialog::OnedPasswordTextEnter(wxCommandEvent& event)
{
  EndDialog(wxID_OK);
}
