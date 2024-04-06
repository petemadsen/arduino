/****************************************************************************
 *
 * RUN:
 * * Select Arduino Uno as board
 *
 * TODO:
 * * Nothing
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
#define GAME_IDLE 6
int _game_state = GAME_SPLASH;
int pole_to_win = WIRE;

unsigned long running_last_update = 0L;
const int INIT_RUN_SECS = 90;
int max_run_secs = INIT_RUN_SECS;
int secs_remaining = 0;

#define SMILEY_NUM_TYPES 3
#define SMILEY_WON 0
#define SMILEY_LOST 1
#define SMILEY_ABORTED 2

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
	uView.println("Du");
	uView.display();
	delay(1000);
	digitalWrite(LED_RED, LOW);

	smiley(32, 38, 9, SMILEY_WON, true);
	delay(3 * 1000);

	beep();

	_game_state = GAME_SELECT;
}

void game_select()
{
	Serial.println("SELECT");
	max_run_secs = INIT_RUN_SECS;
	digitalWrite(LED_GREEN, HIGH);
	digitalWrite(LED_RED, HIGH);

	game_select_redraw();
	idle_reset();

	// wait for left/right pole decision
	while (pole_to_win != POLE_LEFT && pole_to_win != POLE_RIGHT)
	{
		if (idle_true())
		{
			_game_state = GAME_IDLE;
			return;
		}
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
			idle_reset();
			if (max_run_secs > 30)
			{
				max_run_secs -= 30;
				game_select_redraw();
			}
		}
		if (read_button(BUTTON_RED, LOW))
		{
			idle_reset();
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

void game_won()
{
	Serial.println("WON");

	uView.clear(PAGE);
	uView.setCursor(0, 0);
	uView.setFontType(1);
	uView.print("SUPER");
	uView.display();

	smiley(32, 30, 16, SMILEY_WON, false);

	digitalWrite(BUZZER, HIGH);
	delay(100);
	digitalWrite(BUZZER, LOW);
	delay(100);
	digitalWrite(BUZZER, HIGH);
	delay(1000);
	digitalWrite(BUZZER, LOW);

	smiley(32, 30, 16, SMILEY_WON, true);

	wait_for_green_button_and_game_select();
}

void game_lost()
{
	Serial.println("LOST");

	uView.clear(PAGE);
	uView.setFontType(1);
	uView.setCursor(0, 0);
	uView.print("SCHADE");
	uView.display();

	smiley(32, 30, 16, SMILEY_LOST, false);

	digitalWrite(BUZZER, HIGH);
	delay(2000);
	digitalWrite(BUZZER, LOW);

	wait_for_green_button_and_game_select();
}

void game_aborted()
{
	Serial.println("ABORTED");

	uView.clear(PAGE);
	uView.setCursor(0, 0);
	uView.setFontType(1);
	uView.print("NA GUT");
	uView.display();

	smiley(32, 30, 16, SMILEY_ABORTED, false);

	for (int i = 0; i < 1; ++i)
	{
		digitalWrite(BUZZER, HIGH);
		delay(200);
		digitalWrite(BUZZER, LOW);
		delay(200);
	}

	wait_for_green_button_and_game_select();
}

void game_idle()
{
	digitalWrite(LED_GREEN, HIGH);
	digitalWrite(LED_RED, LOW);

	uint8_t x = uView.getLCDWidth() / 2;
	uint8_t y = uView.getLCDHeight() / 2;
	uint8_t s = 10;
	uint8_t t = 0;

	uint8_t dx = 1;
	uint8_t dy = 1;

	unsigned long last_update = millis();

	while (!read_button(BUTTON_GREEN, LOW))
	{
		unsigned long now = millis();
		if ((now - last_update) < 50)
			continue;
		last_update = now;

		x += dx;
		if ((x + s + 1) >= uView.getLCDWidth())
		{
			dx = -1;
			t = (t + 1) % SMILEY_NUM_TYPES;
		}
		else if ((x - s) <= 0)
		{
			dx = +1;
			t = (t + 1) % SMILEY_NUM_TYPES;
		}

		y += dy;
		if ((y + s + 1) >= uView.getLCDHeight())
		{
			dy = -1;
			t = (t + 1) % SMILEY_NUM_TYPES;
		}
		else if ((y - s) <= 0)
		{
			dy = +1;
			t = (t + 1) % SMILEY_NUM_TYPES;
		}

		uView.clear(PAGE);
		smiley(x, y, s, t, false);
	}

	_game_state = GAME_SELECT;
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
	case GAME_IDLE:
		game_idle();
		break;
	}
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

void wait_for_green_button_and_game_select()
{
	Serial.println("IDLE");
	idle_reset();

	// Wait for green button to be pressed
	digitalWrite(LED_GREEN, HIGH);
	// FIXME: Do we need to turn GREEN off???
	while (!read_button(BUTTON_GREEN, LOW))
	{
		if (idle_true())
		{
			_game_state = GAME_IDLE;
			return;
		}
		_game_state = GAME_SELECT;
	}
}

void beep()
{
	digitalWrite(BUZZER, HIGH);
	delay(100);
	digitalWrite(BUZZER, LOW);
}

void smiley(const uint8_t x, const uint8_t y, const uint8_t size, const uint8_t type, bool wink)
{
	const uint8_t eye_size = size / 4;
	const uint8_t mouth_height = y + 2 * eye_size;

	// face
	uView.circle(x, y, size, WHITE, NORM);
	// left eye
	uView.circle(x - 2 * eye_size, y - 2, eye_size, WHITE, NORM);
	// right eye
	uView.circle(x + 2 * eye_size, y - 2, eye_size, WHITE, NORM);
	// mouth
	uView.line(x - 2 * eye_size, mouth_height, x + 2 * eye_size, mouth_height);
	if (type == SMILEY_WON)
	{
		uView.pixel(x - 2 * eye_size - 1, mouth_height - 1);
		uView.pixel(x + 2 * eye_size, mouth_height - 1);
	}
	else if (type == SMILEY_LOST)
	{
		uView.pixel(x - 2 * eye_size - 1, mouth_height + 1);
		uView.pixel(x + 2 * eye_size, mouth_height + 1);
	}
	//
	uView.display();

	if (wink)
	{
		// wink - close right eye
		delay(1000);
		uView.circleFill(x + 2 * eye_size, y - 2, eye_size, WHITE, NORM);
		uView.display();
		// open right eye
		delay(500);
		uView.circleFill(x + 2 * eye_size, y - 2, eye_size, BLACK, NORM);
		uView.circle(x + 2 * eye_size, y - 2, eye_size, WHITE, NORM);
		uView.display();
	}
}

unsigned long _idle_started = 0;

void idle_reset()
{
	_idle_started = millis();
}

bool idle_true()
{
	return (millis() - _idle_started) > 10L * 1000L;
}