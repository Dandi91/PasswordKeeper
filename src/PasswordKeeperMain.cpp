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
#include <wx/icon.h>

#if defined(__WXMSW__)
  #include <wx/iconbndl.h>
#endif

//(*InternalHeaders(PasswordKeeperFrame)
#include <wx/settings.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include <wx/event.h>
#include "PropDialog.h"
#include "AuthDialog.h"
#include "TabDialog.h"
#include "Saver.h"

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
const long PasswordKeeperFrame::idMenuSwitch = wxNewId();
const long PasswordKeeperFrame::idMenuSynchronize = wxNewId();
const long PasswordKeeperFrame::idMenuChange = wxNewId();
const long PasswordKeeperFrame::idMenuSave = wxNewId();
const long PasswordKeeperFrame::idMenuQuit = wxNewId();
const long PasswordKeeperFrame::idMenuTabAdd = wxNewId();
const long PasswordKeeperFrame::idMenuTabRename = wxNewId();
const long PasswordKeeperFrame::idMenuTabDelete = wxNewId();
const long PasswordKeeperFrame::idMenuView = wxNewId();
const long PasswordKeeperFrame::idMenuAdd = wxNewId();
const long PasswordKeeperFrame::idMenuEdit = wxNewId();
const long PasswordKeeperFrame::ID_MENUITEM2 = wxNewId();
const long PasswordKeeperFrame::idMenuMove = wxNewId();
const long PasswordKeeperFrame::idMenuDelete = wxNewId();
const long PasswordKeeperFrame::idMenuCopyName = wxNewId();
const long PasswordKeeperFrame::idMenuCopyLogin = wxNewId();
const long PasswordKeeperFrame::idMenuCopyEmail = wxNewId();
const long PasswordKeeperFrame::idMenuCopyPass = wxNewId();
const long PasswordKeeperFrame::idMenuAbout = wxNewId();
const long PasswordKeeperFrame::ID_STATUSBAR = wxNewId();
const long PasswordKeeperFrame::idMenuViewP = wxNewId();
const long PasswordKeeperFrame::idMenuAddP = wxNewId();
const long PasswordKeeperFrame::idMenuEditP = wxNewId();
const long PasswordKeeperFrame::ID_MENUITEM1 = wxNewId();
const long PasswordKeeperFrame::idMenuMoveP = wxNewId();
const long PasswordKeeperFrame::idMenuDeleteP = wxNewId();
const long PasswordKeeperFrame::idMenuCopyNameP = wxNewId();
const long PasswordKeeperFrame::idMenuCopyLoginP = wxNewId();
const long PasswordKeeperFrame::idMenuCopyEmailP = wxNewId();
const long PasswordKeeperFrame::idMenuCopyPassP = wxNewId();
const long PasswordKeeperFrame::idTabAddP = wxNewId();
const long PasswordKeeperFrame::idTabRenameP = wxNewId();
const long PasswordKeeperFrame::idTabDeleteP = wxNewId();
//*)

const long PasswordKeeperFrame::ID_LIST = wxNewId();

BEGIN_EVENT_TABLE(PasswordKeeperFrame,wxFrame)
    //(*EventTable(PasswordKeeperFrame)
    //*)
END_EVENT_TABLE()

void PutStringToClipboard(const wxString& string, const wxString& stringName)
{
  if (wxTheClipboard->Open())
  {
    wxTheClipboard->SetData(new wxTextDataObject(string));
    wxTheClipboard->Close();
    wxMessageBox(stringName + " has been copied to the clipboard", "Information", wxOK | wxICON_INFORMATION);
  }
}

CRecordList* PasswordKeeperFrame::CurrentList()
{
  return account->GetContent()->GetList(tbTabs->GetSelection());
}

const int PasswordKeeperFrame::CurrentLine(int* selCount = NULL)
{
  if (lbList)
  {
    wxArrayInt selection;
    lbList->GetSelections(selection);
    if (selCount)
      *selCount = selection.size();
    if (selection.size() == 1)
      return selection[0];
  }
  else
    if (selCount)
      *selCount = 0;
  return -1;
}

