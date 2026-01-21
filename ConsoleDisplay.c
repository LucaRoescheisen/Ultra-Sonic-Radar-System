#include "General.h"
#include "Application.h"
#include <TM4C129.h>
#include <stdio.h>

extern volatile SystemState sys;

void ClearScreen(){
	WriteStringUART("\033[2J");
}


void MoveCursor(int row, int col){
	char buff[20];
	
	sprintf(buff, "\033[%d;%dH", row, col);
   WriteStringUART(buff);


}

char radarMap[20][50];
int prevRow = -1;
int prevCol = -1;
int sweepDir = 1; // 1 = forward, -1 = backward

void RadarPlot() {
    int row = sys.currentAngle / 5;
    int col = sys.distanceCm / 6;

    // Clamp row/col
    if(row < 0) row = 0;
    if(row > 19) row = 19;
    if(col < 0) col = 0;
    if(col > 49) col = 49;

    // Check if we are moving backward
    if(prevRow >= 0){
        if(sweepDir > 0 && row < prevRow) sweepDir = -1;  // forward ? backward
        else if(sweepDir < 0 && row > prevRow) sweepDir = 1; // backward ? forward

        // Only erase row if we are sweeping back into it
        
    }

    // Draw current point
		for(int c=0; c<50; c++){
                radarMap[row][c] = 0; // clear row
            }
    radarMap[row][col] = 'x';

    // Save previous position
    prevRow = row;
    prevCol = col;
}


void DisplayRadar() {
    MoveCursor(1,1);
    WriteStringUART("Radar Sweep\n");

    for(int r=0; r<20; r++){
        MoveCursor(r+2, 1);
        for(int c=0; c<50; c++){
            char ch = radarMap[r][c] ? radarMap[r][c] : ' ';
            WriteUART(ch);
        }
    }
}




void EchoChar(char c){
	MoveCursor(30, 1);  // start after >>
    WriteUART(c);

}
