#include "UsbLoader.h" 


//HauptFenster::HauptFenster( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
HauptFenster::HauptFenster(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(-1, -1))
{
	fininish = false;
	connected = false;
	
	wxColour col1, col2;
	col1.Set(wxT("#4f5049"));
	col2.Set(wxT("#ededed"));

	this->SetMinSize(wxSize(600, 600));//   SetItemMinSize(this, 600, 600);

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

	m_buttonCleanlog = new wxButton(m_panel, ID_CLEANLOG, wxT("Leeren"), wxDefaultPosition, wxDefaultSize, 0);
	hSizer4->Add(m_buttonCleanlog, 0, wxLEFT , 420);
	
	//vbox1->Add(hSizer4, 0, wxALIGN_RIGHT | wxRIGHT, 10);
	vbox1->Add(hSizer4, 0,  wxLEFT | wxRIGHT, 10);
	//vbox1->Add(hSizer4, 0, wxALL, 5);

	vbox1->Add(-1, 10);

	wxBoxSizer* hSizer2 = new wxBoxSizer(wxHORIZONTAL);
	m_textCtrlAusgabe = new wxTextCtrl(m_panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1, -1), wxTE_LEFT|wxTE_MULTILINE);
	hSizer2->Add(m_textCtrlAusgabe, 1, wxEXPAND);

	vbox1->Add(hSizer2, 1, wxEXPAND | wxLEFT | wxRIGHT, 10);

	vbox1->Add(-1, 10);

	wxBoxSizer* hSizer5 = new wxBoxSizer(wxHORIZONTAL);
	m_checkBox1 = new wxCheckBox(m_panel, ID_CHECK_LED, wxT("LED an"), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);
	m_checkBox1->SetValue(false);
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

	m_statusBar1 = this->CreateStatusBar( 3, wxSTB_SIZEGRIP, wxID_ANY );

	m_Gauge = new wxGauge(m_statusBar1, wxID_ANY, 100, wxPoint(5, -1));
	m_Gauge->SetSize(150, 15);
	//m_Gauge->CenterOnParent(wxGA_HORIZONTAL | wxGA_SMOOTH | wxGA_VERTICAL);
	m_Gauge->CenterOnParent(wxGA_SMOOTH | wxGA_VERTICAL);

	m_Timer = new wxTimer(this, ID_TIMER);
	if (m_Timer->Start(300, false) == false)
	{
		wxMessageBox("Could not start Timer!");
	}
	
	this->SetStatusText("nicht verbunden", 2);

	this->Centre();
	
	this->Connect(ID_USBINT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(HauptFenster::trackUSBInterfaceClick));
	this->Connect(ID_EXIT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(HauptFenster::OnExitClick));
	this->Connect(ID_CHECK_LED, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(HauptFenster::LEDToggle));
	this->Connect(ID_CLEANLOG, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(HauptFenster::CleanOutputLogClick));
	this->Connect(ID_TIMER, wxEVT_TIMER, wxTimerEventHandler(HauptFenster::OnTimerEvent));

	digiSpark = new USBDevice();
}

HauptFenster::~HauptFenster()
{
	delete digiSpark;
	delete m_Timer;
}


void HauptFenster::trackUSBInterfaceClick(wxCommandEvent& event)
{
	wxString m, s;
	int led;

	wxStreamToTextRedirector redirect(m_textCtrlAusgabe);
	fininish = false;
	connected = true;
	digiSpark->connect_device();

	if (digiSpark->isConnected())
	{
		s << "verbunden";
		led = digiSpark->getLED();
		if (led)
			m_checkBox1->SetValue(true);
		else
			m_checkBox1->SetValue(false);
	}
	else
	{
		s << "nicht verbunden";
		cout << digiSpark->getLog();
		cout << digiSpark->print_deviceList();
		cout << digiSpark->getLog();
	}
	this->SetStatusText(s, 2);
	
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

void HauptFenster::OnTimerEvent(wxTimerEvent& event)
{
	if (m_Gauge)
	{
		int value = m_Gauge->GetValue() + 1;
		if (value == 100) value = 0;
		m_Gauge->SetValue(value);
		wxString s;
		s << value;
		this->SetStatusText(s, 1);
	}
}

void HauptFenster::CleanOutputLogClick(wxCommandEvent& event)
{
	this->m_textCtrlAusgabe->Clear();
}