PasswordKeeperFrame::PasswordKeeperFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(PasswordKeeperFrame)
    wxMenu* miAccount;
    wxMenu* miTab;
    wxMenuItem* miQuit;
    wxMenuBar* meMainMenu;
    wxBoxSizer* BoxSizer1;
    wxMenu* miHelp;
    wxMenuItem* miAbout;

    Create(parent, id, _("Password Keeper 1.1"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    tbTabs = new wxNotebook(this, ID_NOTEBOOK, wxDefaultPosition, wxSize(463,300), 0, _T("ID_NOTEBOOK"));
    BoxSizer1->Add(tbTabs, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(BoxSizer1);
    meMainMenu = new wxMenuBar();
    miAccount = new wxMenu();
    miSwitch = new wxMenuItem(miAccount, idMenuSwitch, _("S&witch...\tCtrl+W"), _("Switch account"), wxITEM_NORMAL);
    miAccount->Append(miSwitch);
    miSync = new wxMenuItem(miAccount, idMenuSynchronize, _("S&ynchronize...\tCtrl+Y"), _("Synchronize account with server"), wxITEM_NORMAL);
    miAccount->Append(miSync);
    miChange = new wxMenuItem(miAccount, idMenuChange, _("&Change login/password"), _("Change login or password for this account"), wxITEM_NORMAL);
    miAccount->Append(miChange);
    miSave = new wxMenuItem(miAccount, idMenuSave, _("&Save locally\tCtrl+S"), _("Save to local file"), wxITEM_NORMAL);
    miAccount->Append(miSave);
    miAccount->AppendSeparator();
    miQuit = new wxMenuItem(miAccount, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    miAccount->Append(miQuit);
    meMainMenu->Append(miAccount, _("&Account"));
    miTab = new wxMenu();
    miAddTab = new wxMenuItem(miTab, idMenuTabAdd, _("&Add...\tCtrl+T"), _("Add new tab"), wxITEM_NORMAL);
    miTab->Append(miAddTab);
    miRenameTab = new wxMenuItem(miTab, idMenuTabRename, _("&Rename...\tCtrl+R"), _("Rename current tab"), wxITEM_NORMAL);
    miTab->Append(miRenameTab);
    miDeleteTab = new wxMenuItem(miTab, idMenuTabDelete, _("&Delete"), _("Delete current tab with all records in it"), wxITEM_NORMAL);
    miTab->Append(miDeleteTab);
    meMainMenu->Append(miTab, _("&Tab"));
    miRecord = new wxMenu();
    miView = new wxMenuItem(miRecord, idMenuView, _("&View...\tCtrl+I"), _("View selected record"), wxITEM_NORMAL);
    miRecord->Append(miView);
    miRecord->AppendSeparator();
    miAdd = new wxMenuItem(miRecord, idMenuAdd, _("&Add...\tCtrl+A"), _("Add new record to the current tab"), wxITEM_NORMAL);
    miRecord->Append(miAdd);
    miEdit = new wxMenuItem(miRecord, idMenuEdit, _("&Edit...\tCtrl+E"), _("Edit selected record"), wxITEM_NORMAL);
    miRecord->Append(miEdit);
    miMove = new wxMenu();
    MenuItem2 = new wxMenuItem(miMove, ID_MENUITEM2, _("New Menu"), wxEmptyString, wxITEM_NORMAL);
    miMove->Append(MenuItem2);
    miRecord->Append(idMenuMove, _("&Move"), miMove, wxEmptyString);
    miDelete = new wxMenuItem(miRecord, idMenuDelete, _("&Delete\tDelete"), _("Delete selected records"), wxITEM_NORMAL);
    miRecord->Append(miDelete);
    miRecord->AppendSeparator();
    miCopyName = new wxMenuItem(miRecord, idMenuCopyName, _("Copy name"), _("Copy record\'s name to the clipboard"), wxITEM_NORMAL);
    miRecord->Append(miCopyName);
    miCopyLogin = new wxMenuItem(miRecord, idMenuCopyLogin, _("Copy login"), _("Copy record\'s login to the clipboard"), wxITEM_NORMAL);
    miRecord->Append(miCopyLogin);
    miCopyEmail = new wxMenuItem(miRecord, idMenuCopyEmail, _("Copy email"), _("Copy record\'s email to the clipboard"), wxITEM_NORMAL);
    miRecord->Append(miCopyEmail);
    miCopyPass = new wxMenuItem(miRecord, idMenuCopyPass, _("Copy password"), _("Copy record\'s password to the clipboard"), wxITEM_NORMAL);
    miRecord->Append(miCopyPass);
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
    mpView = new wxMenuItem((&puListMenu), idMenuViewP, _("&View...\tCtrl+I"), _("View selected record"), wxITEM_NORMAL);
    puListMenu.Append(mpView);
    puListMenu.AppendSeparator();
    mpAdd = new wxMenuItem((&puListMenu), idMenuAddP, _("&Add...\tCtrl+A"), _("Add new record to the current tab"), wxITEM_NORMAL);
    puListMenu.Append(mpAdd);
    mpEdit = new wxMenuItem((&puListMenu), idMenuEditP, _("&Edit...\tCtrl+E"), _("Edit selected record"), wxITEM_NORMAL);
    puListMenu.Append(mpEdit);
    mpMove = new wxMenu();
    MenuItem1 = new wxMenuItem(mpMove, ID_MENUITEM1, _("New Menu"), wxEmptyString, wxITEM_NORMAL);
    mpMove->Append(MenuItem1);
    puListMenu.Append(idMenuMoveP, _("&Move"), mpMove, wxEmptyString);
    mpDelete = new wxMenuItem((&puListMenu), idMenuDeleteP, _("&Delete"), _("Delete selected records"), wxITEM_NORMAL);
    puListMenu.Append(mpDelete);
    puListMenu.AppendSeparator();
    mpCopyName = new wxMenuItem((&puListMenu), idMenuCopyNameP, _("Copy name"), _("Copy record\'s name to the clipboard"), wxITEM_NORMAL);
    puListMenu.Append(mpCopyName);
    mpCopyLogin = new wxMenuItem((&puListMenu), idMenuCopyLoginP, _("Copy login"), _("Copy record\'s login to the clipboard"), wxITEM_NORMAL);
    puListMenu.Append(mpCopyLogin);
    mpCopyEmail = new wxMenuItem((&puListMenu), idMenuCopyEmailP, _("Copy email"), _("Copy record\'s email to the clipboard"), wxITEM_NORMAL);
    puListMenu.Append(mpCopyEmail);
    mpCopyPass = new wxMenuItem((&puListMenu), idMenuCopyPassP, _("Copy password"), _("Copy record\'s password to the clipboard"), wxITEM_NORMAL);
    puListMenu.Append(mpCopyPass);
    mpTabAdd = new wxMenuItem((&puTabMenu), idTabAddP, _("&Add...\tCtrl+T"), wxEmptyString, wxITEM_NORMAL);
    puTabMenu.Append(mpTabAdd);
    mpTabRename = new wxMenuItem((&puTabMenu), idTabRenameP, _("&Rename...\tCtrl+R"), wxEmptyString, wxITEM_NORMAL);
    puTabMenu.Append(mpTabRename);
    mpTabDelete = new wxMenuItem((&puTabMenu), idTabDeleteP, _("&Delete"), wxEmptyString, wxITEM_NORMAL);
    puTabMenu.Append(mpTabDelete);
    BoxSizer1->Fit(this);
    BoxSizer1->SetSizeHints(this);

    Connect(ID_NOTEBOOK,wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,(wxObjectEventFunction)&PasswordKeeperFrame::OntbTabsPageChanged);
    Connect(idMenuSwitch,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiSwitchSelected);
    Connect(idMenuSynchronize,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiSyncSelected);
    Connect(idMenuSave,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiSaveSelected);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiQuitSelected);
    Connect(idMenuTabAdd,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiAddTabSelected);
    Connect(idMenuTabRename,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiRenameTabSelected);
    Connect(idMenuTabDelete,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiDeleteTabSelected);
    Connect(idMenuView,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiViewSelected);
    Connect(idMenuAdd,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiAddSelected);
    Connect(idMenuEdit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiEditSelected);
    Connect(idMenuDelete,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiDeleteSelected);
    Connect(idMenuCopyName,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiCopyNameSelected);
    Connect(idMenuCopyLogin,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiCopyLoginSelected);
    Connect(idMenuCopyEmail,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiCopyEmailSelected);
    Connect(idMenuCopyPass,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiCopyPassSelected);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiAboutSelected);
    Connect(idMenuViewP,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiViewSelected);
    Connect(idMenuAddP,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiAddSelected);
    Connect(idMenuEditP,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiEditSelected);
    Connect(idMenuDeleteP,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiDeleteSelected);
    Connect(idMenuCopyNameP,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiCopyNameSelected);
    Connect(idMenuCopyLoginP,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiCopyLoginSelected);
    Connect(idMenuCopyEmailP,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiCopyEmailSelected);
    Connect(idMenuCopyPassP,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiCopyPassSelected);
    Connect(idTabAddP,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiAddTabSelected);
    Connect(idTabRenameP,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiRenameTabSelected);
    Connect(idTabDeleteP,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PasswordKeeperFrame::OnmiDeleteTabSelected);
    Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&PasswordKeeperFrame::OnClose);
    //*)

    pnPanel = NULL;
    BoxSizer = NULL;
    lbList = NULL;

    // Tab dragging
    isDragging = false;
    tbTabs->Bind(wxEVT_LEFT_DOWN, (wxObjectEventFunction)&PasswordKeeperFrame::OnMouseEvent, this);
    tbTabs->Bind(wxEVT_LEFT_UP, (wxObjectEventFunction)&PasswordKeeperFrame::OnMouseEvent, this);
    tbTabs->Bind(wxEVT_MOTION, (wxObjectEventFunction)&PasswordKeeperFrame::OnMouseEvent, this);

    // Tab popup menu
    tbTabs->Bind(wxEVT_RIGHT_UP, (wxObjectEventFunction)&PasswordKeeperFrame::OnTabsRightUp, this);

    account = &CAccount::Get();

    CSaver::Get().LoadWindow(*this, "Window");
    SetLabel(account->GetLogin() + " - Password Keeper");

    // Application icon
    #if defined(__WXMSW__)
      // Multiple icon from exe resources for MSW
      wxIconBundle bundle;
      bundle.AddIcon(wxIcon("MAIN", wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));
      bundle.AddIcon(wxIcon("MAIN", wxBITMAP_TYPE_ICO_RESOURCE, 32, 32));
      SetIcons(bundle);
    #endif // (__WXMSW__)
    #if defined(__WXGTK__) || defined(__WXMOTIF__)
      // Single icon from xpm for X
      wxIcon appIcon(icon_p2);
      SetIcon(appIcon);
    #endif // (__WXGTK__) || (__WXMOTIF__)

    UpdateTabs();
    UpdateInterface();
}

