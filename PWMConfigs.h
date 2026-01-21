#ifndef PWMCONFIGS_H
#define PWMCONFIGS_H

typedef struct{
	int dir; // left = -1, right = +1
	int previous;
} ServoInfo;

void SetupPWM();
void SetupPWMTimer();
void SetupSweepTimer();

void SweepServo();



#endif