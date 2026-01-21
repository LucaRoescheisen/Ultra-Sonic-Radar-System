#include <TM4C129.h>
#include "Application.h"
#include "General.h"
#include <stdio.h>
extern volatile SystemState sys;

void SetupADCGPIO(){ 
    SYSCTL->RCGCGPIO |= (1 << 4);               // Enable clock for Port E
    while (!(SYSCTL->PRGPIO & (1 << 4))) {};    // Wait until Port E is ready

    GPIOE_AHB->AMSEL |= (1 << 3);               // Enable analog mode on PE3
    GPIOE_AHB->DIR &= ~(1 << 3);                // Set PE3 as input
    GPIOE_AHB->AFSEL |= (1 << 3);               // Enable alternate function
    GPIOE_AHB->DEN &= ~(1 << 3);                // Disable digital function for PE3

    GPIOE_AHB->DEN  |= (1 << 4);                // Enable digital function on PE4
    GPIOE_AHB->DIR  |= (1 << 4);                // Set PE4 as output
    GPIOE_AHB->DATA |= (1 << 4);                // Drive PE4 high


    SYSCTL->RCGCADC |= (1 << 0);                // Enable ADC0 module clock
    while (!(SYSCTL->PRADC & (1 << 0))) {};     // Wait until ADC0 is ready

    ADC0->PC = 0x3;                             // Configure max sample rate (250 ksps)
    ADC0->ACTSS &= ~(1 << 3);                   // Disable SS3 during setup
    ADC0->EMUX &= ~(0xF << 12);                 // Select software trigger for SS3
    ADC0->SSMUX3 = 0x0;                         // Select AIN0 (PE3) as input channel
    ADC0->SSCTL3 = (1 << 1) | (1 << 2);         // Enable IE0 (interrupt) & END0 (end of sequence)
    ADC0->ACTSS |= (1 << 3);                    // Re-enable SS3

}


void ReadADC() {
	ADC0->PSSI |= (1 << 3);                     // Initiate SS3 conversion
    while (!(ADC0->RIS & (1 << 3))) {};         // Wait for conversion complete flag

    uint32_t result = ADC0->SSFIFO3 & 0xFFF;    // Read 12-bit ADC result
    ADC0->ISC = (1 << 3);                       // Clear completion flag

    /* Scale ADC result from 0–4095 ? 650–2200 range */
    result = 650 + (result * (2200 - 650)) / 4095;
    sys.adcVal = result;                        // Store result in global system struct


}