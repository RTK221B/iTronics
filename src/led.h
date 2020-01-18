/* this code for the LED light for alert*/

#include <Adafruit_NeoPixel.h>

#define PIN 13
int Pixels = 8;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(Pixels, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
    strip.begin();
    strip.show(); //Initilize strip to all off
}

void loop()
{
    delay(50);
    redFlash(); //call redFlash funtion
    delay(50);
    red2Flash(); //Call blueFlash function
}

void redFlash()
{
    for (int x = 0; x <= 3; x++)
    {
        red(strip.setPixelColor(255, 0, 0), 13); //call Red funtion - pass it color and dleay values
        red(strip.Color(0, 0, 0), 0);            //Turn off all pixels
        delay(75);
    }
} //end redFlash

void red2Flash()
{
    for (int x = 0; x <= 3; x++)
    {
        red2(strip.Color(225, 0, ), 13); //call blue funtion - pass it color and dleay values
        red2(strip.Color(0, 0, 0), 0);   //Turn off all pixels
        delay(75);
    }
} //end blueFlash

void red(uint32_t c, uint8_t wait) //passed color (c) and delay (wait) values
{
    int i = 0;             //start at pixel 0
    while (i < Pixels / 2) //loop until you reach half the pixels
    {
        strip.setPixelColor(i, c); //sets the pixel and color
        i++;
    }
    strip.show(); //turns on the pixels
    delay(wait);  //wait designated time (wait)
} //end red

void red2(uint32_t c, uint8_t wait) //passed color (c) and delay (wait) values
{
    int i = Pixels + 1;        //start at the last pixel
    while (i > Pixels / 2 - 1) //loop until you reach half the pixels
    {
        strip.setPixelColor(i, c); //sets pixel and color
        i--;
    }
    strip.show(); //turns on pixels
    delay(wait);  //wait designated time (wait)
} //end blue