PasswordKeeperFrame::~PasswordKeeperFrame()
{
  //(*Destroy(PasswordKeeperFrame)
  //*)
  if (pnPanel)
    delete pnPanel;
}

void PasswordKeeperFrame::UpdateInterface()
{
  int tabsSelection = tbTabs->GetSelection();
  if (lbList != NULL)
  {
    if (tabsSelection > -1)
    {
      // Save selection and scrolling position before cleaning
      wxArrayInt listSelection;
      lbList->GetSelections(listSelection);
      int listFirstVisible = lbList->HitTest(1, 1);
      // Reread values from file
      CRecordList* f = CurrentList();
      f->Sort();
      wxArrayString items;
      for (size_t i = 0; i < f->GetCount(); ++i)
        items.Add(f->GetRecord(i).name);
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
        // Restore selection and scrolling position
        for (size_t i = 0; i < listSelection.size(); ++i)
        {
          int itemFound = lbList->FindString(olds[listSelection[i]]);
          if (itemFound > -1)
            lbList->SetSelection(itemFound);
        }
        if ((listFirstVisible < (int)lbList->GetCount()) && (listFirstVisible != wxNOT_FOUND))
          lbList->SetFirstItem(listFirstVisible);
      }
      else
      {
        for (size_t i = 0; i < listSelection.size(); ++i)
          lbList->SetSelection(listSelection[i]);
        if (listFirstVisible != wxNOT_FOUND)
          lbList->SetFirstItem(listFirstVisible);
      }
    }
  }
  UpdateMenus();
}

