#pragma once

#include "libusb.h"
#include <string.h>
#include <iostream>
#include <sstream>

#include "protocol.h"

using namespace std;


// HID Class-Specific Requests values(section 7.2 of the HID specifications) 
#define HID_GET_REPORT                0x01 
#define HID_GET_IDLE                  0x02 
#define HID_GET_PROTOCOL              0x03 
#define HID_SET_REPORT                0x09 
#define HID_SET_IDLE                  0x0A 
#define HID_SET_PROTOCOL              0x0B 
#define HID_REPORT_TYPE_INPUT         0x01 
#define HID_REPORT_TYPE_OUTPUT        0x02 
#define HID_REPORT_TYPE_FEATURE       0x03 

#define CTRL_IN  LIBUSB_ENDPOINT_IN|LIBUSB_REQUEST_TYPE_CLASS|LIBUSB_RECIPIENT_INTERFACE 
#define CTRL_OUT LIBUSB_ENDPOINT_OUT|LIBUSB_REQUEST_TYPE_CLASS|LIBUSB_RECIPIENT_INTERFACE 

#define DIGISPARK_VENDOR_ID			0x16c0;
#define DIGISPARK_PRODUCT_ID		0x05df;
#define PACKET_CTRL_LEN 			1
#define TIMEOUT						1000 /* timeout in ms */


///////////////////////////////////////////////////////////////////////////////
/// Class USB Connection
///////////////////////////////////////////////////////////////////////////////
class USBDevice
{
private:

protected:
	bool fininish;
	uint16_t vendor_id; 
	uint16_t product_id; 
	bool connected;
	bool bibo_connected;
	bool USB_init;
	//std::ostream* out = NULL;
	//std::ostream& _cout = std::cout;
	std::ostringstream* out;
	libusb_device_handle* handle;  // handle DigiSpark
	std::string  readed;

	std::string print_device(libusb_device* dev, int level);
	int find_device(libusb_device* dev, int level, libusb_device_handle** handleusb, std::string& sout);
	int claim_device(libusb_device* dev, int level, libusb_device_handle** handleusb);
	const char* libusb_error_text(ssize_t err_number);
	int readUSBhid(const unsigned char* data);
	int readUSBVendor(const unsigned char* data);
	int writeUSB(const unsigned char* data);
	void init();
	int reset_device();


public:
	USBDevice(uint16_t _vendor_id, uint16_t _product_id);
	USBDevice(uint16_t _vendor_id, uint16_t _product_id, std::ostringstream& iout);
	USBDevice();
	~USBDevice();

	void connect_device();
	void connect_device_with_search();

	std::string print_deviceList();
	std::string print_connectedDevice();

	void writeln(std::string a);
	void write(std::string a);
	void writeByte(signed char b);
	std::string readString();
	std::string getLog();
	int setLED(int val);
	int setLED_on();
	int setLED_off();
	int getLED();
	bool isConnected();
};

inline bool USBDevice::isConnected()
{
	return connected;
}
