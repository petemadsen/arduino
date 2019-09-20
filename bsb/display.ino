#include <TM1638.h>



unsigned long display_last_access = 0;
#define DISPLAY_ENTER_INTERVAL 1000

uint8_t display_loop_cnt = 0;


void display_loop(const TM1638& bsb, uint8_t cur_player)
{
  unsigned long m = millis();
  if((unsigned long)(m - display_last_access) < DISPLAY_ENTER_INTERVAL)
  {
    return;
  }
  display_last_access = m;
  ++display_loop_cnt;

  bsb.clearDisplayDigit(cur_player * 4, display_loop_cnt%2);
}