void PasswordKeeperFrame::UpdateMenus()
{
  int selCount;
  bool tabFlag = tbTabs->GetSelection() > -1;
  bool singleRecordFlag = tabFlag;
  bool multiRecordsFlag = tabFlag;
  singleRecordFlag &= CurrentLine(&selCount) != -1;
  multiRecordsFlag &= selCount > 0;
  // Account menu
  miSave->Enable(!account->IsSaved());
  // Tab menu
  miRenameTab->Enable(tabFlag);
  miDeleteTab->Enable(tabFlag);
  // Record menu
  miAdd->Enable(tabFlag);
  miView->Enable(singleRecordFlag);
  miEdit->Enable(singleRecordFlag);
  miDelete->Enable(multiRecordsFlag);
  miCopyName->Enable(singleRecordFlag);
  miCopyLogin->Enable(singleRecordFlag);
  miCopyEmail->Enable(singleRecordFlag);
  miCopyPass->Enable(singleRecordFlag);
  // Tab popup
  mpTabRename->Enable(tabFlag);
  mpTabDelete->Enable(tabFlag);
  // List popup
  mpView->Enable(singleRecordFlag);
  mpEdit->Enable(singleRecordFlag);
  mpDelete->Enable(multiRecordsFlag);
  mpCopyName->Enable(singleRecordFlag);
  mpCopyLogin->Enable(singleRecordFlag);
  mpCopyEmail->Enable(singleRecordFlag);
  mpCopyPass->Enable(singleRecordFlag);
  // Dynamic items for moving to different tabs
  ConstructMoveMenu(miMove, multiRecordsFlag);
  // And again for popup menu
  ConstructMoveMenu(mpMove, multiRecordsFlag);
}

