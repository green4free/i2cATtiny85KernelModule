// Code for the ATtiny85
#define I2C_SLAVE_ADDRESS 0x4 // Address of the slave
 
#include <TinyWireS.h>
 
int in = 0, ut = 0;
 
void setup()
{
    TinyWireS.begin(I2C_SLAVE_ADDRESS); // join i2c network
    TinyWireS.onReceive(receiveEvent);
    TinyWireS.onRequest(requestEvent);
}
 
void loop()
{
    // This needs to be here
    TinyWireS_stop_check();
}

// Gets called when the ATtiny receives data
void receiveEvent ()
{
    in = TinyWireS.receive();
    analogWrite(1, in);
}

// Gets called when the ATtiny receives an i2c request
void requestEvent()
{
    ut = map(analogRead(A2),0, 1023, 0, 255);
    TinyWireS.send(ut);
}
