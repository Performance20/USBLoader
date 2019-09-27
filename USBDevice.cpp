
#include "USBDevice.h"


	USBDevice::USBDevice(uint16_t _vendor_id, uint16_t _product_id)
	{
		vendor_id = _vendor_id;  
		product_id = _product_id; 
		out = new std::ostringstream();
		this->init();
	}

    USBDevice::USBDevice(uint16_t _vendor_id, uint16_t _product_id, std::ostringstream& iout)
    {
		vendor_id = _vendor_id; // 
		product_id = _product_id; // 
		out = &iout;
		this->init();
    }

	USBDevice::USBDevice(void)
	{
		vendor_id = DIGISPARK_VENDOR_ID; // 
		product_id = DIGISPARK_PRODUCT_ID; // 
		out = new std::ostringstream();
		//out = new std::ostream("");
		this->init(); 
	}

    USBDevice::~USBDevice()
    {
		if (connected == true) {
			libusb_release_interface(handle, 0);
			libusb_close(handle);
		}
		if (bibo_connected == true)
			libusb_exit(NULL);
    }


	void USBDevice::init() {
		int ret;

		connected = false;
		handle = NULL;

		ret = libusb_init(NULL);
		if (ret < 0)
		{
			*out << "libusb_init(NULL) fehlgeschlagen" << endl << ends;
			bibo_connected = false;
			return;
		}
		bibo_connected = true;

	}

	void USBDevice::connect_device() {
		int ret;
		libusb_device_handle* _handle = NULL;

		if (connected == false)
		{
			_handle = libusb_open_device_with_vid_pid(NULL, vendor_id, product_id);
			if (_handle == NULL) {  // have not connected my DigiSpark
				*out << "could not open device via libusb_open_device_with_vid_pid(...." << endl << ends;
				return;
			}
			ret = libusb_claim_interface(_handle, 0);
			if (ret < 0) {
				*out << "libusb_claim_interface error: " << libusb_error_text(ret) << endl << ends;
				libusb_close(_handle);
				return;
			}
			handle = _handle;
			connected = true;
		}
		else
			*out << "device is already connected" << endl << ends;
		return;
	}

	void USBDevice::connect_device_with_search() {
		
		libusb_device** devs;
		libusb_device_handle* _handle;  // handle DigiSpark
		ssize_t cnt;
		int i;

		cnt = libusb_get_device_list(NULL, &devs);
		if (cnt < 0)
		{
			*out << libusb_error_text(cnt) << " - call libusb_get_device_list(.." << endl << ends;
			return;
		}
		else
			*out << "Number of existing USB devices: " << cnt << endl;

		for (i = 0; devs[i]; ++i) {
			if (this->claim_device(devs[i], 0, &_handle) == 1) { //found my DigiSpark
				handle = _handle;
				connected = true;
				break;
			}
		}
		libusb_free_device_list(devs, 1);
	}


std::string USBDevice::print_device(libusb_device* dev, int level)
{
	struct libusb_device_descriptor desc;
	libusb_device_handle* _handle = NULL;
	unsigned char strBuffer[256];
	int ret;
	unsigned int i, ii;
	std::ostringstream  m, m1, mout;
	std::string ss("");


	ret = libusb_get_device_descriptor(dev, &desc);
	if (ret < 0) {
		*out << "failed to get device descriptor" << endl << ends;
		return "";
	}
	ret = libusb_open(dev, &_handle);
	if (ret == LIBUSB_SUCCESS)
	{
		if (desc.iManufacturer)
		{
			ret = libusb_get_string_descriptor_ascii(_handle, desc.iManufacturer, strBuffer, sizeof(strBuffer));
			if (ret > 0) {
				strBuffer[ret] = 0;
				m1 << "  idVendor: " << strBuffer;
			}
			else
				m1 << "  idVendor: " << desc.idVendor;
		}
		else
			m1 << "  idVendor: " << desc.idVendor;

		if (desc.iProduct) {
			ret = libusb_get_string_descriptor_ascii(_handle, desc.iProduct, strBuffer, sizeof(strBuffer));
			if (ret > 0) {
				strBuffer[ret] = 0;
				m1 << "  idProduct: " << strBuffer;
			}
			else
				m1 << "  idProduct: " << desc.idProduct;
		}
		else
			m1 << "  idProduct: " << desc.idProduct;
	}
	else 
	{
			m1 << "idVend: " << desc.idVendor << "  idProduct: " << desc.idProduct;
	}

	i = libusb_get_bus_number(dev);
	ii = libusb_get_device_address(dev);
	m1 << ends;
	mout << level * 2 << ".*sDev (bus " << i << ", device " << ii << "): " << m1.str() << "\n";
	if (_handle) {
			if (desc.iSerialNumber) {
				ret = libusb_get_string_descriptor_ascii(_handle, desc.iSerialNumber, strBuffer, sizeof(strBuffer));
					if (ret > 0)
						mout << "%" << level * 2 << "  - Serial Number: " << strBuffer << endl;
			}
	}

			/*    needs further seaching work to get more details */
			/*	for (i = 0; i < desc.bNumConfigurations; i++) {
					struct libusb_config_descriptor* config;
					ret = libusb_get_config_descriptor(dev, i, &config);
					if (LIBUSB_SUCCESS != ret) {

						out << "  Couldn't retrieve descriptors" << endl;
						continue;
					}

					//print_configuration(config);

					libusb_free_config_descriptor(config);
				}

				if (_handle && desc.bcdUSB >= 0x0201) {
					//print_bos(_handle);
			} */

	if (_handle)
			libusb_close(_handle);
	mout << ends;	
	return mout.str();
}

		/* find the device and claim the interface */
