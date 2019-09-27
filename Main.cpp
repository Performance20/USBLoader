#include "main.h"
#include "UsbLoader.h"


IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	
	HauptFenster* usbl = new HauptFenster(wxT("USBLoader"));
		usbl->Show(true);

	return true;
}
