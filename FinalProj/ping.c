#include "ping.h"


void ping_init(void){
    //enable clock to GPIO, R1 = port B
        SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
        SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3;
        //set pin 3 as digital pin
        GPIO_PORTB_DEN_R |= 0x08;
//        GPIO_PORTB_DIR_R |= 0x08;
        //enable PB3
        GPIO_PORTB_AFSEL_R |= 0x08;
        //enable pin 3 alternate function on port B
        GPIO_PORTB_PCTL_R |= 0x7000;

        TIMER3_CTL_R &= 0x11111011;
        TIMER3_CFG_R |= 0x4;
        TIMER3_TBMR_R |= 0x17;
        TIMER3_CTL_R |=TIMER_CTL_TBEVENT_BOTH;
        TIMER3_TBILR_R = 0xFFFF;
//        TIMER3_TBPR_R = 0xFF;
        TIMER3_ICR_R |= 0x400;
        TIMER3_IMR_R |= 0x400;
        NVIC_PRI9_R |= 0x20;
        NVIC_EN1_R |= 0x10;
        TIMER3_CTL_R |= TIMER_CTL_TBEN;
}
void send_pulse(void){
    TIMER3_CTL_R &= ~TIMER_CTL_TBEN;
    GPIO_PORTB_AFSEL_R &= ~0x08;
    GPIO_PORTB_PCTL_R &= ~0x7000;
    GPIO_PORTB_DIR_R |= 0x08;// set PB3 as output
    GPIO_PORTB_DATA_R |= 0x08;// set PB3 to high
    timer_waitMicros(5);// wait at least 5 microseconds based on data sheet
    GPIO_PORTB_DATA_R &= ~0x08;// set PB3 to low
    GPIO_PORTB_DIR_R &= ~0x08; // set PB3 as input
    GPIO_PORTB_AFSEL_R |= 0x08;
    GPIO_PORTB_PCTL_R |= 0x7000;
    TIMER3_CTL_R |= TIMER_CTL_TBEN;
}