int USBDevice::find_device(libusb_device * dev, int level, libusb_device_handle** handleusb, std::string& sout)
{
	struct libusb_device_descriptor desc;
	unsigned char strBuffer[256];
	int ret;
	unsigned int i, ii;
	std::ostringstream m, m1, m2;
	std::istringstream in;
	long _vendor_id, _product_id;
	libusb_device_handle* _handle;

	ret = libusb_get_device_descriptor(dev, &desc);
	if (ret < 0) {
		*out << "failed to get device descriptor" << endl << ends;
		sout = "";
		return -1;
	}

	_handle = NULL;
	ret = libusb_open(dev, &_handle);
	if (ret == LIBUSB_SUCCESS) {
		if (desc.iManufacturer) {
			ret = libusb_get_string_descriptor_ascii(_handle, desc.iManufacturer, strBuffer, sizeof(strBuffer));
			if (ret > 0) {
					strBuffer[ret] = 0;
					m1 << strBuffer;
			}
			else
					m1 << desc.idVendor;
		}
		else
			m1 << desc.idVendor;

		if (desc.iProduct) {
			ret = libusb_get_string_descriptor_ascii(_handle, desc.iProduct, strBuffer, sizeof(strBuffer));
			if (ret > 0) {
					strBuffer[ret] = 0;
					m2 << strBuffer;
			}
			else
					m2 << desc.idProduct;
		}
		else
			m2 << desc.idProduct;
	}
	else {
		m1 << desc.idVendor;
		m2 << desc.idProduct;
	}
	m1 << ends;
	m2 << ends;
	in.str(m1.str());
	in >> _vendor_id;
	in.str(m2.str());
	in >> _product_id;

	if ((_vendor_id == vendor_id) && (_product_id == product_id)) {
		i = libusb_get_bus_number(dev);
		ii = libusb_get_device_address(dev);
		m << "Device founded:  " << level * 2 << ".*sDev (bus " << i << ", device " << ii << "): " << m1.str() << ":" << m2.str() << "\n" << ends;
				//		out << m.str();
		*handleusb = _handle;
		sout = m.str();
		return 1;
	}
	if (_handle)
		libusb_close(_handle);
	sout = "";
	return 0;
}

int USBDevice::claim_device(libusb_device* dev, int level, libusb_device_handle** handleusb)
{
	int ret;
	std::string sout;

	if (find_device(dev, level, handleusb, sout) == 1)
	{
		ret = libusb_claim_interface(*handleusb, 0);
		if (ret < 0) {
			*out << "libusb_claim_interface error: " << libusb_error_text(ret) << endl << ends;
			libusb_close(*handleusb);
			*handleusb = NULL;
			return -1;
		}
		else
			return 1;
	}
	return 0;
}


int  USBDevice::readUSBhid(const unsigned char* data)
{
			int status;

			if (connected == true) {
				//status = libusb_control_transfer(device, (0x01 << 5) | 0x80, 0x01, 0, 0, (unsigned char*) data, 1, 1000);
				status = libusb_control_transfer(handle, CTRL_IN, HID_GET_REPORT, (HID_REPORT_TYPE_FEATURE << 8) | 0x00,
					0, (unsigned char*)data, PACKET_CTRL_LEN, TIMEOUT);

				if (status < 0) {
					*out << endl << libusb_error_name(status) << endl << ends;
					reset_device();
					return 0;
				}
				else
					return status;
			}
			return 0;
		}

int  USBDevice::readUSBVendor(const unsigned char* data)
{
	int status;

	if (connected == true) {
		//status = libusb_control_transfer(device, (0x01 << 5) | 0x80, 0x01, 0, 0, (unsigned char*) data, 1, 1000);
		status = libusb_control_transfer(handle, LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_IN, 
			REQ_LOGGING, 0, 0, (unsigned char*) data, 1, TIMEOUT);

		if (status < 0) {
			*out << endl << libusb_error_name(status) << endl << ends;
			reset_device();
			return 0;
		}
		else
			return status;
	}
	return 0;
}


int USBDevice::USBDevice::setLED(int val)
{
	int status;
	unsigned char data[1];

	if (connected == true) {
		//status = libusb_control_transfer(device, (0x01 << 5) | 0x80, 0x01, 0, 0, (unsigned char*) data, 1, 1000);
		status = libusb_control_transfer(handle, LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_OUT,
			REQ_ONBOARD_LED_SET, val, 0, data, 1, TIMEOUT);

		if (status < 0) {
			*out << endl << libusb_error_name(status) << endl << ends;
			reset_device();
			return status;
		}
		else
			return status;
	}
	return -1;
}

