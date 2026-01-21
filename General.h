#ifndef GENERAL_H
#define GENERAL_H
#include <TM4C129.h>

char ReadUART();
void WriteUART(char c);
void WriteStringUART(char *c);
 void WriteNewLine();
void DelayMilliseconds(uint32_t ms);
 void DelayMicroseconds(uint32_t us);
#endif