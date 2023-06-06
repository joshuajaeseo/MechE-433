#include "nu32dip.h"          // constants, funcs for startup and UART

int main(void) {
  NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
    RPA0Rbits.RPA0R = 0b0101;//set Pin A0 to OC1
    T2CONbits.TCKPS = 6;     // Timer2 prescaler N=64 (1:64)
    PR2 = 15000-1;              // period = (PR2+1) * N * (1/48000000) = 50 Hz
    TMR2 = 0;                // initial TMR2 count is 0
    OC1CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
    OC1CONbits.OCTSEL = 0;   // Use timer2
    OC1RS = 750;             // duty cycle = OC1RS/(PR2+1) = 25%
    OC1R = 750;              // initialize before turning OC1 on; afterward it is read-only
    T2CONbits.ON = 1;        // turn on Timer2
    OC1CONbits.ON = 1;       // turn on OC1

    while(1){
        OC1RS = 750;
        _CP0_SET_COUNT(0);
        while (_CP0_GET_COUNT() < 48000000*4){
        }
        OC1RS = 1500;             // duty cycle = OC1RS/(PR2+1) = 25%

        _CP0_SET_COUNT(0);
        while (_CP0_GET_COUNT() < 48000000*4){
        }
    }
}

