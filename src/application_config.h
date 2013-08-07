/*
 * application_config.h
 *
 *  Created on: Feb 17, 2012
 *      Author: matthias
 */

#ifndef APPLICATIONCONFIG_H_
#define APPLICATIONCONFIG_H_

#include "sentio_em_io.h"


/****************************************************************************************************************************************//**
 * @brief MCU Clock Configuration
 *
 *******************************************************************************************************************************************/

#define MCU_CLOCK_MHZ           32
#define LF_CLOCK                1
#define CLOCK_TEST_PINS         0


/****************************************************************************************************************************************//**
 * @brief Specification of the Wireless Platform
 *
 *******************************************************************************************************************************************/

#define ONBOARD_LEDS            1
#define ONBOARD_BUTTON          1


/****************************************************************************************************************************************//**
 * @brief Interrupt Configuration
 *
 *******************************************************************************************************************************************/

#define DMA_IRQ_ENABL           0
#define GPIO_EVEN_IRQ_ENABLE    0
#define TIMER0_IRQ_ENABLE       0
#define USART0_RX_IRQ_ENABLE    0
#define USART0_TX_IRQ_ENABLE    0
#define ACMP0_IRQ_ENABLE        0
#define ADC0_IRQ_ENABLE         0
#define DAC0_IRQ_ENABLE         0
#define I2C0_IRQ_ENABLE         0
#define GPIO_ODD_IRQ_ENABLE     1
#define TIMER1_IRQ_ENABLE       0
#define TIMER2_IRQ_ENABLE       0
#define USART1_RX_IRQ_ENABLE    0
#define USART1_TX_IRQ_ENABLE    0
#define USART2_RX_IRQ_ENABLE    0
#define USART2_TX_IRQ_ENABLE    0
#define UART0_RX_IRQ_ENABLE     0
#define UART0_TX_IRQ_ENABLE     0
#define LEUART0_IRQ_ENABLE      0
#define LEUART1_IRQ_ENABLE      0
#define LETIMER0_IRQ_ENABLE     0
#define PCNT0_IRQ_ENABLE        0
#define PCNT1_IRQ_ENABLE        0
#define PCNT2_IRQ_ENABLE        0
#define RTC_IRQ_ENABLE          0
#define CMU_IRQ_ENABLE          0
#define VCMP_IRQ_ENABLE         0
#define LCD_IRQ_ENABLE          0
#define MSC_IRQ_ENABLE          0
#define AES_IRQ_ENABLE          0


/****************************************************************************************************************************************//**
 * @brief Statemachine Configuration
 *
 *******************************************************************************************************************************************/

#define MAX_NUM_STATES 10
#define MAX_NUM_APPS   1


#endif /* APPLICATIONCONFIG_H_ */
