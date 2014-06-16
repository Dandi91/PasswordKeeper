/***************************************************************
 * Name:      PasswordKeeperMain.h
 * Purpose:   Defines Application Frame
 * Author:    Dandi ()
 * Created:   2014-05-28
 * Copyright: Dandi ()
 * License:
 **************************************************************/

#ifndef PASSWORDKEEPERMAIN_H
#define PASSWORDKEEPERMAIN_H

//(*Headers(PasswordKeeperFrame)
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
//*)

#include "Account.h"
#include <wx/listbox.h>
#include <wx/panel.h>

class PasswordKeeperFrame: public wxFrame
{
    public:

        PasswordKeeperFrame(wxWindow* parent, wxWindowID id = -1);
        virtual ~PasswordKeeperFrame();

    private:
        CAccount* account;

        bool isDragging;
        wxPoint dragStartPos;
        int draggedTab;

        int lastLineClicked;

        wxPanel* pnPanel;
        wxBoxSizer* BoxSizer;
        wxListBox* lbList;
        wxWindow* GetTabPage();

        void Deauthorization();
        void UpdateInterface();
        void UpdateMenus();
        void ConstructMoveMenu(wxMenu* menu, const bool enable);
        void UpdateTabs();
        CRecordList* CurrentList();
        const int CurrentLine(int* selCount);

        //(*Handlers(PasswordKeeperFrame)
        void OnmiSwitchSelected(wxCommandEvent& event);
        void OnmiSyncSelected(wxCommandEvent& event);
        void OnmiSaveSelected(wxCommandEvent& event);
        void OnmiQuitSelected(wxCommandEvent& event);
        void OnmiAddTabSelected(wxCommandEvent& event);
        void OnmiRenameTabSelected(wxCommandEvent& event);
        void OnmiDeleteTabSelected(wxCommandEvent& event);
        void OntbTabsPageChanged(wxNotebookEvent& event);
        void OnmiAboutSelected(wxCommandEvent& event);
        void OnmiViewSelected(wxCommandEvent& event);
        void OnmiAddSelected(wxCommandEvent& event);
        void OnmiEditSelected(wxCommandEvent& event);
        void OnmiDeleteSelected(wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnmiCopyNameSelected(wxCommandEvent& event);
        void OnmiCopyLoginSelected(wxCommandEvent& event);
        void OnmiCopyEmailSelected(wxCommandEvent& event);
        void OnmiCopyPassSelected(wxCommandEvent& event);
        void OnmiChangeSelected(wxCommandEvent& event);
        //*)

        void OnListClick(wxMouseEvent& event);
        void OnListDblClick(wxCommandEvent& event);
        void OnListRightClick(wxMouseEvent& event);
        void OnMouseEvent(wxMouseEvent& event);
        void OnTabsRightUp(wxMouseEvent& event);
        void OnMenuMoveSelected(wxCommandEvent& event);

        //(*Identifiers(PasswordKeeperFrame)
        static const long ID_NOTEBOOK;
        static const long idMenuSwitch;
        static const long idMenuSynchronize;
        static const long idMenuChange;
        static const long idMenuSave;
        static const long idMenuQuit;
        static const long idMenuTabAdd;
        static const long idMenuTabRename;
        static const long idMenuTabDelete;
        static const long idMenuView;
        static const long idMenuAdd;
        static const long idMenuEdit;
        static const long ID_MENUITEM2;
        static const long idMenuMove;
        static const long idMenuDelete;
        static const long idMenuCopyName;
        static const long idMenuCopyLogin;
        static const long idMenuCopyEmail;
        static const long idMenuCopyPass;
        static const long idMenuAbout;
        static const long ID_STATUSBAR;
        static const long idMenuViewP;
        static const long idMenuAddP;
        static const long idMenuEditP;
        static const long ID_MENUITEM1;
        static const long idMenuMoveP;
        static const long idMenuDeleteP;
        static const long idMenuCopyNameP;
        static const long idMenuCopyLoginP;
        static const long idMenuCopyEmailP;
        static const long idMenuCopyPassP;
        static const long idTabAddP;
        static const long idTabRenameP;
        static const long idTabDeleteP;
        //*)

        static const long ID_LIST;

        //(*Declarations(PasswordKeeperFrame)
        wxMenuItem* miAddTab;
        wxMenuItem* miRenameTab;
        wxMenuItem* mpCopyPass;
        wxMenuItem* mpTabAdd;
        wxMenu* mpMove;
        wxMenuItem* mpCopyEmail;
        wxMenuItem* miDeleteTab;
        wxMenuItem* miSave;
        wxMenuItem* miEdit;
        wxMenuItem* MenuItem2;
        wxMenuItem* mpAdd;
        wxMenuItem* MenuItem1;
        wxMenuItem* mpEdit;
        wxMenu* miMove;
        wxMenuItem* miView;
        wxMenu puTabMenu;
        wxMenuItem* miAdd;
        wxStatusBar* sbStatus;
        wxMenuItem* mpTabDelete;
        wxMenuItem* miSync;
        wxMenuItem* mpDelete;
        wxMenuItem* miDelete;
        wxMenuItem* miCopyPass;
        wxMenuItem* miCopyLogin;
        wxMenuItem* miCopyName;
        wxMenuItem* mpCopyLogin;
        wxMenuItem* mpTabRename;
        wxMenuItem* mpView;
        wxMenuItem* mpCopyName;
        wxMenuItem* miCopyEmail;
        wxMenu puListMenu;
        wxMenu* miRecord;
        wxMenuItem* miChange;
        wxMenuItem* miSwitch;
        wxNotebook* tbTabs;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // PASSWORDKEEPERMAIN_H
