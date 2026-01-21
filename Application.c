#include "Application.h"
#include "Peripherals.h"
#include "ADCConfigs.h"
#include "PWMConfigs.h"
#include "LCDPanel.h"
#include "General.h"
#include "LCD_Display.h"
#include "Radar.h"
#include <stdio.h>
#include <TM4C129.h>
extern volatile SystemState sys;
volatile ServoInfo servoInfo = {.dir = 1};

void ReadUserInput();


void SystemInitialise(){
	sys.running = 1;
	sys.measurementMode = NONE;
	sys.previousScreenType = NONE_SCREEN;
	sys.screenType = MENU_SCREEN;
	SetupUART0();			//Tera Term connection
  SetupGPIOA();			//Tera Term Connection
	SetupGPIOF();			//PWM0 output for ADC
	SetupGPIOE();			//Echo and Trigger pins for Ultrasonic sensor
	SetupGPIOM();			//Output for Piezo Buzzer
	SetupTimer();			//Timer to measure echo pin 
  SetupPWM();				//Sets up PWM module for adc
	SetupSweepTimer();//Timer for Servo motor
	SetupADCGPIO();		//Sets up GPIO and config for adc
	
	SetupPiezoTimer();// Sets up the timer for the piezo buzzer
  SetupLCDPorts();	//All LCD GPIO ports
	initSPI();				//SSI config
  initLCD();
  setRotation(2);
  clearScreen();
  setCharConfig(ILI9341_DARKGREEN, 1, 1, ILI9341_BLACK, 1);

}	



void SystemUpdate(){
	ReadUserInput();
	ReadRadar();
	UpdateScreen();
	sys.previousScreenType = sys.screenType;
}






void ReadUserInput(){
	if(sys.uartEchoChar == 'a'){
		sys.measurementMode = AUTO;
		sys.screenType = AUTO_SCREEN;
		sys.isBuzzer = 0;
	}
	if(sys.uartEchoChar == 'b'){
		sys.measurementMode = MANUAL;
		sys.screenType = MANUAL_SCREEN;
		sys.isBuzzer = 1;
	}
	if(sys.uartEchoChar == 'm'){
		sys.measurementMode = MANUAL;
		sys.screenType = MANUAL_SCREEN;
		sys.isBuzzer = 0;
	}
	if(sys.uartEchoChar == '<'){
		sys.measurementMode = RIGHT;
		sys.isBuzzer = 0;
	}
	if(sys.uartEchoChar == '>'){
		sys.measurementMode = LEFT;
		sys.isBuzzer = 0;
	}


	
	if(sys.measurementMode == NONE){
		sys.screenType = MENU_SCREEN;
	}
	
	if(sys.measurementMode == MANUAL){
		ReadADC();
		if (TIMER0->RIS & 1) {    // check timeout flag
			TIMER0->ICR = 1;       // clear flag
			GPIOM->DATA ^= (1<<7); // toggle buzzer
			float buzzerSpeed = (sys.adcVal - 650.0f) / (2200.0f - 650.0f);
      buzzerSpeed = 1.0f - buzzerSpeed;
			uint32_t ticks = 1600000 - (uint32_t)((1600000 - 16000000) * buzzerSpeed);
      TIMER0->TAILR = ticks;
		}

	}

}



volatile char uartEchoBuffer = 0;

void UART0_Handler(void) {
    UART0->ICR = (1<<4);

    if (!(UART0->FR & (1<<4))) {  
        char c = UART0->DR & 0xFF;

        if (c == 13) {  // enter
            sys.uartEchoChar = uartEchoBuffer;
            WriteNewLine();
						
        } 
				else if(c == 27){						//escape
						GPIOM->DATA |= (1<<7);  // turn buzzer ON
						DelayMilliseconds(20);
						GPIOM->DATA &= ~(1<<7); 
						WriteStringUART("esc pressed");
						WriteNewLine();
						sys.uartEchoChar = 0; 
						sys.measurementMode = NONE;
				}
        else {
            uartEchoBuffer = c;       // store last typed
            WriteUART(c);
        }
    }
}

