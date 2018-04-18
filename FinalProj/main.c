#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "lcd.h"
#include "Timer.h"
#include "adc.h"
#include "ping.h"
#include "uart.h"
#include "servo.h"

// bot 25

struct Object{//holds data about detected objects
    int right_edge;
    int left_edge;
    int center;
    int distance;
    int angular_width;
//    double radian_width;
    double linear_width;
};

volatile unsigned long  rising_time=0; // start time of the return pulse
volatile unsigned long  falling_time=0; // end time of the return pulse
volatile unsigned long event_time=0;
volatile unsigned int dist = 0;
volatile unsigned int overflow = 0;

//Interrupt to control Ping sensor
void TIMER3B_Handler(void){
    TIMER3_ICR_R = TIMER_ICR_CBECINT; //clear the TIMER3B timeout interrupt flag (page 754 of datasheet)

    rising_time = falling_time;
    falling_time = (TIMER3_TBPS_R << 16)|TIMER3_TBR_R;  // pg 770 datasheet (TBPS) 765 (TBR)
    if (falling_time < rising_time){
        event_time = (pow(2,24)-rising_time)+falling_time;  // when it overflows 't1 + t2' =time_width
        overflow++;
    }
    else {
        event_time = falling_time - rising_time;   //else time_width =event_time
    }
    dist = 0.0010625*event_time;   /* convert time in clock counts to single-trip distance in cm */
}

char plot[45][155];

void createPlot(){
    int i,j;
    memset(plot, 0, sizeof plot);
    for(i=0; i< 40; i++){
        for(j=0; j< 150; j++){
            plot[i][j] = '.';
        }
        printf("\n");
    }
}
void printPlot(){
    int i,j;
    for(i=0; i < 40; i++){
        for(j=0; j< 150; j++){
            char line = plot[i][j];
            if(j == 0)  uart_sendStr("  ");
            uart_sendChar(line);
       }
        uart_sendStr("\n\r");
    }
}

void clearPlot(){
    int i;
    for(i=0; i< 45; i++){
        uart_sendStr("\n\r");
    }

}

int main(){
    //Declaration and initialization
    int i;
    struct Object object[20];
    int ping_dist[181];
    int adc_dist[181];
    int degree = 0;
    int obj_count = 0; // count objects
    int threshold = 100; // some value, not sure yet -> in lab9-intro-lecture
    char mess[80];


    //initialize registers
    lcd_init();
    ADC_init();
    servo_init();
    uart_init();
    ping_init();

    clearPlot();
    createPlot();
    printPlot();


    lcd_printf("Connecting..."); // Wait until done scanning

    IntRegister(INT_TIMER3B, TIMER3B_Handler);
    IntMasterEnable();

    while(1){
        for(i=0;i<=180;i++)
        {
            send_pulse(); //send pulse
            ping_dist[i] = dist; //Put dist into ping distance array, i corresponds to
            uint32_t value= ADC_read(10); // ir sensor
            int distance = 62215*pow(value,-1.095); // calculate distance for Cybot
            adc_dist[i] = distance; //Put distance into adc distance array, i corresponds to degrees.
            move_servo(++degree); //turn servo
        }
        degree=0;
        uart_sendStr("Degree\t\tIR\t\tSonar\n\r"); //Send data to Putty

        for(i=0;i<=180;i++) //increment for each degree
        {
            sprintf(mess,"  %d\t\t%d\t\t%d\n\r",i,adc_dist[i],ping_dist[i]); //Uses sprintf to create string to send to Putty
            uart_sendStr(mess); //send data to Putty
        }

        for(i=0; i<=180; i++)
                {//finds edges
                    if( (adc_dist[i] < threshold && ping_dist[i] < threshold) && (i==0 || (adc_dist[i-1] >= threshold || ping_dist[i-1] >= threshold)) )// runs on object right edge
                        object[obj_count].right_edge = i;

                    if( (((adc_dist[i] > threshold) || (ping_dist[i] > threshold))&& (i!=0) && ((adc_dist[i-1] < threshold) && (ping_dist[i-1] < threshold))) || ((adc_dist[i] < threshold) && (ping_dist[i] < threshold)&& (i==180)) ){//runs on object left edge
                        object[obj_count].left_edge = i;

                        if((object[obj_count].left_edge - object[obj_count].right_edge) > 6)//accepts object if at least 6-degrees wide
                            obj_count++;//moves to collecting info for next object
                    }
                }
        int smallest = 0;
                //Update all information
                for(i = 0; i < obj_count; i++){ //Iterate through objects
                    object[i].angular_width = object[i].left_edge - object[i].right_edge;//finds object angular width
                    if(object[i].angular_width<object[smallest].angular_width){
                        smallest = i;
                    }
                    object[i].center = object[i].right_edge + 0.5*object[i].angular_width;//finds object center
                    object[i].distance = ping_dist[object[i].center];//find's object's distance

                    object[smallest].linear_width = object[smallest].distance * tan(object[smallest].angular_width * 3.14 / 180); // Calculate smallest object width

                }
                lcd_printf("No Objs: %d\nSmallest: %d\nDistance: %d\nWidth: %d",obj_count,smallest+1,object[smallest].distance,(int) object[smallest].linear_width); //Print data to Cybot

                  //Putty
                   char msg[100];
                   sprintf(msg, "No Objs: %d Smallest: %d Distance: %d Width: %d ",obj_count,smallest+1,object[smallest].distance,(int) object[smallest].linear_width); //Create string to send to Putty
                   uart_sendStr(msg);  //Send string to Putty
                   uart_sendChar('\r'); //End transmission
                   uart_sendChar('\n');


                move_servo(object[smallest].center);// the smallest radial width
                obj_count=0;
                exit(1);




    }

}

