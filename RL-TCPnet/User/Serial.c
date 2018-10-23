/*-----------------------------------------------------------------------------
 *      RL-ARM
 *-----------------------------------------------------------------------------
 *      Name:    Serial.c
 *      Purpose: Serial Input Output for STmicroelectronics STM32F4xx
 *-----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stm32f4xx.h>                  /* STM32F4xx Definitions              */
#include "Serial.h"
#include "stdio.h"

#ifdef __DBG_ITM
volatile int32_t ITM_RxBuffer;
#endif

/*-----------------------------------------------------------------------------
 *       SER_Init:  Initialize Serial Interface
 *----------------------------------------------------------------------------*/
void SER_Init (void) {
#ifdef __DBG_ITM
  ITM_RxBuffer = ITM_RXBUFFER_EMPTY;

#else
  RCC->APB2ENR  |= (1UL << 4);          /* Enable USART1 clock                */
  RCC->AHB1ENR  |= (1UL << 0);          /* Enable GPIOA clock                 */

  /* Connect PA9 and PA10 to Alternate Function 7 (AF7) */
  GPIOA->MODER  &= ~(   3 << 10*2);
  GPIOA->MODER  |=  (   2 << 10*2);     /* PA10: Alternate function mode      */
  GPIOA->AFR[1] &= ~(0x0F <<  8);
  GPIOA->AFR[1] |=  (   7 <<  8);       /* PA10: Alternate function USART1_TX */


  GPIOA->MODER  &= ~(   3 << 9*2);
  GPIOA->MODER  |=  (   2 << 9*2);       /* PA9: Alternate function mode      */
  GPIOA->AFR[1] &= ~(0x0F << 4);
  GPIOA->AFR[1] |=  (   7 << 4);         /* PA9: Alternate function USART1_RX */

  /* Configure USART1: 115200 baud @ 84MHz, 8 bits, 1 stop bit, no parity     */
  USART1->BRR = (45 << 4) | 9;
  USART1->CR3 = 0x0000;
  USART1->CR2 = 0x0000;
  USART1->CR1 = 0x200C;
#endif
}


/*-----------------------------------------------------------------------------
 *       SER_PutChar:  Write a character to Serial Port
 *----------------------------------------------------------------------------*/
int32_t SER_PutChar (int32_t ch) {
#ifdef __DBG_ITM
  int i;
  ITM_SendChar (ch & 0xFF);
  for (i = 10000; i; i--);
#else
  while (!(USART1->SR & 0x0080));
  USART1->DR = (ch & 0xFF);
#endif  

  return (ch);
}


/*-----------------------------------------------------------------------------
 *       SER_GetChar:  Read a character from Serial Port
 *----------------------------------------------------------------------------*/
int32_t SER_GetChar (void) {
#ifdef __DBG_ITM
  if (ITM_CheckChar())
    return ITM_ReceiveChar();
#else
  if (USART1->SR & 0x0020)
    return (USART1->DR);
#endif
  return (-1);
}

/*--------------------------- fputc -----------------------------------------*/
int fputc (int ch, FILE *f)  {
  /* Debug output to serial port. */
  if (ch == '\n')  {
    SER_PutChar (0x0D);
  }
  SER_PutChar (ch & 0xFF);
  return (ch);
}

/*-----------------------------------------------------------------------------
 * end of file
 *----------------------------------------------------------------------------*/
