#include "main.h"
#include "UsbLoader.h"


IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	//Simple* simple = new Simple(wxT("Simple"));
	//simple->Show(true);

	//Listbox* listbox = new Listbox(wxT("Listbox"));
	//listbox->Show(true);

	HauptFenster* usbl = new HauptFenster(wxT("USBLoader"));
		usbl->Show(true);

	return true;
}