void PasswordKeeperFrame::ConstructMoveMenu(wxMenu* menu, const bool enable)
{
  while (menu->GetMenuItemCount() > 0)
    menu->Destroy(menu->FindItemByPosition(0));
  for (size_t i = 0; i < account->GetContent()->GetCount(); ++i)
  {
    if ((int)i == tbTabs->GetSelection())
      continue;
    wxString itemName;
    itemName << "&" << i + 1 << " " << account->GetContent()->GetList(i)->GetName();
    int newId = wxNewId();
    wxMenuItem* tabItem = new wxMenuItem(menu, newId, itemName, "", wxITEM_NORMAL);
    menu->Append(tabItem);
    tabItem->Enable(enable);
    Connect(newId, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&PasswordKeeperFrame::OnMenuMoveSelected);
  }
  if (account->GetContent()->GetCount() > 1)
    menu->AppendSeparator();
  int newId = wxNewId();
  wxMenuItem* tabItem = new wxMenuItem(menu, newId, "New tab", "", wxITEM_NORMAL);
  menu->Append(tabItem);
  tabItem->Enable(enable);
  Connect(newId, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&PasswordKeeperFrame::OnMenuMoveSelected);
}

void PasswordKeeperFrame::UpdateTabs()
{
  while (tbTabs->GetPageCount() > 0)
    tbTabs->RemovePage(0);
  for (size_t i = 0; i < account->GetContent()->GetCount(); ++i)
    tbTabs->AddPage(GetTabPage(), account->GetContent()->GetList(i)->GetName(), false);
  if (tbTabs->GetPageCount() > 0)
    tbTabs->SetSelection(0);
  if (pnPanel)
  {
    pnPanel->Show(tbTabs->GetPageCount() > 0);
    pnPanel->SetFocus();
  }
}

