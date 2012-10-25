/*
 * Blink.cpp
 *
 *  Created on: May, 2012
 *      Author: Sebastian
 */

#include "Blink.h"

STATUS_BLOCK     Blink::myStatusBlock;
INTERRUPT_CONFIG Blink::rtcInterruptConfig;

const time Blink::baseTime(0);
const time Blink::delayTime(1);


Blink::Blink()
{
	myStatusBlock.numberOfISR         = 1;
	myStatusBlock.restoreClockSetting = true;

	rtcInterruptConfig.enableAlarm1           = true;
	rtcInterruptConfig.enableBatteryBackedSQW = true;
	rtcInterruptConfig.interruptControl       = true;

	stateDefinition[initialState] = _initialState;
	stateDefinition[ledOnState]   = _ledOnState;
	stateDefinition[ledOffState]  = _ledOffState;

	ISR_Definition[0].function        = _ODD_GPIO_InterruptHandler;
	ISR_Definition[0].interruptNumber = GPIO_ODD_IRQn;
}


ERROR_CODE Blink::executeApplication()
{
	return startApplication( &myStatusBlock );
}


uint8_t Blink::setupApplication()
{
	return initializeApplication( &myStatusBlock );
}


bool Blink::_initialState()
{
	debug.initializeInterface( numberOfDebugPinsUsed, numberOfButtonsUsed );
	debug.initializeDebugUart();

	timer.initializeInterface();
	timer.setBaseTime( baseTime );
	timer.setInterruptConfig( rtcInterruptConfig );
	timer.initializeMCU_Interrupt();
	timer.resetInterrupts();
	timer.setLowPowerMode();

	sentio.LED_SetOrange();
	debug.printLine("Blink program start", true);

	myStatusBlock.nextState = ledOnState;

	return true;
}


bool Blink::_ledOnState()
{
	time current;
	time alarm;

	ALARM_REG_SETTING aux;

	timer.setAlarmPeriod( delayTime, alarm1, alarmMatchSeconds );
	timer.resetInterrupts();
	timer.getBaseTime(current);
	timer.getAlarmTime(alarm, alarm1, aux);
	timer.setLowPowerMode();

	sentio.LED_SetRed();

	debug.printLine("LED on", false);
	debug.printTimeDet(current);
	debug.printTimeDet(alarm);
	debug.printLine("\n",true);

	myStatusBlock.sleepMode   = 3;
	myStatusBlock.wantToSleep = true;

	return true;
}


bool Blink::_ledOffState()
{
	time current;
	time alarm;

	ALARM_REG_SETTING aux;

	timer.setAlarmPeriod( delayTime, alarm1, alarmMatchSeconds );
	timer.resetInterrupts();
	timer.getBaseTime(current);
	timer.getAlarmTime(alarm, alarm1, aux);
	timer.setLowPowerMode();

	sentio.LED_ClearRed();

	debug.printLine("LED off", false);
	debug.printTimeDet(current);
	debug.printTimeDet(alarm);
	debug.printLine("\n",true);

	myStatusBlock.sleepMode   = 3;
	myStatusBlock.wantToSleep = true;

	return true;
}


void Blink::_ODD_GPIO_InterruptHandler( uint32_t temp )
{
	if( temp & maskInterruptRTC_wakeup )
	{
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
