#include <TM4C129.h>

char ReadUART(){
 	while(UART0->FR & (1<<4)){}
	return (char)(UART0->DR & 0xFF);
}

void WriteUART(char c){
    while(UART0->FR & (1<<5)){}  
    UART0->DR = c;
}




 void WriteStringUART(char *c){
	    while(*c) {
        WriteUART(*c++);
    }
}
 
void WriteNewLine(){
	   char seq[] = { '\n', '\r', '>', '>', ' ' };
    for(int i = 0; i < 4; i++){
        while(UART0->FR & (1<<5)) {}  
        UART0->DR = seq[i];
    }

}


 void DelayMicroseconds(uint32_t us) {
    volatile uint32_t count;
    while(us--) {
        count = 5;  
        while(count--) {
            __asm("nop");  
        }
    }
}
 
void DelayMilliseconds(uint32_t ms){
    volatile uint32_t i, j;
    for(i = 0; i < ms; i++){
        for(j = 0; j < 16000; j++){ 
            __asm__("nop"); 
        }
    }
}