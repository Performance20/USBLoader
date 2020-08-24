///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Aug 31 2019)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"


#include "wx/wx.h"
#include "wx/artprov.h"
#include "wx/xrc/xmlres.h"
#include "wx/string.h"
#include "wx/textctrl.h"
#include "wx/gdicmn.h"
#include "wx/font.h"
#include "wx/colour.h"
#include "wx/settings.h"
#include "wx/bitmap.h"
#include "wx/image.h"
#include "wx/icon.h"
#include "wx/button.h"
#include "wx/sizer.h"
#include "wx/wrapsizer.h"
#include "wx/statusbr.h"
#include "wx/frame.h"
#include "wx/panel.h"
//#include "wx/aui/aui.h"

#include "libusb.h"
#include "string.h"
#include <iostream>
#include "USBDevice.h"

using namespace std;

/*//////////////////////////////////////////////////////////////////////////
const int ID_USBINT = wxID_HIGHEST + 1;
const int ID_EXIT = wxID_HIGHEST + 2;
const int ID_CHECK_LED = wxID_HIGHEST + 3;
const int ID_TIMER = wxID_HIGHEST + 4;
const int ID_CLEANLOG = wxID_HIGHEST + 5;

*///////////////////////////////////////////////////////////////////////////////
/// Class HauptFenster
///////////////////////////////////////////////////////////////////////////////
class HauptFenster : public wxFrame
{
	enum
	{
		ID_USBINT = wxID_HIGHEST + 1,
		ID_EXIT,
		ID_CHECK_LED,
		ID_TIMER,
		ID_CLEANLOG
	};
	
private:
	bool fininish;
	bool connected;
	USBDevice* digiSpark;
	//wxAuiManager m_mgr;

	//wxDECLARE_EVENT_TABLE();

protected:
	wxPanel* m_panel;
	wxPanel* midPan;
	wxTextCtrl* m_textCtrlAusgabe;
	wxButton* m_buttonCleanlog;
	wxTextCtrl* m_textCtrlEingabe;
	wxStaticText* m_staticText1;
	wxStaticText* m_staticText2;
	wxButton* m_buttonConnect;
	wxButton* m_buttonEnde;
	wxStatusBar* m_statusBar1;
	wxCheckBox* m_checkBox1;
	wxTimer* m_Timer;
	wxGauge* m_Gauge;


public:

	//HauptFenster( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,500 ), long style = wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER|wxSYSTEM_MENU|wxTAB_TRAVERSAL );
	HauptFenster(const wxString& title);
	~HauptFenster();
	void trackUSBInterfaceClick(wxCommandEvent& event);
	void OnExitClick(wxCommandEvent& event);
	void LEDToggle(wxCommandEvent& event);
	void OnTimerEvent(wxTimerEvent& event);
	void CleanOutputLogClick(wxCommandEvent& event);
};

