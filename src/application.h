/*
 * application.h
 *
 *  Created on: May 01, 2012
 *      Author: Sebastian
 */

#ifndef Application_H_
#define Application_H_

#include "fsm.h"
#include "time.h"
#include "DriverInterface.h"
#include "application_config.h"


enum STATES
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
class APPLICATION : public FSM, public DriverInterface
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
	APPLICATION();
	~APPLICATION() {}
	void run();
	void init();
};

#endif /* APPLICATION_H_ */
