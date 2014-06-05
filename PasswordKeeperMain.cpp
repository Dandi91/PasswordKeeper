/***************************************************************
 * Name:      PasswordKeeperMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Dandi ()
 * Created:   2014-05-28
 * Copyright: Dandi ()
 * License:
 **************************************************************/

#include "PasswordKeeperMain.h"
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/clipbrd.h>

//(*InternalHeaders(PasswordKeeperFrame)
#include <wx/settings.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include "CryptoFile.h"
#include <wx/dynarray.h>
#include "PropDialog.h"
#include "RegistrySaver.h"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(PasswordKeeperFrame)
const long PasswordKeeperFrame::ID_NOTEBOOK = wxNewId();
const long PasswordKeeperFrame::idMenuNew = wxNewId();
const long PasswordKeeperFrame::idMenuOpen = wxNewId();
const long PasswordKeeperFrame::idMenuSave = wxNewId();
const long PasswordKeeperFrame::idMenuSaveAs = wxNewId();
const long PasswordKeeperFrame::idMenuMerge = wxNewId();
const long PasswordKeeperFrame::idMenuClose = wxNewId();
const long PasswordKeeperFrame::idMenuCloseAll = wxNewId();
const long PasswordKeeperFrame::idMenuQuit = wxNewId();
const long PasswordKeeperFrame::idMenuView = wxNewId();
const long PasswordKeeperFrame::idMenuAdd = wxNewId();
const long PasswordKeeperFrame::idMenuEdit = wxNewId();
const long PasswordKeeperFrame::idMenuDelete = wxNewId();
const long PasswordKeeperFrame::idMenuAbout = wxNewId();
const long PasswordKeeperFrame::ID_STATUSBAR = wxNewId();
const long PasswordKeeperFrame::idMenuViewP = wxNewId();
const long PasswordKeeperFrame::idMenuAddP = wxNewId();
const long PasswordKeeperFrame::idMenuEditP = wxNewId();
const long PasswordKeeperFrame::idMenuDeleteP = wxNewId();
//*)

const long PasswordKeeperFrame::ID_LIST = wxNewId();

BEGIN_EVENT_TABLE(PasswordKeeperFrame,wxFrame)
    //(*EventTable(PasswordKeeperFrame)
    //*)
END_EVENT_TABLE()

WX_DECLARE_OBJARRAY(CCryptoFile, CFilesArray);

#include <wx/arrimpl.cpp> // This is a magic incantation which must be done!
WX_DEFINE_OBJARRAY(CFilesArray);

CFilesArray fileList;

CCryptoFile* PasswordKeeperFrame::CurrentFile()
{
  return &fileList[tbTabs->GetSelection()];
}

const int PasswordKeeperFrame::CurrentLine()
{
  return lbList->GetSelection();
}

