/*
 * Blink.cpp
 *
 *  Created on: May, 2012
 *      Author: Sebastian
 */

#include "Blink.h"

// redundant, automatically included by the framework
//#include "efm32_gpio.h"
//#include "efm32_emu.h"

// Status block variable; Needed for all state-machine applications
STATUS_BLOCK Blink::myStatusBlock;

// RTC configuration variables
INTERRUPT_CONFIG Blink::rtcInterruptConfig;
OUTPUT_32KHZ     Blink::rtcOutputConfig;

// Time objects
time Blink::baseTime(0);
time Blink::delayTime(1);


Blink::Blink()
{
	// State-machine settings
	myStatusBlock.applicationID       = 0;
	myStatusBlock.initialState        = initialState;
	myStatusBlock.recallState         = initialState;
	myStatusBlock.numberOfISR         = 1;
	myStatusBlock.numberOfStates      = 3;
	myStatusBlock.recallOption        = useInitialState;
	myStatusBlock.restoreClockSetting = true;
	myStatusBlock.wantToSleep         = true;
	myStatusBlock.sleepMode           = 1;

	// RTC settings
	rtcInterruptConfig.enableAlarm1           = true;
	rtcInterruptConfig.enableAlarm2           = false;
	rtcInterruptConfig.enableBatteryBackedSQW = true; // Keep outputting SQW in low-power mode
	rtcInterruptConfig.interruptControl       = true; // Output SQW or enable Interrupt pin
	rtcInterruptConfig.squareWaveFrequency    = square_1Hz;

	rtcOutputConfig.enable32kHz              = false;
	rtcOutputConfig.enableBatteryBacked32kHz = false;

	// State-machine states
	stateDefinition[initialState] = _initialState;
	stateDefinition[ledOnState]   = _ledOnState;
	stateDefinition[ledOffState]  = _ledOffState;

	// Interrupt settings
	ISR_Definition[0].function        = _ODD_GPIO_InterruptHandler;
	ISR_Definition[0].interruptNumber = GPIO_ODD_IRQn;
	ISR_Definition[0].anchorISR       = false; // Use ISR for all SM
}


ERROR_CODE Blink::executeApplication()
{
	return startApplication( &myStatusBlock );
}


uint8_t Blink::setupApplication()
{
	return initializeApplication( &myStatusBlock );
}


/*
 * STATE IMPLEMENTATIONS
 */

/*
 * initialState
 *
 * @brief	platform/peripheral initialization
 */
bool Blink::_initialState()
{
	debug.initializeInterface( numberOfDebugPinsUsed, numberOfButtonsUsed );
	debug.initializeDebugUart();

	timer.initializeInterface();
	timer.setBaseTime( baseTime );
	timer.setInterruptConfig( rtcInterruptConfig );
	timer.set32kHzOutputConfig( rtcOutputConfig );
	timer.initializeMCU_Interrupt();
	timer.setBatteryBackedTempConv( true );
	timer.setTempConvRate( seconds_512 );

	timer.resetInterrupts();
	timer.setLowPowerMode();

	sentio.LED_SetOrange();
	debug.printLine("Welcome to FSM-Blink", true);

	// Go directly to next state
	myStatusBlock.wantToSleep = false;
	myStatusBlock.nextState = ledOnState;
	GPIO->P[gpioPortA].DOUTSET = 1 << 1;
	return true;
}

/*
 * ledOnState
 *
 * @brief	Turns the LED on
 */
bool Blink::_ledOnState()
{
	GPIO->P[gpioPortA].DOUTSET = 1 << 0;
	GPIO->P[gpioPortA].DOUTCLR = 1 << 0;
	GPIO->P[gpioPortA].DOUTSET = 1 << 0;
	GPIO->P[gpioPortA].DOUTCLR = 1 << 0;
	timer.setAlarmPeriod( delayTime, alarm1, alarmMatchSeconds );
	timer.resetInterrupts();

	time current;
	time alarm;
	ALARM_REG_SETTING aux;

	timer.getBaseTime(current);
	timer.getAlarmTime(alarm, alarm1, aux);

	timer.setLowPowerMode();

	sentio.LED_SetRed();

	debug.printLine("LED on", false);
	debug.printTimeDet(current);
	debug.printTimeDet(alarm);
	debug.printLine("\r\n",true);

	myStatusBlock.sleepMode = 3;
	myStatusBlock.wantToSleep = true;

	return true;
}

/*
 * ledOffState
 *
 * @brief	Turns the LED off
 */
bool Blink::_ledOffState()
{
	GPIO->P[gpioPortA].DOUTSET = 1 << 0;
	GPIO->P[gpioPortA].DOUTCLR = 1 << 0;
	GPIO->P[gpioPortA].DOUTSET = 1 << 0;
	GPIO->P[gpioPortA].DOUTCLR = 1 << 0;
	timer.setAlarmPeriod( delayTime, alarm1, alarmMatchSeconds );
	timer.resetInterrupts();

	time current;
	time alarm;
	ALARM_REG_SETTING aux;

	timer.getBaseTime(current);
	timer.getAlarmTime(alarm, alarm1, aux);

	timer.setLowPowerMode();

	sentio.LED_ClearRed();

	debug.printLine("LED off", false);
	debug.printTimeDet(current);
	debug.printTimeDet(alarm);
	debug.printLine("\r\n",true);

	myStatusBlock.sleepMode = 3;
	myStatusBlock.wantToSleep = true;

	return true;
}

/*
 * ISR IMPLEMENTATION
 */
void Blink::_ODD_GPIO_InterruptHandler( uint32_t temp )
{
	GPIO->P[gpioPortA].DOUTCLR = 1 << 0;

	if( temp & maskInterruptRTC_wakeup )
	{
		// State transitions (triggered by timer event)
		switch( myStatusBlock.nextState )
		{
		case initialState:
			myStatusBlock.nextState = ledOnState;
			break;

		case ledOnState:
			myStatusBlock.nextState = ledOffState;
			break;

		case ledOffState:
			myStatusBlock.nextState = ledOnState;
			break;

		default:
			myStatusBlock.nextState = initialState;
		}
	}

	GPIO_IntClear(~0);
}
