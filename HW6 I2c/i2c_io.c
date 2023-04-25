#include "nu32dip.h"          // constants, funcs for startup and UART
#include "i2c_master_noint.h"

void turn_on_gp7(void);
void turn_off_gp7(void);
int read_from_GP0(void);


int main(void) {
  
  NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init

  i2c_master_setup();   //init i2c

  //init the chip, set the IODIR register for GPo is in, GP7 is out
  i2c_master_start();//send start
  i2c_master_send(0b01000000); //send address with write bit
  i2c_master_send(0x00);//send register name IODIR
  i2c_master_send(0b00000001);
  i2c_master_stop(); //send stop
  //IODIR = 0b10000000;

  while (1) {
    //blink the green or yellow led
    NU32DIP_YELLOW = 0; // on
    _CP0_SET_COUNT(0);
  	while(_CP0_GET_COUNT()<24000000){}
    NU32DIP_YELLOW = 1; // off
  	_CP0_SET_COUNT(0);
  	while(_CP0_GET_COUNT()<24000000){}


    // turn_on_gp7();//turn on GP7 
  	// _CP0_SET_COUNT(0);
  	// while(_CP0_GET_COUNT()<24000000){}
    // //turn off GP7
    // turn_off_gp7();
  	// _CP0_SET_COUNT(0);
  	// while(_CP0_GET_COUNT()<24000000){}


    int read = read_from_GP0(); //read from GP0
    if (read == 0) {
      turn_on_gp7();//turn on GP7
      _CP0_SET_COUNT(0);
    	while(_CP0_GET_COUNT()<24000000){}
      turn_off_gp7();
      _CP0_SET_COUNT(0);
    	while(_CP0_GET_COUNT()<24000000){}
    }
    else{}
  }
  return 0;
}

void turn_on_gp7(){
  i2c_master_start();//send start
  i2c_master_send(0b01000000); //send address with write bit
  i2c_master_send(0x0A);//send register name OLAT
  i2c_master_send(0b10000000);//send the value to turn on gp7
  i2c_master_stop(); //send stop
}

void turn_off_gp7(){
  i2c_master_start();//send start
  i2c_master_send(0b01000000); //send address with write bit
  i2c_master_send(0x0A);//send register name OLAT
  i2c_master_send(0b00000000);//send the value to turn off gp7
  i2c_master_stop(); //send stop
}

int read_from_GP0(){
  i2c_master_start(); //send start bit
  i2c_master_send(0b01000000); //send address with write bit
  i2c_master_send(0x09);//send register name OLAT
  i2c_master_restart(); //restart
  i2c_master_send(0b01000001);//send address with read bit
  unsigned char r = i2c_master_recv();
  i2c_master_ack(1);//send the NACK bit
  i2c_master_stop();//send the stop bit
  return (r&0b1);
}