wxWindow* PasswordKeeperFrame::GetTabPage()
{
  if (pnPanel == NULL)
  {
    // If hasn't created yet
    pnPanel = new wxPanel(tbTabs);
    pnPanel->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
    BoxSizer = new wxBoxSizer(wxHORIZONTAL);
    lbList = new wxListBox(pnPanel, ID_LIST, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_MULTIPLE, wxDefaultValidator, wxListBoxNameStr);
    BoxSizer->Add(lbList, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 8);
    pnPanel->SetSizer(BoxSizer);
    lbList->Bind(wxEVT_LEFT_DOWN, (wxObjectEventFunction)&PasswordKeeperFrame::OnListClick, this);
    lbList->Bind(wxEVT_LISTBOX_DCLICK, (wxObjectEventFunction)&PasswordKeeperFrame::OnListDblClick, this);
    lbList->Bind(wxEVT_RIGHT_UP, (wxObjectEventFunction)&PasswordKeeperFrame::OnListRightClick, this);
  }
  return pnPanel;
}

void PasswordKeeperFrame::Deauthorization()
{
  if (account->Deauthorize() != AC_ERROR_SUCCESS)
    wxMessageBox(account->GetErrorMessage(), "Error", wxOK | wxICON_ERROR);
}

void PasswordKeeperFrame::OntbTabsPageChanged(wxNotebookEvent& event)
{
  UpdateInterface();
}

void PasswordKeeperFrame::OnClose(wxCloseEvent& event)
{
  CSaver::Get().SaveWindow(*this, "Window");
  Deauthorization();
  event.Skip();
}

//////////////////////////////////////////////////////////////
// Tabs mouse event for dragging
//////////////////////////////////////////////////////////////

void PasswordKeeperFrame::OnMouseEvent(wxMouseEvent& event)
{
  if (event.LeftDown() && !isDragging)
  {
    // Hit test
    int tab = tbTabs->HitTest(event.GetPosition());
    if (tab > -1)
    {
      // Start dragging
      isDragging = true;
      dragStartPos = event.GetPosition();
      draggedTab = tab;
    }
  }
  else if (event.Dragging() && isDragging)
  {
    int onTab = tbTabs->HitTest(event.GetPosition());
    if ((onTab != draggedTab) && (onTab != -1))
    {
      // If we above another existing tab move dragging tab at the new place
      account->GetContent()->Move(draggedTab, onTab);
      account->SetSaved(false);
      UpdateTabs();
      draggedTab = onTab;
      tbTabs->SetSelection(draggedTab);
    }
  }
  else if (event.LeftUp() && isDragging)
    isDragging = false;
  event.Skip();
}

//////////////////////////////////////////////////////////////
// Tabs right-click event for popup
//////////////////////////////////////////////////////////////

void PasswordKeeperFrame::OnTabsRightUp(wxMouseEvent& event)
{
  int selection = tbTabs->HitTest(event.GetPosition());
  if (selection != -1)
    tbTabs->SetSelection();
  UpdateMenus();
  tbTabs->PopupMenu(&puTabMenu);
}

//////////////////////////////////////////////////////////////
// List handlers (clicks and popup)
//////////////////////////////////////////////////////////////

