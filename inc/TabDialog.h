#ifndef TABDIALOG_H
#define TABDIALOG_H

//(*Headers(TabDialog)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/dialog.h>
//*)

class TabDialog: public wxDialog
{
	public:

    enum ShowMode
    {
      smADD = 0,
      smRENAME
    };

		TabDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~TabDialog();

		const int ShowModalEx(wxString& name, const ShowMode mode);

		//(*Declarations(TabDialog)
		wxTextCtrl* edName;
		//*)

	protected:

		//(*Identifiers(TabDialog)
		static const long ID_TEXTCTRL1;
		//*)

	private:

		//(*Handlers(TabDialog)
		void OnedNameTextEnter(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
