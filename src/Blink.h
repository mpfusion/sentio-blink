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


enum BLINK {
	initialState,
	ledOnState,
	ledOffState,
	_END
};
const int number_of_states = _END - initialState;


class Blink : public Statemachine, public DriverInterface
{
private:
	static STATUS_BLOCK myStatusBlock;

	static bool _initialState();
	static bool _ledOnState();
	static bool _ledOffState();

	static void _ODD_GPIO_InterruptHandler( uint32_t temp );

	static const time baseTime;
	static const time delayTime;

	static INTERRUPT_CONFIG rtcInterruptConfig;

public:
	Blink();
	~Blink(){}

	ERROR_CODE executeApplication();
	uint8_t    setupApplication();
};

#endif /* BLINK_H_ */