int USBDevice::setLED_off(void)
{
	return setLED(VAL_ONBOARD_LED_OFF);
}

int USBDevice::getLED()
{
	int status, ret =0;
	unsigned char data[1] , c;

	if (connected == true) {
		//status = libusb_control_transfer(device, (0x01 << 5) | 0x80, 0x01, 0, 0, (unsigned char*) data, 1, 1000);
		status = libusb_control_transfer(handle, LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_IN,
			REQ_ONBOARD_LED_STATUS, 0, 0, data, 1, TIMEOUT);

		if (status < 0) {
			*out << endl << libusb_error_name(status) << endl << ends;
			reset_device();
			return -1;
		}
		else
		{
			ret = ret + data[0];
			return ret;
		}
	}
	return -1;
}

int USBDevice::setLED_on(void)
{
	return setLED(VAL_ONBOARD_LED_ON);
}

int  USBDevice::writeUSB(const unsigned char* data)
{
	int status;

	if (connected == true) {
	//status = libusb_control_transfer(this->handle, (0x01 << 5), 0x09, 0, 0, (unsigned char*) data, 0, 1000);
		status = libusb_control_transfer(handle, CTRL_OUT, HID_SET_REPORT, (HID_REPORT_TYPE_FEATURE << 8) | 0x00,
					0, (unsigned char*)data, PACKET_CTRL_LEN, TIMEOUT);

		if (status < 0) {
			*out << endl << libusb_error_name(status) << endl << ends;
			reset_device();
			return status;
		}
	}
	return 0;
}


		const char* USBDevice::libusb_error_text(ssize_t err_number) {
			switch (err_number) {
			case 0:
				return "LIBUSB_SUCCESS";
			case -1:
				return "LIBUSB_ERROR_IO";
			case -2:
				return "LIBUSB_ERROR_INVALID_PARAM";
			case -3:
				return "LIBUSB_ERROR_ACCESS";
			case -4:
				return "LIBUSB_ERROR_NO_DEVICE";
			case -5:
				return "LIBUSB_ERROR_NOT_FOUND";
			case -6:
				return "LIBUSB_ERROR_BUSY";
			case -7:
				return "LIBUSB_ERROR_TIMEOUT";
			case -8:
				return "LIBUSB_ERROR_OVERFLOW";
			case -9:
				return "LIBUSB_ERROR_PIPE";
			case -10:
				return "LIBUSB_ERROR_INTERRUPTED";
			case -11:
				return "LIBUSB_ERROR_NO_MEM";
			case -12:
				return "LIBUSB_ERROR_NOT_SUPPORTED";
			case -99:
				return "LIBUSB_ERROR_OTHER";
			}
			return "UNDEFINED ERROR";
		}

int USBDevice::reset_device()
{
	if (connected == true) {
		libusb_release_interface(handle, 0);
		libusb_close(handle);
		connected = false;
	}
	return 1;
}

std::string USBDevice::print_deviceList()
{
			libusb_device** devs;
			ssize_t cnt;
			int i;
			std::string ss;

			cnt = libusb_get_device_list(NULL, &devs);
			if (cnt < 0) {
				*out << libusb_error_text(cnt) << " - call libusb_get_device_list(.." << endl << ends;
				return "";
			}
			else
				*out << "Number of existing USB devices: " << cnt << endl;
			for (i = 0; devs[i]; ++i)
				ss += this->print_device(devs[i], 0);
			libusb_free_device_list(devs, 1);
			return ss;
}

void USBDevice::writeln(std::string a) {

}


void USBDevice::writeByte(signed char b) {

}

void write(std::string a) {

}

std::string USBDevice::readString() { // blocking reading
			
	unsigned char c;
	std::string ss("");
	bool loopcnt = true;

	if (connected == true)
	{
		while (loopcnt == true) {
			if (this->readUSBVendor(&c))
			{
				switch (c) {
				case 0:		loopcnt = false; break;
				default:	ss += c;
				};

			}
			else
				loopcnt = false;
		}
	}
	return ss;
}

		
std::string USBDevice::getLog() {
	
	std::string ss;

	ss = out->str();
	out->str("");
	out->clear();
	return ss;
		}

std::string USBDevice::print_connectedDevice() {
	libusb_device** devs;
	libusb_device_handle* _handle;

	ssize_t cnt;
	int i;
	std::string ss = "";

	cnt = libusb_get_device_list(NULL, &devs);
	if (cnt < 0) {
		*out << libusb_error_text(cnt) << " - call libusb_get_device_list(.." << endl << ends;
		return "";
	}

	for (i = 0; devs[i]; i++) {
		if (find_device(devs[i], 0, &_handle, ss) == 1) break;
	}
	libusb_free_device_list(devs, 1);
	return ss;
}


