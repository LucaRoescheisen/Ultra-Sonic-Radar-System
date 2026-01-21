#include "PWMConfigs.h"
#include <TM4C129.h>
#include "General.h"
#include "Application.h"

extern volatile ServoInfo servoInfo;
extern volatile SystemState sys;

void SetupPWM(){ 							//M0PWM1
		SYSCTL->RCGCPWM |= (1 << 0);
    while ((SYSCTL->PRPWM & (1 << 0)) == 0) {};

    PWM0->CC = 0x103;    // system clock, divide by 16

    PWM0->_0_CTL = 0;             // disable generator 1

    PWM0->_0_LOAD = 19999;        // 50Hz period
    PWM0->_0_CMPB = 500;         // Changing this changes the angle of the motor, it ranges from 500 to 2600 (500 all to left, 2600 all to right

    PWM0->_0_GENB |= (0x2 << 2);  // LOW on LOAD
    PWM0->_0_GENB |= (0x3 << 10);  // HIGH on CMPB down-count

    PWM0->_0_CTL = 1;             // enable generator 0
    PWM0->ENABLE = (1 << 1);                   // enable PF1 output
}



void SetupSweepTimer(void) {
    SYSCTL->RCGCTIMER |= (1 << 3);             // Enable clock for TIMER3
    while ((SYSCTL->PRTIMER & (1 << 3)) == 0); // Wait until TIMER3 is ready

    TIMER3->CTL &= ~(1 << 0);                  // Disable Timer A during setup
    TIMER3->CFG = 0;                           // 32-bit timer configuration
    TIMER3->TAMR = 0x2;                        // Periodic mode (count down)
    TIMER3->TAILR = 223999;                    // Reload value (defines interval)
    TIMER3->ICR = 0x1;                         // Clear any previous timeout flag
    TIMER3->IMR |= 0x1;                        // Enable Timer A timeout interrupt

    NVIC_EnableIRQ(TIMER3A_IRQn);              // Enable TIMER3A interrupt in NVIC
    NVIC_SetPriority(TIMER3A_IRQn, 3);         // Set interrupt priority to level 3

    TIMER3->CTL |= 0x1;                        // Enable Timer A
}


int FindAngle(){
    int cmp = PWM0->_0_CMPB;

    // Clamp to valid range to avoid out-of-bounds
    if(cmp < 967) cmp = 967;
    if(cmp > 1753) cmp = 1753;

    // Map 967 ? 0°, 1753 ? 90°
    int angle = (cmp - 1753) * 90 / (967 - 1753);

    return angle;
}
void TIMER3A_Handler(){																				//2200 is all the way left/right
	
	TIMER3->ICR = 0x1;  // Clear interrupt first

			if(sys.measurementMode == AUTO){
					int current = PWM0->_0_CMPB;
					
					// Check limits and reverse direction
					if(current >= 1753) {
						servoInfo.dir = -1;
						 current  = 1753;
					}
					else if(current <= 967) {
					servoInfo.dir = 1;
						current = 967;
					}

					// Update PWM based on direction
					if(servoInfo.dir > 0) {
						PWM0->_0_CMPB = current + 1;
					}
					else {
						PWM0->_0_CMPB = current - 1;
					}
				
				sys.currentAngle = FindAngle();
		
    }
		
		
		
		else if (sys.measurementMode == LEFT){
			servoInfo.dir = -1;
			PWM0->_0_CMPB = 650;
		}
		else if (sys.measurementMode == RIGHT){
			servoInfo.dir = 1;
			PWM0->_0_CMPB = 2200;
		}
		else if (sys.measurementMode == MANUAL){
			PWM0->_0_CMPB = sys.adcVal;
			
		}
		else{
			PWM0->_0_CMPB = 1350;
		}
}



