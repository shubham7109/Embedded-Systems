#include "lcd.h"

/// Simple 'Hello, world' program
/**
 * This program prints "Hello, world" to the LCD screen
 * @author Chad Nelson
 * @date 06/26/2012
 */
int main (void) {
	// Initialize the the LCD.  This also clears the screen.
	lcd_init(); 


	int time =300;
	int length = 20;
	char myString[] = "MicroControllers are fun!";

	int pos = 20;
	int i=0;
	int counter=0;
	int counter2=0;
	while(1)
	{

	    timer_waitMillis(time);
	    char output[] = "                    ";
	    lcd_init();
	    if(pos > 0)
	    {
	        // Prints myString
	        for(i=pos-1; i < 20 ; i++)
	        {
	            output[i] = myString[counter];
	            counter++;
	        }

	        pos--;
	    }
	    else
	    {
              for(i=0; i < 20 ; i++)
              {
                  if(counter2 < 26)
                      output[i] = myString[counter2+i];
              }

              counter2++;
	    }
	    if(counter2 >= 26)
	    {
	        pos =20;
	        counter2 = 0;
	    }

	    lcd_printf(output);

	    counter=0;

	}

    

    
	return 0;
}
