// WS2812B library
#include "nu32dip.h"
#include "ws2812b.h"
// other includes if necessary for debugging
 
// Timer2 delay times, you can tune these if necessary
#define LOWTIME 15 // number of 48MHz cycles to be low for 0.35uS
#define HIGHTIME 65 // number of 48MHz cycles to be high for 1.65uS


// setup Timer2 for 48MHz, and setup the output pin
void ws2812b_setup() {
    T2CONbits.TCKPS = 0; // Timer2 prescaler N=1 (1:1)
    PR2 = 65535; // maximum period
    TMR2 = 0; // initialize Timer2 to 0
    T2CONbits.ON = 1; // turn on Timer2

    // initialize output pin as off
    // TRIS...
    TRISBbits.TRISB6 = 0;
    LATBbits.LATB6 = 0;
    // LAT...
}

// build an array of high/low times from the color input array, then output the high/low bits
void ws2812b_setColor(wsColor * c, int numLEDs) {
    int i = 0; int j = 0; // for loops
    int numBits = 2 * 3 * 8 * numLEDs; // the number of high/low bits to store, 2 per color bit
    volatile unsigned int delay_times[numBits]; // I only gave you 5 WS2812B, adjust this if you get more somewhere

    // start at time at 0
    delay_times[0] = 0;
    
    int nB = 1; // which high/low bit you are storing, 2 per color bit, 24 color bits per WS2812B
	
    // loop through each WS2812B
    for (i = 0; i < numLEDs; i++) {
        // loop through each color bit, MSB first
        for (j = 7; j >= 0; j--) {
            // if the bit is a 1
            if (((c[i].r >> j) & 0b1 ) == 0b1/* identify the bit in c[].r, is it 1 */) {
                // the high is longer
                delay_times[nB] = delay_times[nB - 1] + HIGHTIME;
                nB++;
                delay_times[nB] = delay_times[nB - 1] + LOWTIME;
                nB++;
            } 
            // if the bit is a 0
            else {
                // the low is longer
                delay_times[nB] = delay_times[nB - 1] + LOWTIME;
                nB++;
                delay_times[nB] = delay_times[nB - 1] + HIGHTIME;
                nB++;
            }
        }
        // do it again for green
        for (j = 7; j >= 0; j--) {
            // if the bit is a 1
            if (((c[i].g >> j) & 0b1) == 0b1/* identify the bit in c[].r, is it 1 */) {
                // the high is longer
                delay_times[nB] = delay_times[nB - 1] + HIGHTIME;
                nB++;
                delay_times[nB] = delay_times[nB - 1] + LOWTIME;
                nB++;
            } 
            // if the bit is a 0
            else {
                // the low is longer
                delay_times[nB] = delay_times[nB - 1] + LOWTIME;
                nB++;
                delay_times[nB] = delay_times[nB - 1] + HIGHTIME;
                nB++;
            }
        }
		// do it again for blue
        for (j = 7; j >= 0; j--) {
            // if the bit is a 1
            if (((c[i].b >> j) & 0b1) == 0b1/* identify the bit in c[].r, is it 1 */) {
                // the high is longer
                delay_times[nB] = delay_times[nB - 1] + HIGHTIME;
                nB++;
                delay_times[nB] = delay_times[nB - 1] + LOWTIME;
                nB++;
            } 
            // if the bit is a 0
            else {
                // the low is longer
                delay_times[nB] = delay_times[nB - 1] + LOWTIME;
                nB++;
                delay_times[nB] = delay_times[nB - 1] + HIGHTIME;
                nB++;
            }
        }
    }

    // turn on the pin for the first high/low
    LATBbits.LATB6 = 1;
    TMR2 = 0; // start the timer
    for (i = 1; i < numBits; i++) {
        while (TMR2 < delay_times[i]) {
        }
        LATBINV = 0b1000000; // invert B6
    }
    LATBbits.LATB6 = 0;
    TMR2 = 0;
    while(TMR2 < 50000){} // wait 50uS, reset condition
}

