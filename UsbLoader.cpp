///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Aug 31 2019)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "UsbLoader.h" 

//////////////////////////////////////////////////////////////////////////

//HauptFenster::HauptFenster( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
HauptFenster::HauptFenster(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(-1, -1))
{
	fininish = false;
	connected = false;
	
	wxColour col1, col2;
	col1.Set(wxT("#4f5049"));
	col2.Set(wxT("#ededed"));

	this->SetMinSize(wxSize(500, 600));//   SetItemMinSize(this, 600, 600);

	//this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	//this->SetSizeHints(500, 510);
	
	m_panel = new wxPanel(this, -1);
	//m_panel->SetBackgroundColour(col1);

	wxBoxSizer* vbox1;
	vbox1 = new wxBoxSizer(wxVERTICAL);
	vbox1->SetMinSize(wxSize(600, 600));

	wxBoxSizer* hSizer1 = new wxBoxSizer(wxHORIZONTAL);
	m_staticText1 = new wxStaticText(m_panel, wxID_ANY, wxT("Eingabe: "));
	hSizer1->Add(m_staticText1, 0, wxRIGHT, 8);
	
	m_textCtrlEingabe = new wxTextCtrl(m_panel, wxID_ANY);
	hSizer1->Add(m_textCtrlEingabe, 1);

	vbox1->Add(hSizer1, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);
	vbox1->Add(-1,15);

	wxBoxSizer* hSizer4 = new wxBoxSizer(wxHORIZONTAL);
	m_staticText2 = new wxStaticText(m_panel, wxID_ANY, wxT("Ausgabe: "));
	hSizer4->Add(m_staticText2, 0, wxRIGHT, 8);
	//hSizer4->Add(0, 0, 1, wxEXPAND, 5);
	//m_checkBox1 = new wxCheckBox(m_panel, wxID_ANY, wxT("LED an"), wxDefaultPosition, wxDefaultSize, 0);
	//m_checkBox1->SetValue(true);
	//hSizer4->Add(m_checkBox1, 0, wxALL, 5);

	vbox1->Add(hSizer4, 0, wxLEFT | wxRIGHT, 10);
	
	vbox1->Add(-1, 10);

	wxBoxSizer* hSizer2 = new wxBoxSizer(wxHORIZONTAL);
	m_textCtrlAusgabe = new wxTextCtrl(m_panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1, -1), wxTE_LEFT|wxTE_MULTILINE);
	hSizer2->Add(m_textCtrlAusgabe, 1, wxEXPAND);

	vbox1->Add(hSizer2, 1, wxEXPAND | wxLEFT | wxRIGHT, 10);

	vbox1->Add(-1, 10);

	wxBoxSizer* hSizer5 = new wxBoxSizer(wxHORIZONTAL);
	m_checkBox1 = new wxCheckBox(m_panel, ID_CHECK_LED, wxT("LED an"), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);
	m_checkBox1->SetValue(true);
	hSizer5->Add(m_checkBox1, 0, wxRIGHT, 5);
	vbox1->Add(hSizer5, 0,  wxLEFT | wxRIGHT, 10);
	
	vbox1->Add(-1, 10);
	
	wxBoxSizer* hSizer3;
	hSizer3 = new wxBoxSizer(wxHORIZONTAL);

	m_buttonConnect = new wxButton(m_panel, ID_USBINT, wxT("Connect"), wxDefaultPosition, wxDefaultSize, 0 );
	hSizer3->Add(m_buttonConnect, 0, wxRIGHT | wxBOTTOM, 20);

	m_buttonEnde = new wxButton(m_panel, ID_EXIT, wxT("Ende"), wxDefaultPosition, wxDefaultSize, 0 );
	hSizer3->Add( m_buttonEnde, 0, wxLEFT | wxBOTTOM, 20 );

	vbox1->Add(hSizer3, 0, wxALIGN_RIGHT | wxRIGHT, 10);
	

	m_panel->SetSizer(vbox1);

	this->Layout();

	m_statusBar1 = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );

	this->Centre();
	
	Connect(ID_USBINT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(HauptFenster::trackUSBInterfaceClick));
	Connect(ID_EXIT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(HauptFenster::OnExitClick));
	Connect(ID_CHECK_LED, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(HauptFenster::LEDToggle));


	digiSpark = new USBDevice();


}

HauptFenster::~HauptFenster()
{
	delete digiSpark;
}


void HauptFenster::trackUSBInterfaceClick(wxCommandEvent& event)
{
	wxString m;
	int led;

	wxStreamToTextRedirector redirect(m_textCtrlAusgabe);
	fininish = false;
	connected = true;
	digiSpark->connect_device();
	cout << digiSpark->getLog();
	cout << digiSpark->print_deviceList();
	cout << digiSpark->getLog();
	led = digiSpark->getLED();
	if (led)
		m_checkBox1->SetValue(true);
	else
		m_checkBox1->SetValue(false);
	
	while (fininish == false) {
		cout << digiSpark->readString();
		cout << digiSpark->getLog();
		wxYield();
	}
	return;
}


void HauptFenster::OnExitClick(wxCommandEvent& event)
{
	fininish = true;
	Sleep(500);
	wxYield();
	Close(TRUE); // Tells the OS to quit running this process
}

void HauptFenster::LEDToggle(wxCommandEvent& event)
{
	if (event.IsChecked())
	{
		digiSpark->setLED_on();
	}
	else
	{
		digiSpark->setLED_off();
	}

}