#include <TM4C129.h>
#include "Application.h"
#include "General.h"
#include <stdio.h>
extern volatile SystemState sys;

void ReadRadar(){
	int timeout = 160000;


  GPIOE_AHB->DATA |= (1 << 0);          // Set PE0 high (Trigger ON)
  DelayMicroseconds(20);                // Keep high for 20 µs
  GPIOE_AHB->DATA &= ~(1 << 0);         // Set PE0 low (Trigger OFF)


  while (!(GPIOE_AHB->DATA & (1 << 1)) && --timeout) {
      // Wait until PE1 goes high (echo pulse start)
      // Timeout prevents infinite loop if no echo received
  }


  TIMER1->TAV = 0;                      // Reset Timer A counter
  TIMER1->CTL |= (1 << 0);              // Enable Timer A to start counting


  timeout = 160000;
  while ((GPIOE_AHB->DATA & (1 << 1)) && --timeout) {
      // Wait while Echo pin stays high (measure pulse duration)
  }

  TIMER1->CTL &= ~(1 << 0);             // Stop Timer A after pulse ends


  float us = ((float)TIMER1->TAV * 1000000) / 16000000;  // Convert timer ticks to microseconds (16 MHz clock)
  float distance = us * 0.01715f;                        // Convert time to distance (cm)
  int distanceInt = (int)distance;


  char str[32];
  snprintf(str, sizeof(str), "%i cm\r\n", distanceInt);
  // WriteStringUART(str); // Uncomment for debugging output


  if (distanceInt > 5 && distanceInt < 300) {
      sys.distanceCm = distanceInt;      // Valid range: 5–300 cm
  } else if (distanceInt >= 300) {
      sys.distanceCm = 300;              // Cap distance to max 300 cm
  }
	

}