void PasswordKeeperFrame::OnListClick(wxMouseEvent& event)
{
  int selecting = lbList->HitTest(event.GetPosition());
  if (!event.ControlDown() && !event.ShiftDown())
    lbList->SetSelection(wxNOT_FOUND);
  if (!event.ShiftDown())
  {
    lastLineClicked = selecting;
    if (selecting != wxNOT_FOUND)
      lbList->SetSelection(selecting, !lbList->IsSelected(selecting));
  }
  else
    if ((lastLineClicked != wxNOT_FOUND) && (selecting != wxNOT_FOUND))
    {
      int i = lastLineClicked;
      while (i != selecting)
      {
        lbList->SetSelection(i, true);
        (lastLineClicked < selecting) ? ++i : --i;
      }
      lbList->SetSelection(selecting, true);
    }
  UpdateMenus();
}

void PasswordKeeperFrame::OnListDblClick(wxCommandEvent& event)
{
  int selection = CurrentLine();
  if (selection != wxNOT_FOUND)
    PutStringToClipboard(CurrentList()->GetRecord(selection).password, "Password");
}

void PasswordKeeperFrame::OnListRightClick(wxMouseEvent& event)
{
  int index = lbList->HitTest(event.GetPosition());
  if (index > wxNOT_FOUND)
  {
    if (event.ControlDown())
      lbList->SetSelection(index);
    if (!lbList->IsSelected(index))
    {
      lbList->SetSelection(wxNOT_FOUND);
      lbList->SetSelection(index);
    }
  }
  UpdateMenus();
  lbList->PopupMenu(&puListMenu);
}

//////////////////////////////////////////////////////////////
// Menu handlers
//
// "Account" menu
//////////////////////////////////////////////////////////////

void PasswordKeeperFrame::OnmiSwitchSelected(wxCommandEvent& event)
{
  Deauthorization();
  AuthDialog dlg(this);
  int res;
  do
  {
    res = dlg.ShowModal();
    if ((res == wxID_OK) || (res == wxID_NEW))
    {
      account->Authorize(dlg.edLogin->GetValue(), dlg.edPassword->GetValue(), res == wxID_NEW);
      if (account->IsOk())
      {
        SetLabel(account->GetLogin() + " - Password Keeper");
        UpdateTabs();
        UpdateInterface();
        return;
      }
      else
        wxMessageBox(account->GetErrorMessage(), "Error", wxOK | wxICON_ERROR);
    }
  }
  while ((res == wxID_OK) || (res == wxID_NEW));
}

void PasswordKeeperFrame::OnmiSyncSelected(wxCommandEvent& event)
{

}

void PasswordKeeperFrame::OnmiChangeSelected(wxCommandEvent& event)
{
  /*AuthDialog dlg(this);
  dlg.ChangeBehavior();
  int res;
  do
  {
    res = dlg.ShowModal();
    if (res == wxID_OK)
    {

    }
  }*/
}

void PasswordKeeperFrame::OnmiSaveSelected(wxCommandEvent& event)
{
  account->SaveContent();
  if (!account->IsOk())
    wxMessageBox(account->GetErrorMessage(), "Error", wxOK | wxICON_ERROR);
  UpdateMenus();
}

void PasswordKeeperFrame::OnmiQuitSelected(wxCommandEvent& event)
{
  Close();
}

//////////////////////////////////////////////////////////////
// "Tab" menu
//////////////////////////////////////////////////////////////

void PasswordKeeperFrame::OnmiAddTabSelected(wxCommandEvent& event)
{
  TabDialog dlg(this);
  wxString tabName;
  if (dlg.ShowModalEx(tabName, TabDialog::smADD) == wxID_OK)
  {
    account->GetContent()->Add(new CRecordList(tabName));
    UpdateTabs();
    tbTabs->SetSelection(tbTabs->GetPageCount() - 1);
    account->SetSaved(false);
    UpdateInterface();
  }
}

void PasswordKeeperFrame::OnmiRenameTabSelected(wxCommandEvent& event)
{
  TabDialog dlg(this);
  wxString tabName = CurrentList()->GetName();
  if (dlg.ShowModalEx(tabName, TabDialog::smRENAME) == wxID_OK)
  {
    CurrentList()->SetName(tabName);
    tbTabs->SetPageText(tbTabs->GetSelection(), tabName);
    account->SetSaved(false);
    UpdateInterface();
  }
}

