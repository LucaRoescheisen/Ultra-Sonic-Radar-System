#include <TM4C129.h>



void SetupUART0() {
    SYSCTL->RCGCUART |= (1<<0);
    while((SYSCTL->PRUART & (1<<0)) == 0) {}
    
    UART0->CTL &= ~(1<<0);       // Disable UART during config
    UART0->IBRD = 8;             // 16 MHz / (16*115200) = 8.68
    UART0->FBRD = 44;
    UART0->LCRH = 0x68;          // 8-bit, FIFO enable
    UART0->CC = 0x0;             // Use system clock

    UART0->IM |= (1<<4) | (1<<6);         // RX interrupt enable
    NVIC_SetPriority(UART0_IRQn, 1);
    NVIC_EnableIRQ(UART0_IRQn);

    UART0->CTL |= (1<<0) | (1<<8) | (1<<9); // Enable UART, TXE, RXE
}

void SetupGPIOA(void) {													
    SYSCTL->RCGCGPIO |= (1 << 0);              // Enable clock for Port A
    while ((SYSCTL->PRGPIO & (1 << 0)) == 0);  // Wait until Port A is ready

    GPIOA_AHB->AFSEL |= (1 << 0) | (1 << 1);   // Enable alternate function on PA0, PA1
    GPIOA_AHB->PCTL = (GPIOA_AHB->PCTL & ~0xFF) | 0x11; // Set PA0, PA1 ? UART0 (PCTL = 1)
    GPIOA_AHB->DEN |= (1 << 0) | (1 << 1);     // Enable digital function
}

void SetupGPIOF(void) {													//- PF1  PWM0 module output channel 1
    SYSCTL->RCGCGPIO |= (1 << 5);              // Enable clock for Port F
    while ((SYSCTL->PRGPIO & (1 << 5)) == 0);  // Wait until Port F is ready

    GPIOF_AHB->AFSEL |= (1 << 1);              // Enable alternate function on PF1
    GPIOF_AHB->PCTL &= ~(0xF << 4);            // Clear bits 4–7
    GPIOF_AHB->PCTL |= (0x6 << 4);             // Set M0PWM1 function (value 6)
    GPIOF_AHB->DEN |= (1 << 1);                // Enable digital function
}


void SetupGPIOE(void) {
    SYSCTL->RCGCGPIO |= (1 << 4);              // Enable clock for Port E
    while ((SYSCTL->PRGPIO & (1 << 4)) == 0);  // Wait until Port E is ready

    GPIOE_AHB->DIR |= (1 << 0);                // PE0 as output (Trigger)
    GPIOE_AHB->DIR &= ~(1 << 1);               // PE1 as input (Echo)
    GPIOE_AHB->DEN |= (1 << 0) | (1 << 1);     // Enable digital function
}



void SetupTimer(void) {
    SYSCTL->RCGCTIMER |= (1 << 1);             // Enable TIMER1 module
    while ((SYSCTL->PRTIMER & (1 << 1)) == 0); // Wait until TIMER1 is ready

    TIMER1->CTL &= ~(1 << 0);                  // Disable Timer A during setup
    TIMER1->CFG = 0x0;                         // 32-bit mode
    TIMER1->TAMR = (0x1 | (1 << 4));           // One-shot or capture mode, up-count
    TIMER1->TAILR = 0xFFFFFFFF;                // Maximum reload value
    TIMER1->ICR = 0x1;                         // Clear any pending timeout flag
}


void SetupPiezoTimer(void) {
    SYSCTL->RCGCTIMER |= (1 << 0);             // Enable TIMER0 module
    while ((SYSCTL->PRTIMER & (1 << 0)) == 0); // Wait until TIMER0 is ready

    TIMER0->CTL &= ~(1 << 0);                  // Disable Timer A
    TIMER0->CFG = 0;                           // 32-bit configuration (default)

    TIMER0->TAMR = 0x2;                        // Periodic mode
    TIMER0->TAILR = 0xFFFF;                    // Initial reload value
    TIMER0->TAPR = 0x7A;                       // Prescaler value (controls period)
    TIMER0->ICR = 0x1;                         // Clear timeout flag
    TIMER0->CTL |= 1;                          // Enable Timer A
}

void SetupGPIOM(void) {
    SYSCTL->RCGCGPIO |= (1 << 11);             // Enable clock for Port M
    while ((SYSCTL->PRGPIO & (1 << 11)) == 0); // Wait until Port M is ready

    GPIOM->DIR |= (1 << 7);                    // PM7 as output
    GPIOM->DEN |= (1 << 7);                    // Enable digital function
}