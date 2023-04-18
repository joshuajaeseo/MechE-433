// initialize SPI1
void initSPI() {
    // Pin B14 has to be SCK1
    // Turn of analog pins
    //...
    // Make an output pin for CS
    //...
    //...
    // Set SDO1
    //...
    // Set SDI1
    //...

    // setup SPI1
    SPI1CON = 0; // turn off the spi module and reset it
    SPI1BUF; // clear the rx buffer by reading from it
    SPI1BRG = 1000; // 1000 for 24kHz, 1 for 12MHz; // baud rate to 10 MHz [SPI1BRG = (48000000/(2*desired))-1]
    SPI1STATbits.SPIROV = 0; // clear the overflow bit
    SPI1CONbits.CKE = 1; // data changes when clock goes from hi to lo (since CKP is 0)
    SPI1CONbits.MSTEN = 1; // master operation
    SPI1CONbits.ON = 1; // turn on spi 
}


// send a byte via spi and return the response
unsigned char spi_io(unsigned char o) {
  SPI1BUF = o;
  while(!SPI1STATbits.SPIRBF) { // wait to receive the byte
    ;
  }
  return SPI1BUF;
}

init main(){
    //all init functions

    //make array of precomputed sines
    // t= 0;
    while(1){
        //make a sine(t) voltage to send to the chip
        //float s = sine(t) from 0 to 3.3v
        unsigned int st = s from 0 to 1023

        unsigned short t = 0b111<<12 //can't use set bits registers bc this is for the DAC chip
        unsigned char a = 0b1
        t = t | b << 15

        //make the 2 8-bit numbers for spi_io
        [a_or b 1 1 1 <10 bit number> 0 0] is a 16 bit number

        //make chip select go low
        //send high 8 bits from 16 bit number, first 8 numbers spi_io(t>>8) bit shifting to turn into 8 bit number
        //send low 8 bits from 16 bit number, last 8 numbers spi_io(t&0xFF)
        //make cs go high

        //send the sine voltage
        //make a triangle voltage
        //send to chip
        //delay .5ms

        //if i want

        fn(unsigned int v, int channel) //channel is 0 or 1, v is 0-2013
    }
}