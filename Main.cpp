#include "main.h"
#include "UsbLoader.h"


bool USBLoaderApp::OnInit()
{
	
	HauptFenster* usbl = new HauptFenster(wxT("Z�ndungsmanager"));
		usbl->Show(true);

	return true;
}
