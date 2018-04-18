#include "adc.h"
#define BIT4        0x10

void ADC_init(void)
{
    //enable port B
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    //enable clock for ADC
    SYSCTL_RCGCADC_R |= 0x1;
    //enable port B pin 4 to work as alternate functions
    GPIO_PORTB_AFSEL_R |= BIT4;
    //set pin to input
    GPIO_PORTB_DIR_R &= ~BIT4;
    //disable PB4 digital function
    GPIO_PORTB_DEN_R &= ~BIT4;
    //enable PB4 analog function
    GPIO_PORTB_AMSEL_R |= BIT4;
    //initialize the port trigger source as processor (default)
    GPIO_PORTB_ADCCTL_R = 0x00;
    //disable SS1 sample sequencer to configure it
    ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN1;
    //initialize the ADC trigger source as processor (default)
    ADC0_EMUX_R = ADC_EMUX_EM1_PROCESSOR;
    //set 1st sample to use the AIN10 ADC pin
    ADC0_SSMUX1_R |= 0x000A;
    ADC0_SSCTL1_R |= (ADC_SSCTL1_IE0 | ADC_SSCTL1_END0);
    ADC0_SAC_R |= ADC_SAC_AVG_64X;
    //re-enable ADC0 SS1
    ADC0_ACTSS_R |= ADC_ACTSS_ASEN1;
}
unsigned ADC_read(char channel)
{
        //disable ADC0 SS1 to configure it
       ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN1;
        //set 1st sample to use the channel ADC pin
      ADC0_SSMUX1_R |= channel;
        //re-enable ADC0 SS0
        ADC0_ACTSS_R |= ADC_ACTSS_ASEN1;
        //initiate SS1 conversion
        ADC0_PSSI_R=ADC_PSSI_SS1;
        //wait for ADC conversion to be complete
        while((ADC0_RIS_R & ADC_RIS_INR1) == 0);

    return ADC0_SSFIFO1_R;
}
