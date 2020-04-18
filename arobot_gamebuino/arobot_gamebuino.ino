//imports the SPI library (needed to communicate with Gamebuino's screen)
#include <SPI.h>
//imports the Gamebuino library
#include <Gamebuino.h>
//creates a Gamebuino object named gb
Gamebuino gb;

#include <Wire.h>


// the setup routine runs once when Gamebuino starts up
void setup()
{
  Wire.begin();
  
  // initialize the Gamebuino object
  gb.begin();
  gb.battery.show = true;
  //display the main menu:
  gb.titleScreen(F("My first game"));
  gb.popup(F("Let's go!"), 100);
}


#define I2C_MAX 10
byte i2c_found = 0;
byte i2c_devices[I2C_MAX];

// the loop routine runs over and over again forever
void loop()
{
  /*
  int addr = 0x12;
  Wire.requestFrom(addr, 1);
  while (Wire.available())
  {
    char c = Wire.read();
    gb.
  }
  */





  
  
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


    for (int addr = 0; addr < i2c_found; ++addr)
    {
    }
    gb.display.print(F("i2c found "));
    gb.display.println(i2c_found);

    return;
  }



  i2c_found = 0;
  for (int addr = 0x40; addr < 0x41; ++addr)
  {
    Wire.beginTransmission(addr);
    byte error = Wire.endTransmission();
    if (error == 0)
    {
      i2c_found += 1;
      //Serial.print(F("Found: "));
      //Serial.println(addr, HEX);
    }
  }
  //Serial.println(F("<<done"));
}
