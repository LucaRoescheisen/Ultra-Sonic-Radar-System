#include <TM4C129.h>
#include "Application.h"
#include "ES.h"
#include <stdio.h>
#include "General.h"


volatile SystemState sys = {0};

//Entry Point
int main(){
	ES_setSystemClk(16);
	
	SystemInitialise();
	WriteNewLine();
	while(sys.running){
		SystemUpdate();
  }

	return 1;
}


