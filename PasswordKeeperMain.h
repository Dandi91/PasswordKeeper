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
#include <wx/listbox.h>
#include <wx/panel.h>

class PasswordKeeperFrame: public wxFrame
{
    public:

        PasswordKeeperFrame(wxWindow* parent, wxWindowID id = -1);
        virtual ~PasswordKeeperFrame();

    private:

        wxPanel* pnPanel;
        wxBoxSizer* BoxSizer;
        wxListBox* lbList;
        wxWindow* GetTabPage();
        void UpdateInterface();

        //(*Handlers(PasswordKeeperFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnmiNewSelected(wxCommandEvent& event);
        void OntbTabsPageChanged(wxNotebookEvent& event);
        void OnmiOpenSelected(wxCommandEvent& event);
        void OnmiSaveSelected(wxCommandEvent& event);
        void OnmiSaveAsSelected(wxCommandEvent& event);
        void OnmiCloseSelected(wxCommandEvent& event);
        void OnmiCloseAllSelected(wxCommandEvent& event);
        void OnmiViewSelected(wxCommandEvent& event);
        void OnmiAddSelected(wxCommandEvent& event);
        void OnmiEditSelected(wxCommandEvent& event);
        void OnmiDeleteSelected(wxCommandEvent& event);
        void OnmiMergeSelected(wxCommandEvent& event);
        //*)

        void OnListClick(wxCommandEvent& event);
        void OnListDblClick(wxCommandEvent& event);
        void OnListRightClick(wxMouseEvent& event);

        //(*Identifiers(PasswordKeeperFrame)
        static const long ID_NOTEBOOK;
        static const long idMenuNew;
        static const long idMenuOpen;
        static const long idMenuSave;
        static const long idMenuSaveAs;
        static const long idMenuMerge;
        static const long idMenuClose;
        static const long idMenuCloseAll;
        static const long idMenuQuit;
        static const long idMenuView;
        static const long idMenuAdd;
        static const long idMenuEdit;
        static const long idMenuDelete;
        static const long idMenuAbout;
        static const long ID_STATUSBAR;
        static const long idMenuViewP;
        static const long idMenuAddP;
        static const long idMenuEditP;
        static const long idMenuDeleteP;
        //*)

        static const long ID_LIST;

        //(*Declarations(PasswordKeeperFrame)
        wxMenu puMenu;
        wxMenuItem* miOpen;
        wxMenuItem* miSave;
        wxMenuItem* miEdit;
        wxMenuItem* mpAdd;
        wxMenuItem* mpEdit;
        wxMenuItem* miView;
        wxMenuItem* miAdd;
        wxMenuItem* miCloseAll;
        wxStatusBar* sbStatus;
        wxMenuItem* mpDelete;
        wxMenuItem* miDelete;
        wxMenuItem* miMerge;
        wxBoxSizer* BoxSizer1;
        wxMenuItem* miClose;
        wxMenuItem* miNew;
        wxMenuItem* mpView;
        wxMenuItem* miSaveAs;
        wxMenu* miRecord;
        wxNotebook* tbTabs;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // PASSWORDKEEPERMAIN_H
