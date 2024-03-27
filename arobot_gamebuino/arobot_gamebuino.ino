//imports the SPI library (needed to communicate with Gamebuino's screen)
#include <SPI.h>
//imports the Gamebuino library
#include <Gamebuino.h>
#include "gb_emu.h"
Gamebuino gb;
//GbEmu gb;

#include <Wire.h>


// the setup routine runs once when Gamebuino starts up
void setup()
{
  Serial.begin(9600);
  
  // initialize the Gamebuino object
  gb.begin();
  //gb.battery.show = true;
  //display the main menu:
  gb.titleScreen(F("My arobot game"));
  //gb.popup(F("Let's go!"), 100);


  Wire.begin();
}


#define I2C_MAX 10
byte i2c_found = 0;
byte i2c_devices[I2C_MAX];

// the loop routine runs over and over again forever
void loop()
{
  //updates the gamebuino (the display, the sound, the auto backlight... everything)
  //returns true when it's time to render a new frame (20 times/second)
  if (gb.update())
  {      
    //prints Hello World! on the screen
    gb.display.println(F("Hello World!"));
    //declare a variable named count of type integer :
    int count;
    //get the number of frames rendered and assign it to the "count" variable
    count = gb.frameCount;
    //prints the variable "count"
    gb.display.println(count);

#if 1
    i2c_found = 0;
    for (int addr = 0x0; addr < 0x10; ++addr)
    {
      Wire.beginTransmission(addr);
      byte error = Wire.endTransmission();
      if (error == 0)
      {
        i2c_found += 1;
        gb.display.print(F("dev: "));
        gb.display.println(addr);
      //Serial.print(F("Found: "));
      //Serial.println(addr, HEX);
      }
    }
    gb.display.print(F("i2c found: "));
    gb.display.println(i2c_found);
    Serial.println(F("<<done"));
#endif
  }
}
