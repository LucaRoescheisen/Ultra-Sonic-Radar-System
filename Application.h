#ifndef APPLICATION_H
#define APPLICATION_H

#include <stdint.h>
#include "LCDPanel.h"
// Enum type for measurement mode
typedef enum {
		NONE,
    MANUAL,
    AUTO,
    LEFT,
		RIGHT
} MeasurementType;

// System state structure
typedef struct {
    volatile MeasurementType measurementMode;  // current measurement mode
    volatile uint32_t lastEchoTime;
    volatile uint32_t distanceCm;
    volatile int currentAngle;
    volatile uint8_t previousAngle;
    volatile uint8_t running;
		volatile uint8_t isBuzzer;
		volatile uint32_t adcVal;
		volatile char uartEchoChar;
		
		ScreenType screenType;
		ScreenType previousScreenType;
} SystemState;

// Distance samples structure
typedef struct {
    volatile float samples[5];
    volatile uint8_t pointer;
    volatile float average;
} DistanceSamples;

// Function declarations
void SystemInitialise();
void SystemUpdate();

#endif