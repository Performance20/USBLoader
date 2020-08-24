#include "main.h"
#include "UsbLoader.h"


bool USBLoaderApp::OnInit()
{
	
	HauptFenster* usbl = new HauptFenster(wxT("Zündungsmanager"));
		usbl->Show(true);

	return true;
}
