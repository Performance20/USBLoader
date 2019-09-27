#pragma once

#ifndef PROTOCOL_H_
#define PROTOCOL_H_


#define REQ_LOGGING				1  // log text messages to the log window
#define REQ_ONBOARD_LED_SET		2
#define REQ_ONBOARD_LED_STATUS	3

#define VAL_ONBOARD_LED_ON		1
#define VAL_ONBOARD_LED_OFF		2

#define DATA_SIZE_IN_BYTE		10  // 1th byte define type if needed

#endif /* PROTOCOL_H_ */