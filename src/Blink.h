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
#include "ApplicationConfig.h"


enum BLINK
{
	initialState,
	ledOnState,
	ledOffState
};


/**
 * Main class of the program.
 *
 * This is the main class of the application. It implements all
 * necessary functionality and the states.
 */
class Blink : public Statemachine, public DriverInterface
{
private:
	static STATUS_BLOCK myStatusBlock; ///< responsible for the state information

	/**
	 * Initialises the hardware.
	 *
	 * Initialises the used hardware components like timers and temperature
	 * sensors. At the end of this function the orange LED is switched on.
	 *
	 * @return The return value is always `true,` since a return value of
	 * `false` would stop this state machine.
	 */
	static bool _initialState();

	/**
	 * Switches the LED on.
	 *
	 * @return The return value is always `true,` since a return value of
	 * `false` would stop this state machine.
	 */
	static bool _ledOnState();

	/**
	 * Switches the LED off.
	 *
	 * @return The return value is always `true,` since a return value of
	 * `false` would stop this state machine.
	 */
	static bool _ledOffState();

	/**
	 * Interrupt handler used for changing states after wakeup.
	 *
	 * @param This integer serves as a bit array and represents the GPIO ports
	 * that can throw an interrupt. The exact port can be determined when a
	 * bit mask is checked against this variable.
	 */
	static void _ODD_GPIO_InterruptHandler( uint32_t temp );

	static const time baseTime;  ///< controls the starting value of the timer
	static const time delayTime; ///< controls the sleep duration

	static INTERRUPT_CONFIG rtcInterruptConfig;

public:
	Blink();
	~Blink() {}

	ERROR_CODE executeApplication();
	uint8_t    setupApplication();
};

#endif /* BLINK_H_ */
