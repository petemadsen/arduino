/****************************************************************************
 *
 * * Select Arduino Uno as board
 *
 * TODO:
 * * Add buzzer beep when close to end
 *
 **/
#include <MicroView.h>

const int BUTTON_RED = 5;
const int BUTTON_GREEN = 6;
const int HANDLE = 3;
const int BUZZER = 2;

const int LED_GREEN = A0;
const int LED_RED = A1;
const int WIRE = A2;
const int POLE_RIGHT = A3;
const int POLE_LEFT = A4;

#define GAME_SPLASH 0
#define GAME_SELECT 1
#define GAME_RUNNING 2
#define GAME_WON 3
#define GAME_LOST 4
#define GAME_ABORTED 5
int _game_state = GAME_SPLASH;
int pole_to_win = WIRE;

unsigned long running_last_update = 0L;
const int INIT_RUN_SECS = 90;
int max_run_secs = INIT_RUN_SECS;
int secs_remaining = 0;

void setup()
{
	pinMode(BUTTON_RED, INPUT);
	pinMode(BUTTON_GREEN, INPUT);
	pinMode(WIRE, INPUT_PULLUP);
	pinMode(POLE_RIGHT, INPUT_PULLUP);
	pinMode(POLE_LEFT, INPUT_PULLUP);

	pinMode(HANDLE, OUTPUT);
	pinMode(BUZZER, OUTPUT);
	digitalWrite(HANDLE, LOW);
	digitalWrite(BUZZER, LOW);

	pinMode(LED_GREEN, OUTPUT);
	pinMode(LED_RED, OUTPUT);
	digitalWrite(LED_GREEN, LOW);
	digitalWrite(LED_RED, LOW);

	Serial.begin(9600);

	Serial.print(F("display: "));
	Serial.print(LCDWIDTH);
	Serial.print(F("x"));
	Serial.println(LCDHEIGHT);

	uView.begin();

	_game_state = GAME_SPLASH;
}

void game_splash()
{
	Serial.println("SPLASH");

	uView.clear(PAGE);
	uView.setCursor(0, 0);
	uView.setFontType(1);

	uView.println("Hallo");
	uView.display();
	digitalWrite(LED_GREEN, HIGH);
	delay(1000);
	digitalWrite(LED_GREEN, LOW);
	digitalWrite(LED_RED, HIGH);
	uView.println("Spieler");
	uView.display();
	delay(1000);
	digitalWrite(LED_RED, LOW);

	beep();
	delay(1 * 1000);
	_game_state = GAME_SELECT;
}

void game_select()
{
	Serial.println("SELECT");
	max_run_secs = INIT_RUN_SECS;
	digitalWrite(LED_GREEN, HIGH);
	digitalWrite(LED_RED, HIGH);

	game_select_redraw();

	// wait for left/right pole decision
	while (pole_to_win != POLE_LEFT && pole_to_win != POLE_RIGHT)
	{
		if (read_button(POLE_LEFT, HIGH))
		{
			Serial.println("POLE_LEFT");
			pole_to_win = POLE_RIGHT;
		}
		if (read_button(POLE_RIGHT, HIGH))
		{
			Serial.println("POLE_RIGHT");
			pole_to_win = POLE_LEFT;
		}
		if (read_button(BUTTON_GREEN, LOW))
		{
			if (max_run_secs > 30)
			{
				max_run_secs -= 30;
				game_select_redraw();
			}
		}
		if (read_button(BUTTON_RED, LOW))
		{
			if (max_run_secs < 180)
			{
				max_run_secs += 30;
				game_select_redraw();
			}
		}
	}

	digitalWrite(LED_GREEN, LOW);
	digitalWrite(LED_RED, LOW);
	_game_state = GAME_RUNNING;
}

void game_select_redraw()
{
	uView.clear(PAGE);
	uView.setCursor(0, 0);
	uView.setFontType(1);
	uView.println("L<-?->R");
	uView.print(max_run_secs, DEC);
	uView.print("s");
	uView.display();
}

