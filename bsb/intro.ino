/**
 * Play some intro on the display.
 */

byte intro_frame[] =
{
  0b00111001,
  0b00001001, 0b00001001, 0b00001001, 0b00001001, 0b00001001, 0b00001001,
  0b00001111
};


void intro(const TM1638& bsb)
{
  for(int i=0; i<2; ++i)
  {
    bsb.setLEDs(0xff);
    bsb.setDisplay(intro_frame, 8);
    delay(1 * 500);
    bsb.setLEDs(0x00);
    bsb.clearDisplay();
    delay(1 * 500);
  }
}


void intro_player(const TM1638& bsb, uint8_t player)
{
  for(int i=0; i<2; ++i)
  {
    bsb.setLEDs(player ? 0xf0 : 0x0f);
    delay(1 * 500);
    bsb.setLEDs(0);
    delay(1 * 500);
  }
}

