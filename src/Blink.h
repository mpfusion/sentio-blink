/*
 * Blink.h
 *
 *  Created on: May 01, 2012
 *      Author: Sebastian
 */

#ifndef BLINK_H_
#define BLINK_H_

#include "Statemachine.h"
#include "DriverInterface.h"

#include "time.h"


typedef enum{
	initialState = 0,
	ledOnState   = 1,
	ledOffState  = 2
}BLINK;

class Blink : public Statemachine, public DriverInterface
{
private:
	static STATUS_BLOCK myStatusBlock;

	// States in statemachine
	static bool _initialState();
	static bool _ledOnState();
	static bool _ledOffState();

	// Statemachine ISRs
	static void _ODD_GPIO_InterruptHandler( uint32_t temp );

	static time baseTime;
	static time delayTime;

	static INTERRUPT_CONFIG rtcInterruptConfig;
	static OUTPUT_32KHZ     rtcOutputConfig;

public:
	Blink();
	~Blink(){}

	ERROR_CODE executeApplication();
	uint8_t    setupApplication();
};

#endif /* BLINK_H_ */
