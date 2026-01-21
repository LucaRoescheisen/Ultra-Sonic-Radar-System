#ifndef LCDPANEL_H
#define LCDPANEL_H

void SetupLCDPorts();

void AutoScreen();

void MainMenu();
void 	UpdateScreen();

typedef enum {
		NONE_SCREEN,
		MENU_SCREEN,
    AUTO_SCREEN,
    MANUAL_SCREEN,

} ScreenType;


#endif