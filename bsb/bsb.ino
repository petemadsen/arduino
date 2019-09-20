#include <TM1638.h>


TM1638 bsb(2, 3, 4);


bool cur_player;
uint8_t player_score[2];
uint8_t player_score_positions[2];


void setup()
{
  Serial.begin(9600);
  
  bsb.setupDisplay(true, 0);

  reset_board();

  Serial.println(F("Ready"));
}


void reset_board()
{
  intro(bsb);
  
  cur_player = 0;
  
  player_score[0] = player_score[1] = 0;
  player_score_positions[0] = 3;
  player_score_positions[0] = 7;

  bsb.setDisplayToString(" 000 000");
  bsb.setLEDs(0x0f);
}


byte prev_buttons = 0;
unsigned long pressed = 0; // time when button was pressed


void loop()
{
  display_loop(bsb, cur_player);
  
  byte buttons = bsb.getButtons();

  // just pressed
  if(buttons != 0 && prev_buttons == 0)
  {
    prev_buttons = buttons;
    pressed = millis();
    return;
  }

  if(prev_buttons != 0)
  {
    unsigned long secs = (millis() - pressed) / 1000L;
    Serial.print("secs: ");
    Serial.println(secs);
    Serial.println(prev_buttons);
    if(secs > 2)
    {
      prev_buttons = 0;
      reset_board();
      return;
    }
  }

  // released
  if(buttons == 0 && prev_buttons != 0)
  {
    uint8_t points = 0;
    switch(prev_buttons) {
      case 128:
        // next player
        cur_player = (cur_player + 1) % 2;
        //bsb.setDisplayDigit(1, 0, false);
        //bsb.setDisplayDigit(2, 4, false);
        bsb.clearDisplayDigit(0, 0);
        bsb.clearDisplayDigit(4, 0);

        intro_player(bsb, cur_player);
        break;
      case 64: points += 1;
      case 32: points += 1;
      case 16: points += 1;
      case 8:  points += 1;
      case 4:  points += 1;
      case 2:  points += 1;
      case 1:  points += 1;
      default:
        break;
    }
    if(points != 0)
    {
      player_score[cur_player] += points;
      points = player_score[cur_player];

      Serial.print("player: ");
      Serial.println(cur_player);
      Serial.print("score: ");
      Serial.print(player_score[0]);
      Serial.print(" / ");
      Serial.println(player_score[1]);
      
      
      bsb.setDisplayDigit(points % 10, 4 + cur_player * 4 - 1, false);
      points /= 10;
      bsb.setDisplayDigit(points % 10, 4 + cur_player * 4 - 2, false);
      points /= 10;
      bsb.setDisplayDigit(points % 10, 4 + cur_player * 4 - 3, false);

      intro_player(bsb, cur_player);

      bsb.setLEDs(cur_player ? 0xf0 : 0x0f);
    }

    prev_buttons = 0;
  }
}

