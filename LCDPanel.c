#include <TM4C129.h>
#include "Application.h"
#include "LCDPanel.h"
#include "LCD_Display.h"
#include "General.h"
#include "PWMConfigs.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define M_PI 3.141592653
#define LCD_WIDTH 240
#define LCD_HEIGHT 320
 
extern volatile SystemState sys;
extern volatile ServoInfo servoInfo;

void AutoDisplay();
void MainDisplay();
void ManualDisplay();

void SetupLCDPorts(){	//Using GPIO_D
	// setting up the clock for PD0,1,2,3 for the screen
	SYSCTL->RCGCGPIO |= (1 << 3);
	while ((SYSCTL->PRGPIO & (1 << 3)) == 0 ){}; 
		
	GPIOD_AHB->DIR |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3); 			// setting all pins to output
	GPIOD_AHB->AFSEL &= ~(1 << 0); 																		// setting as regular port
	GPIOD_AHB->AFSEL |= (1 << 1) | (1 << 2) | (1 << 3); 
		
	// setting alternate function
	GPIOD_AHB->PCTL &= ~((0xF << 4) | (0xF << 8) | (0xF << 12));
	GPIOD_AHB->PCTL |= (0xF << 4) | (0xF << 8) | (0xF << 12);					// setting to SSI
	
	GPIOD_AHB->DEN |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3); 			// digital mode	
	GPIOD_AHB->PUR |= (1 << 3); // pull up resistor for PD3
}


void initSPI(void){
    SYSCTL->RCGCSSI |= (1 << 2);  // SSI2 clock
    while ((SYSCTL->PRSSI & (1 << 2)) == 0) {};

    SSI2->CR1 &= ~(1 << 1);  // disable SSI
    SSI2->CR0 = 0;            // clear CR0
    SSI2->CR1 = 0;            // clear CR1

    SSI2->CPSR = 4;           // prescaler
    SSI2->CR0 |= (0x7 << 0);  // 8-bit data
    SSI2->CR0 &= ~((1 << 4) | (1 << 5)); // Freescale SPI
    SSI2->CR0 |= (1 << 6) | (1 << 7);    // SPO=1, SPH=1

    SSI2->CR1 &= ~(1 << 2);  // master
    SSI2->CR1 |= (1 << 1);   // enable SSI
		}

void spi_Transmit(uint8_t data){
	// while the register is full, wait
	while((SSI2->SR & (1 << 1)) == 0) {};
	
	SSI2->DR = data;
		
	// while the ssi is currently transmitting
	while(SSI2->SR & (1 << 4)) {};
}



void UpdateScreen(){
	if(sys.screenType != sys.previousScreenType){
		clearScreen();
	}

	
	if(sys.screenType == MENU_SCREEN){
		MainDisplay();
	}
	if(sys.screenType == AUTO_SCREEN){
		AutoDisplay();
	}
	if(sys.screenType == MANUAL_SCREEN){
		ManualDisplay();
	}
	
}


void DrawLine(int x0, int y0, int x1, int y1, uint16_t color) {		//Using Bresenham's Line algorithm as its quite fast
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        drawPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}


void AutoDisplay(){
	
    int angle = sys.currentAngle;
    float rad = (90- angle) * (M_PI / 180.0f);

    int x0 = 0; // center
    int y0 = 0;
    int length = sys.distanceCm; 
    int x1 = x0 + (int)(length * cos(rad));
    int y1 = y0 + (int)(length * sin(rad));

    // clear previous line
		if(servoInfo.dir < 0){
			DrawLine(x0+1, y0, x0 + (int)(300*cos(rad)), y0 + (int)(300*sin(rad)), ILI9341_BLACK);
		}
		else{
			DrawLine(x0-1, y0, x0 + (int)(300*cos(rad)), y0 + (int)(300*sin(rad)), ILI9341_BLACK);
		}
		DrawLine(x0, y0, x0 + (int)(300*cos(rad)), y0 + (int)(300*sin(rad)), ILI9341_BLACK);

    // draw new line
    DrawLine(x0, y0, x1, y1, ILI9341_MAGENTA);
}


void MainDisplay(){
	moveCursor(10,200);
  drawString("Radar System",12);
	moveCursor(10,170);
  drawString("Type m for manual",17);
	moveCursor(10,140);
	drawString("Type a for auto",15);
	moveCursor(10,110);
	drawString("Type b for manual with buzzer",29);
	moveCursor(10,80);
	drawString("Use arrows for left and right",29);
	
}

void ManualDisplay(){
	moveCursor(10, 200);
	drawString("Manual Mode",11);
	moveCursor(0, 170);
	setCharConfig(ILI9341_BLACK, 1, 1, ILI9341_BLACK, 1);
	drawString("            ", 12);
	moveCursor(0, 170);
	setCharConfig(ILI9341_DARKGREEN, 1, 1, ILI9341_BLACK, 1);
	char str[32];
	snprintf(str, sizeof(str), "%icm", sys.distanceCm);
	size_t len = strlen(str);
	drawString(str, len);

}