PasswordKeeperFrame::PasswordKeeperFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(PasswordKeeperFrame)
    wxMenuItem* miQuit;
    wxMenuBar* meMainMenu;
    wxMenu* miFile;
    wxMenu* miHelp;
    wxMenuItem* miAbout;

    Create(parent, id, _("Password Keeper 1.1"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    tbTabs = new wxNotebook(this, ID_NOTEBOOK, wxDefaultPosition, wxSize(463,300), 0, _T("ID_NOTEBOOK"));
    BoxSizer1->Add(tbTabs, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(BoxSizer1);
    meMainMenu = new wxMenuBar();
    miFile = new wxMenu();
    miNew = new wxMenuItem(miFile, idMenuNew, _("&New\tCtrl+N"), _("Create a new file"), wxITEM_NORMAL);
    miFile->Append(miNew);
    miOpen = new wxMenuItem(miFile, idMenuOpen, _("&Open\tCtrl+O"), _("Open a file"), wxITEM_NORMAL);
    miFile->Append(miOpen);
    miSave = new wxMenuItem(miFile, idMenuSave, _("&Save\tCtrl+S"), _("Save current file"), wxITEM_NORMAL);
    miFile->Append(miSave);
    miSaveAs = new wxMenuItem(miFile, idMenuSaveAs, _("Save As...\tCtrl+Alt+S"), _("Save current file with a different name"), wxITEM_NORMAL);
    miFile->Append(miSaveAs);
    miMerge = new wxMenuItem(miFile, idMenuMerge, _("&Merge With...\tCtrl+M"), _("Merge current file with another"), wxITEM_NORMAL);
    miFile->Append(miMerge);
    miFile->AppendSeparator();
    miClose = new wxMenuItem(miFile, idMenuClose, _("&Close\tCtrl+W"), _("Close current file"), wxITEM_NORMAL);
    miFile->Append(miClose);
    miCloseAll = new wxMenuItem(miFile, idMenuCloseAll, _("Close All\tCtrl+Alt+W"), _("Close all files"), wxITEM_NORMAL);
    miFile->Append(miCloseAll);
    miFile->AppendSeparator();
    miQuit = new wxMenuItem(miFile, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    miFile->Append(miQuit);
    meMainMenu->Append(miFile, _("&File"));
    miRecord = new wxMenu();
    miView = new wxMenuItem(miRecord, idMenuView, _("&View...\tCtrl+I"), _("View selected record"), wxITEM_NORMAL);
    miRecord->Append(miView);
    miRecord->AppendSeparator();
    miAdd = new wxMenuItem(miRecord, idMenuAdd, _("&Add...\tCtrl+A"), _("Add new record to current file"), wxITEM_NORMAL);
    miRecord->Append(miAdd);
    miEdit = new wxMenuItem(miRecord, idMenuEdit, _("&Edit...\tCtrl+E"), _("Edit selected record"), wxITEM_NORMAL);
    miRecord->Append(miEdit);
    miDelete = new wxMenuItem(miRecord, idMenuDelete, _("&Delete\tCtrl+D"), _("Delete selected record"), wxITEM_NORMAL);
    miRecord->Append(miDelete);
    meMainMenu->Append(miRecord, _("&Record"));
    miHelp = new wxMenu();
    miAbout = new wxMenuItem(miHelp, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    miHelp->Append(miAbout);
    meMainMenu->Append(miHelp, _("&Help"));
    SetMenuBar(meMainMenu);
    sbStatus = new wxStatusBar(this, ID_STATUSBAR, 0, _T("ID_STATUSBAR"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    sbStatus->SetFieldsCount(1,__wxStatusBarWidths_1);
    sbStatus->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(sbStatus);
    mpView = new wxMenuItem((&puMenu), idMenuViewP, _("&View...\tCtrl+I"), _("View selected record"), wxITEM_NORMAL);
    puMenu.Append(mpView);
    puMenu.AppendSeparator();
    mpAdd = new wxMenuItem((&puMenu), idMenuAddP, _("&Add...\tCtrl+A"), _("Add new record to current file"), wxITEM_NORMAL);
    puMenu.Append(mpAdd);
    mpEdit = new wxMenuItem((&puMenu), idMenuEditP, _("&Edit...\tCtrl+E"), _("Edit selected record"), wxITEM_NORMAL);
    puMenu.Append(mpEdit);
    mpDelete = new wxMenuItem((&puMenu), idMenuDeleteP, _("&Delete\tCtrl+D"), _("Delete selected record"), wxITEM_NORMAL);
    puMenu.Append(mpDelete);
    BoxSizer1->Fit(this);
    BoxSizer1->SetSizeHints(this);

    Connect(ID_NOTEBOOK,wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,(wxObjectEventFunction)&PasswordKeeperFrame::OntbTabsPageChanged);
    Connect(idMenuNew,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiNewSelected);
    Connect(idMenuOpen,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiOpenSelected);
    Connect(idMenuSave,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiSaveSelected);
    Connect(idMenuSaveAs,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiSaveAsSelected);
    Connect(idMenuMerge,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiMergeSelected);
    Connect(idMenuClose,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiCloseSelected);
    Connect(idMenuCloseAll,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiCloseAllSelected);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnQuit);
    Connect(idMenuView,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiViewSelected);
    Connect(idMenuAdd,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiAddSelected);
    Connect(idMenuEdit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiEditSelected);
    Connect(idMenuDelete,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiDeleteSelected);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnAbout);
    Connect(idMenuViewP,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiViewSelected);
    Connect(idMenuAddP,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiAddSelected);
    Connect(idMenuEditP,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiEditSelected);
    Connect(idMenuDeleteP,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiDeleteSelected);
    Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&PasswordKeeperFrame::OnClose);
    //*)

    pnPanel = NULL;
    BoxSizer = NULL;
    lbList = NULL;

    srand((unsigned int)time(NULL));

    CRegistrySaver saver("PasswordKeeper");
    saver.LoadWindow(*this, "MainWindow");
    wxArrayString mru;
    int active = -1;
    saver.LoadMRU(mru, active);
    for (size_t i = 0; i < mru.Count(); ++i)
    {
      fileList.Add(CCryptoFile(mru[i]));
      tbTabs->AddPage(GetTabPage(), fileList[fileList.Count() - 1].GetFileName(), true);
    }
    if ((active < (int)tbTabs->GetPageCount()) && (active != -1))
      tbTabs->SetSelection(active);

    UpdateInterface();
}

PasswordKeeperFrame::~PasswordKeeperFrame()
{
  //(*Destroy(PasswordKeeperFrame)
  //*)
  if (pnPanel)
    delete pnPanel;
}

void PasswordKeeperFrame::OnQuit(wxCommandEvent& event)
{
  Close();
}

void PasswordKeeperFrame::UpdateInterface()
{
  int tabsSelection = tbTabs->GetSelection();
  if (lbList != NULL)
  {
    if (tabsSelection > -1)
    {
      // Save selection before cleaning
      int listSelection = CurrentLine();
      // Reread values from file
      CCryptoFile* f = CurrentFile();
     // f->SortContent();
      CContent* curr = f->content;
      CContent::iterator i;
      wxArrayString items;
      volatile int a = curr->GetCount();
      for (i = curr->begin(); i != curr->end(); ++i)
      {
        CRecord* rec = *i;
        items.Add(rec->name);
      }
      // Gather strings from list
      wxArrayString olds;
      for (size_t i = 0; i < lbList->GetCount(); ++i)
        olds.Add(lbList->GetString(i));
      // Compare and fill up
      if (items != olds)
      {
        lbList->Clear();
        if (!items.IsEmpty())
          lbList->InsertItems(items, 0);
      }
      // Add asterisk to caption if file isn't saved
      wxString s = f->GetFileName();
      if (!f->isSaved)
        s += '*';
      tbTabs->SetPageText(tabsSelection, s);
      // Restore selection
      if (listSelection < (int)lbList->GetCount())
        lbList->SetSelection(listSelection);
    }
  }
  bool flag = tabsSelection > -1;
  miClose->Enable(flag);
  miCloseAll->Enable(flag);
  miSave->Enable(flag && !CurrentFile()->isSaved);
  miSaveAs->Enable(flag);
  miAdd->Enable(flag);
  miMerge->Enable(flag);
  if (lbList == NULL)
    flag = false;
  else
    flag &= lbList->GetSelection() != wxNOT_FOUND;
  miView->Enable(flag);
  miEdit->Enable(flag);
  miDelete->Enable(flag);
}

wxWindow* PasswordKeeperFrame::GetTabPage()
{
  if (pnPanel == NULL)
  {
    // If hasn't created yet
    pnPanel = new wxPanel(tbTabs);
    pnPanel->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
    BoxSizer = new wxBoxSizer(wxHORIZONTAL);
    lbList = new wxListBox(pnPanel, ID_LIST, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE, wxDefaultValidator, wxListBoxNameStr);
    BoxSizer->Add(lbList, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 8);
    pnPanel->SetSizer(BoxSizer);
    Connect(ID_LIST, wxEVT_LISTBOX, (wxObjectEventFunction)&PasswordKeeperFrame::OnListClick);
    Connect(ID_LIST, wxEVT_LISTBOX_DCLICK, (wxObjectEventFunction)&PasswordKeeperFrame::OnListDblClick);
    lbList->Connect(ID_LIST, wxEVT_RIGHT_UP, (wxObjectEventFunction)&PasswordKeeperFrame::OnListRightClick, NULL, this);
  }
  return pnPanel;
}

void PasswordKeeperFrame::OnListClick(wxCommandEvent& event)
{
  UpdateInterface();
}

void PasswordKeeperFrame::OnListDblClick(wxCommandEvent& event)
{
  if (wxTheClipboard->Open())
  {
    CRecord* rec = CurrentFile()->content->Item(CurrentLine())->GetData();
    wxTheClipboard->SetData(new wxTextDataObject(rec->password));
    wxTheClipboard->Close();
    wxMessageBox("Password has been copied to the clipboard", "Information", wxOK | wxICON_INFORMATION);
  }
}

void PasswordKeeperFrame::OnListRightClick(wxMouseEvent& event)
{
  lbList->SetSelection(lbList->HitTest(event.GetPosition()));
  UpdateInterface();
  lbList->PopupMenu(&puMenu);
}

void PasswordKeeperFrame::OnAbout(wxCommandEvent& event)
{
  wxString msg = wxbuildinfo(long_f);
  wxMessageBox(msg, _("Welcome to..."));
}

void PasswordKeeperFrame::OnmiNewSelected(wxCommandEvent& event)
{
  fileList.Add(CCryptoFile("?"));
  tbTabs->AddPage(GetTabPage(), "?", true);
  UpdateInterface();
}

void PasswordKeeperFrame::OntbTabsPageChanged(wxNotebookEvent& event)
{
  UpdateInterface();
}

void PasswordKeeperFrame::OnmiOpenSelected(wxCommandEvent& event)
{
  wxFileDialog openDlg(this, "Open file(s)", "", "", "Password Keeper files (*.pkf)|*.pkf|All files|*",
                        wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);
  if (openDlg.ShowModal() == wxID_OK)
  {
    wxArrayString files;
    openDlg.GetPaths(files);
    for (size_t i = 0; i < files.Count(); ++i)
    {
      fileList.Add(CCryptoFile(files[i]));
      tbTabs->AddPage(GetTabPage(), fileList[fileList.Count() - 1].GetFileName(), true);
    }
  }
}

void PasswordKeeperFrame::OnmiSaveSelected(wxCommandEvent& event)
{
  if (CurrentFile()->GetFileName().Cmp('?') == 0)
    OnmiSaveAsSelected(event);
  else
  {
    CurrentFile()->WriteFile(true, true);
    UpdateInterface();
  }
}

void PasswordKeeperFrame::OnmiSaveAsSelected(wxCommandEvent& event)
{
  wxFileDialog saveDlg(this, "Save file", "", "", "Password Keeper files (*.pkf)|*.pkf|All files|*",
                        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  if (saveDlg.ShowModal() == wxID_OK)
  {
    CurrentFile()->SetFilePath(saveDlg.GetPath());
    CurrentFile()->WriteFile(true, true);
    UpdateInterface();
  }
}

void PasswordKeeperFrame::OnmiCloseSelected(wxCommandEvent& event)
{
  if (!CurrentFile()->isSaved)
  {
    int res = wxMessageBox(wxString::Format("File \"%s\" has been changed. Do you want to save it?", CurrentFile()->GetFilePath()),
                           "Information", wxYES_NO | wxCANCEL | wxICON_EXCLAMATION);
    if (res == wxYES)
      OnmiSaveSelected(event);
    else if (res == wxCANCEL)
      return;
  }
  int sel = tbTabs->GetSelection();
  fileList.RemoveAt(sel, 1);
  tbTabs->RemovePage(sel);
  UpdateInterface();
}

void PasswordKeeperFrame::OnmiCloseAllSelected(wxCommandEvent& event)
{
  size_t i = tbTabs->GetPageCount();
  while (i-- > 0)
    OnmiCloseSelected(event);
}

void PasswordKeeperFrame::OnmiViewSelected(wxCommandEvent& event)
{
  PropDialog dlg(this);
  CRecord* record = CurrentFile()->content->Item(CurrentLine())->GetData();
  dlg.ShowModalEx(*record, smVIEW);
}

void PasswordKeeperFrame::OnmiAddSelected(wxCommandEvent& event)
{
  PropDialog dlg(this);
  CRecord* record = new CRecord;
  dlg.ShowModalEx(*record, smADD);
  CurrentFile()->content->Append(record);
  CurrentFile()->isSaved = false;
  UpdateInterface();
}

void PasswordKeeperFrame::OnmiEditSelected(wxCommandEvent& event)
{
  PropDialog dlg(this);
  CRecord* oldRecord = CurrentFile()->content->Item(CurrentLine())->GetData();
  CRecord* newRecord = new CRecord;
  *newRecord = *oldRecord;
  if (dlg.ShowModalEx(*newRecord, smEDIT) == wxID_OK)
  {
    CurrentFile()->content->DeleteObject(oldRecord);
    CurrentFile()->content->Append(newRecord);
    CurrentFile()->isSaved = false;
    UpdateInterface();
  }
}

void PasswordKeeperFrame::OnmiDeleteSelected(wxCommandEvent& event)
{
  if (wxMessageBox("Are you sure to delete this record?", "Confirmation", wxYES_NO | wxICON_EXCLAMATION) == wxYES)
  {
    CurrentFile()->content->DeleteNode(CurrentFile()->content->Item(CurrentLine()));
    CurrentFile()->isSaved = false;
    UpdateInterface();
  }
}

void PasswordKeeperFrame::OnmiMergeSelected(wxCommandEvent& event)
{
  wxFileDialog openDlg(this, "Open file to merge", "", "", "Password Keeper files (*.pkf)|*.pkf|All files|*",
                        wxFD_OPEN | wxFD_FILE_MUST_EXIST);
  if (openDlg.ShowModal() == wxID_OK)
  {
    CCryptoFile mergeFile(openDlg.GetPath());
    CurrentFile()->MergeWith(mergeFile);
    UpdateInterface();
  }
}

void PasswordKeeperFrame::OnClose(wxCloseEvent& event)
{
  CRegistrySaver saver("PasswordKeeper");
  saver.SaveWindow(*this, "MainWindow");
  wxArrayString mru;
  for (size_t i = 0; i < fileList.Count(); ++i)
    if (fileList[i].GetFilePath().Cmp('?'))
      mru.Add(fileList[i].GetFilePath());
  saver.SaveMRU(mru, tbTabs->GetSelection());
  wxCommandEvent cmdEvent;
  OnmiCloseAllSelected(cmdEvent);
  event.Skip();
}
