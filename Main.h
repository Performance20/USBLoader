#pragma once

#include "wx/wx.h"

class USBLoaderApp : public wxApp
{
public:
	//virtual bool OnInit();
	bool OnInit() wxOVERRIDE;
};

wxDECLARE_APP(USBLoaderApp);
wxIMPLEMENT_APP(USBLoaderApp);

