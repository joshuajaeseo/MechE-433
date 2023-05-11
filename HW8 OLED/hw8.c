#include "nu32dip.h" // constants, functions for startup and UART
#include "i2c_master_noint.h"
#include "mpu6050.h"//does not call i2c by default so try checking this
#include <stdio.h>
#include "ssd1306.h"

void blink(int, int); // blink the LEDs function
void drawString(char*, unsigned char, unsigned char);
void delay(void);

int main(void) {
    NU32DIP_Startup(); // cache on, interrupts on, LED/button init, 
    init_mpu6050();
    ssd1306_setup();

    unsigned char data[14];
    char accel[50];
    char fps_arr[50];
    float az;
    float time;
    float fps; 


    //read whoami
    unsigned char who;
    who = whoami(); 

	// print whoami
    char message[100];
    sprintf(message, "0x%X\r\n", who);
    NU32DIP_WriteUART1(message);
	// if whoami is not 0x68, stuck in loop with LEDs on
    if (who != 0x68){
        while(1){
            NU32DIP_YELLOW = 0; // on
            _CP0_SET_COUNT(0);
            while(_CP0_GET_COUNT()<24000000){}
            NU32DIP_YELLOW = 1; // off
            _CP0_SET_COUNT(0);
            while(_CP0_GET_COUNT()<24000000){}
        }
    }

    while (1) {
        _CP0_SET_COUNT(0);
        burst_read_mpu6050(data);
		// convert data
        az = conv_zXL(data);
        sprintf(accel, "Z acc: %.2f", az);
        drawString(accel, 5, 5);
        ssd1306_update();
        time = _CP0_GET_COUNT();
        time = time/48000000;
        fps = 1/time;
        sprintf(fps_arr, "FPS: %f", fps);
        drawString(fps_arr, 5, 15);
        ssd1306_update();  
    }
}

void delay(void) {
    _CP0_SET_COUNT(0);
    while (_CP0_GET_COUNT() < 48000000) {
    }
}



    // char m_in[100];
	// // wait to print until you get a newline
    // NU32DIP_ReadUART1(m_in,100);

    // while (1) {
	// 	// use core timer for exactly 100Hz loop
    //     _CP0_SET_COUNT(0);
    //     blink(1, 5);

    //     // read IMU
    //     burst_read_mpu6050(d);
	// 	// convert data
    //     ax = conv_xXL(d);

    //     // print out the data
    //     sprintf(m,"%f\r\n", ax);
    //     NU32DIP_WriteUART1(m);
    //     while (_CP0_GET_COUNT() < 48000000 / 2 / 100) {
    //     }
    // }