// adapted from https://forum.arduino.cc/index.php?topic=8498.0
// hue is a number from 0 to 360 that describes a color on the color wheel
// sat is the saturation level, from 0 to 1, where 1 is full color and 0 is gray
// brightness sets the maximum brightness, from 0 to 1
wsColor HSBtoRGB(float hue, float sat, float brightness) {
    float red = 0.0;
    float green = 0.0;
    float blue = 0.0;

    if (sat == 0.0) {
        red = brightness;
        green = brightness;
        blue = brightness;
    } else {
        if (hue == 360.0) {
            hue = 0;
        }

        int slice = hue / 60.0;
        float hue_frac = (hue / 60.0) - slice;

        float aa = brightness * (1.0 - sat);
        float bb = brightness * (1.0 - sat * hue_frac);
        float cc = brightness * (1.0 - sat * (1.0 - hue_frac));

        switch (slice) {
            case 0:
                red = brightness;
                green = cc;
                blue = aa;
                break;
            case 1:
                red = bb;
                green = brightness;
                blue = aa;
                break;
            case 2:
                red = aa;
                green = brightness;
                blue = cc;
                break;
            case 3:
                red = aa;
                green = bb;
                blue = brightness;
                break;
            case 4:
                red = cc;
                green = aa;
                blue = brightness;
                break;
            case 5:
                red = brightness;
                green = aa;
                blue = bb;
                break;
            default:
                red = 0.0;
                green = 0.0;
                blue = 0.0;
                break;
        }
    }

    unsigned char ired = red * 255.0;
    unsigned char igreen = green * 255.0;
    unsigned char iblue = blue * 255.0;

    wsColor c;
    c.r = ired;
    c.g = igreen;
    c.b = iblue;
    return c;
}

int main (void) {
    NU32DIP_Startup();
    ws2812b_setup();
 
    wsColor p1 = {0,1,0};
    wsColor p2 = {0,0,1};
    wsColor p3 = {1,0,0};
    wsColor p4 = {0,1,0};
    wsColor p5 = {0,0,1};
    wsColor p6 = {0,1,0};
    wsColor p7 = {1,0,0};
    wsColor p8 = {0,0,1};
    wsColor pixels[8] = {p1,p2,p3,p4,p5,p6,p7,p8};


    ws2812b_setColor(pixels, 8);
   while(1){
    int l = 0;

    if (l=0){
        for(l = 0; l <= 360; l++){
            wsColor c1 = HSBtoRGB(l, 1, 0.1);
            wsColor c2 = HSBtoRGB(l+11, 1, 0.1);
            wsColor c3 = HSBtoRGB(l+22, 1, 0.1);
            wsColor c4 = HSBtoRGB(l+33, 1, 0.1);
            wsColor c5 = HSBtoRGB(l+44, 1, 0.1);
            wsColor c6 = HSBtoRGB(l+55, 1, 0.1);
            wsColor c7 = HSBtoRGB(l+66, 1, 0.1);
            wsColor c8 = HSBtoRGB(l+77, 1, 0.1);
            wsColor c[8] = {c1,c2,c3,c4,c5,c6,c7,c8};
            ws2812b_setColor(c, 8);
        }
    }
    else if (l=360){
        for(l = 360; l >=0; l--){
            wsColor c1 = HSBtoRGB(l, 1, 0.1);
            wsColor c2 = HSBtoRGB(l+11, 1, 0.1);
            wsColor c3 = HSBtoRGB(l+22, 1, 0.1);
            wsColor c4 = HSBtoRGB(l+33, 1, 0.1);
            wsColor c5 = HSBtoRGB(l+44, 1, 0.1);
            wsColor c6 = HSBtoRGB(l+55, 1, 0.1);
            wsColor c7 = HSBtoRGB(l+66, 1, 0.1);
            wsColor c8 = HSBtoRGB(l+77, 1, 0.1);
            wsColor c[8] = {c1,c2,c3,c4,c5,c6,c7,c8};
            ws2812b_setColor(c, 8);
        }
    }
   }
}