void PasswordKeeperFrame::OnmiDeleteTabSelected(wxCommandEvent& event)
{
  if (wxMessageBox("Are you sure to delete this tab?\nAll records on this tab also will be deleted",
                    "Confirmation", wxYES_NO | wxICON_EXCLAMATION) == wxYES)
  {
    account->GetContent()->Delete(tbTabs->GetSelection());
    account->SetSaved(false);
    UpdateTabs();
    UpdateInterface();
  }
}

//////////////////////////////////////////////////////////////
// "Record" menu
//////////////////////////////////////////////////////////////

void PasswordKeeperFrame::OnmiViewSelected(wxCommandEvent& event)
{
  PropDialog dlg(this);
  dlg.ShowModalEx(CurrentList()->GetRecord(CurrentLine()), PropDialog::smVIEW);
}

void PasswordKeeperFrame::OnmiAddSelected(wxCommandEvent& event)
{
  PropDialog dlg(this);
  CRecord record;
  if (dlg.ShowModalEx(record, PropDialog::smADD) == wxID_OK)
  {
    CurrentList()->Add(record);
    account->SetSaved(false);
    UpdateInterface();
  }
}

void PasswordKeeperFrame::OnmiEditSelected(wxCommandEvent& event)
{
  PropDialog dlg(this);
  if (dlg.ShowModalEx(CurrentList()->GetRecord(CurrentLine()), PropDialog::smEDIT) == wxID_OK)
  {
    account->SetSaved(false);
    UpdateInterface();
  }
}

void PasswordKeeperFrame::OnMenuMoveSelected(wxCommandEvent& event)
{
  wxArrayInt selection;
  lbList->GetSelections(selection);
  CRecordList* currList = CurrentList();
  char num = mpMove->FindChildItem(event.GetId())->GetItemLabelText()[0];
  if (num == 'N')
  {
    OnmiAddTabSelected(event);
    num = account->GetContent()->GetCount() - 1;
  }
  else
    num -= '1';
  for (int i = selection.size() - 1; i > -1; --i)
  {
    account->GetContent()->GetList(num)->Add(currList->GetRecord(selection[i]));
    currList->Delete(selection[i]);
  }
  UpdateInterface();
}

void PasswordKeeperFrame::OnmiDeleteSelected(wxCommandEvent& event)
{
  if (wxMessageBox("Are you sure to delete this record(s)?", "Confirmation", wxYES_NO | wxICON_EXCLAMATION) == wxYES)
  {
    wxArrayInt selection;
    lbList->GetSelections(selection);
    for (int i = selection.size() - 1; i > -1; --i)
      CurrentList()->Delete(selection[i]);
    account->SetSaved(false);
    UpdateInterface();
  }
}

void PasswordKeeperFrame::OnmiCopyNameSelected(wxCommandEvent& event)
{
  PutStringToClipboard(CurrentList()->GetRecord(CurrentLine()).name, "Record's name");
}

void PasswordKeeperFrame::OnmiCopyLoginSelected(wxCommandEvent& event)
{
  PutStringToClipboard(CurrentList()->GetRecord(CurrentLine()).login, "Login");
}

void PasswordKeeperFrame::OnmiCopyEmailSelected(wxCommandEvent& event)
{
  PutStringToClipboard(CurrentList()->GetRecord(CurrentLine()).email, "Email");
}

void PasswordKeeperFrame::OnmiCopyPassSelected(wxCommandEvent& event)
{
  PutStringToClipboard(CurrentList()->GetRecord(CurrentLine()).password, "Password");
}

//////////////////////////////////////////////////////////////
// "Help" menu
//////////////////////////////////////////////////////////////

void PasswordKeeperFrame::OnmiAboutSelected(wxCommandEvent& event)
{
  wxString msg = wxbuildinfo(long_f);
  wxMessageBox(msg, _("Welcome to..."));
}
