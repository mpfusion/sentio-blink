/*
 * application.cpp
 *
 *  Created on: May, 2012
 *      Author: Sebastian, Matthias
 */

#include "application.h"

STATUS_BLOCK     APPLICATION::myStatusBlock;
INTERRUPT_CONFIG APPLICATION::rtcInterruptConfig;

const time APPLICATION::baseTime( 0 );
const time APPLICATION::delayTime( 1 );

void APPLICATION::init()
{
	setup( &myStatusBlock );
}

void APPLICATION::run()
{
	execute( &myStatusBlock );
}

APPLICATION::APPLICATION()
{
	sentio.init();

	isrDefinition[0].function        = _ODD_GPIO_InterruptHandler;
	isrDefinition[0].interruptNumber = GPIO_ODD_IRQn;
	isrDefinition[0].anchorISR       = false;

	rtcInterruptConfig.enableAlarm1           = true;
	rtcInterruptConfig.enableBatteryBackedSQW = true;
	rtcInterruptConfig.interruptControl       = true;

	stateDefinition[initialState] = _initialState;
	stateDefinition[ledOnState]   = _ledOnState;
	stateDefinition[ledOffState]  = _ledOffState;

	myStatusBlock.initialState = initialState;
}


bool APPLICATION::_initialState()
{
	debug.initializeInterface( 13, 0 );
	debug.initializeDebugUart();

	timer.initializeInterface();
	timer.setBaseTime( baseTime );
	timer.setInterruptConfig( rtcInterruptConfig );
	timer.initializeMCU_Interrupt();
	timer.resetInterrupts();
	timer.setLowPowerMode();

	debug.printLine( "APPLICATION program start", true );

	myStatusBlock.nextState = ledOnState;
	myStatusBlock.sleepMode = on;

	return true;
}


bool APPLICATION::_ledOnState()
{
	time current;
	time alarm;

	ALARM_REG_SETTING aux;
	timer.setBaseTime( baseTime );
	timer.setAlarmTime( delayTime, alarm1, alarmMatchSeconds );
	timer.resetInterrupts();
	timer.getBaseTime( current );
	timer.getAlarmTime( alarm, alarm1, aux );
	timer.setLowPowerMode();

	debug.printLine( "LED on", false );
	debug.printTimeDet( current );
	debug.printTimeDet( alarm );
	debug.printLine( "\n", true );

	myStatusBlock.sleepMode = deepstop;

	sentio.setLED( RED );
	return true;
}


bool APPLICATION::_ledOffState()
{
	time current;
	time alarm;

	ALARM_REG_SETTING aux;
	timer.setBaseTime( baseTime );

	timer.resetInterrupts();
	timer.getBaseTime( current );
	timer.getAlarmTime( alarm, alarm1, aux );
	timer.setAlarmTime( delayTime, alarm1, alarmMatchSeconds );
	timer.setLowPowerMode();

	debug.printLine( "LED off", false );
	debug.printTimeDet( current );
	debug.printTimeDet( alarm );
	debug.printLine( "\n", true );

	myStatusBlock.sleepMode = deepstop;

	sentio.setLED( RED );
	return true;
}


void APPLICATION::_ODD_GPIO_InterruptHandler( uint32_t temp )
{
	if ( temp & maskInterruptRTC_wakeup )
	{
		switch ( myStatusBlock.nextState )
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

	GPIO_IntClear( ~0 );

	myStatusBlock.sleepMode = on;
}