void game_running()
{
	Serial.println("RUNNING");
	secs_remaining = max_run_secs;
	digitalWrite(LED_RED, HIGH);

	while (true)
	{
		game_running_tick();
		if (secs_remaining <= 0)
		{
			_game_state = GAME_LOST;
			break;
		}

		if (read_button(WIRE, HIGH))
		{
			_game_state = GAME_LOST;
			break;
		}
		if (read_button(pole_to_win, HIGH))
		{
			_game_state = GAME_WON;
			break;
		}
		if (read_button(BUTTON_RED, LOW))
		{
			_game_state = GAME_ABORTED;
			break;
		}
		// FIXME
		if (read_button(BUTTON_GREEN, LOW))
		{
			Serial.println("RUNNING/GREEN pressed");
		}
	}

	digitalWrite(LED_RED, LOW);
	pole_to_win = WIRE;
}

void game_running_tick()
{
	unsigned long now = millis();
	if ((now - running_last_update) < 1000)
		return;
	running_last_update = now;

	uView.clear(PAGE);
	uView.setCursor(0, 0);
	uView.setFontType(1);
	uView.print("Los:");
	uView.print(secs_remaining);
	if (secs_remaining % 2)
	{
		uView.circleFill(32, 30, 10, WHITE, NORM);
	}
	else
	{
		uView.circle(32, 30, 10, WHITE, NORM);
	}
	uView.display();

	if (secs_remaining < 10)
		beep();
	else if (secs_remaining < 30 && secs_remaining % 2)
		beep();

	secs_remaining -= 1;
}

void game_lost()
{
	Serial.println("LOST");

	uView.clear(PAGE);
	uView.setFontType(1);
	uView.setCursor(0, 0);
	uView.print("SCHADE!");
	uView.circleFill(32, 30, 10, WHITE, NORM);
	uView.display();

	digitalWrite(BUZZER, HIGH);
	delay(2000);
	digitalWrite(BUZZER, LOW);

	wait_for_green_button_and_game_select();
}

void game_won()
{
	Serial.println("WON");

	digitalWrite(BUZZER, HIGH);
	delay(100);
	digitalWrite(BUZZER, LOW);
	delay(100);
	digitalWrite(BUZZER, HIGH);
	delay(1000);
	digitalWrite(BUZZER, LOW);

	uView.clear(PAGE);
	uView.setCursor(0, 0);
	uView.setFontType(1);
	uView.print("G U T !");
	uView.circleFill(32, 30, 10, WHITE, NORM);
	uView.display();

	wait_for_green_button_and_game_select();
}

void game_aborted()
{
	Serial.println("ABORTED");

	for (int i = 0; i < 1; ++i)
	{
		digitalWrite(BUZZER, HIGH);
		delay(200);
		digitalWrite(BUZZER, LOW);
		delay(200);
	}

	uView.clear(PAGE);
	uView.setCursor(0, 0);
	uView.setFontType(1);
	uView.print(" STOP");
	uView.circleFill(32, 30, 10, WHITE, NORM);
	uView.display();

	wait_for_green_button_and_game_select();
}

// Checks if button is pressed and waits for it to be released.
bool read_button(int key, int default_state)
{
	if (digitalRead(key) != default_state)
	{
		while (digitalRead(key) != default_state)
			delay(50);
		return true;
	}

	return false;
}

void loop()
{
	switch (_game_state)
	{
	case GAME_SPLASH:
		game_splash();
		break;
	case GAME_SELECT:
		game_select();
		break;
	case GAME_RUNNING:
		game_running();
		break;
	case GAME_WON:
		game_won();
		break;
	case GAME_LOST:
		game_lost();
		break;
	case GAME_ABORTED:
		game_aborted();
		break;
	}
}

void wait_for_green_button_and_game_select()
{
	// Wait for green button to be pressed
	digitalWrite(LED_GREEN, HIGH);
	while (!read_button(BUTTON_GREEN, LOW))
		;
	_game_state = GAME_SELECT;
}

void beep()
{
	digitalWrite(BUZZER, HIGH);
	delay(100);
	digitalWrite(BUZZER, LOW);
}