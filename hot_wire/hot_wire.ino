/****************************************************************************
 *
 * * Select Arduino Uno as board
 *
 **/
#include <MicroView.h>

const int RED = 5;
const int GREEN = 6;
const int CONTROL = 3;
const int BUZZER = 2;

const int WIRE = A2;
const int RIGHT = A3;
const int LEFT = A4;

int MAX_RUN_SECS = 180;

bool running = false;
unsigned long started = 0L;
unsigned long running_update = 0L;
int secs_remaining = 0;
// set when game finishes - used to do a proper screen swap when GREEN pressed
bool over = false;

void setup()
{
	pinMode(RED, INPUT);
	pinMode(GREEN, INPUT);
	pinMode(WIRE, INPUT_PULLUP);
	pinMode(RIGHT, INPUT_PULLUP);
	pinMode(LEFT, INPUT_PULLUP);

	pinMode(CONTROL, OUTPUT);
	pinMode(BUZZER, OUTPUT);

	digitalWrite(CONTROL, LOW);
	digitalWrite(BUZZER, LOW);

	Serial.begin(9600);

	Serial.print(F("display: "));
	Serial.print(LCDWIDTH);
	Serial.print(F("x"));
	Serial.println(LCDHEIGHT);

	uView.begin();
	game_init();
}

void game_init()
{
	Serial.println("INIT");

	running = false;
	over = false;

	uView.clear(PAGE);
	uView.setCursor(0, 0);
	uView.setFontType(0);
	uView.println("Start:");
	uView.println("Gruen");
	uView.display();
}

void game_lost()
{
	Serial.println("LOST");

	running = false;
	over = true;

	for (int i = 0; i < 3; ++i)
	{
		digitalWrite(BUZZER, HIGH);
		delay(200);
		digitalWrite(BUZZER, LOW);
		delay(200);
	}

	uView.clear(PAGE);
	uView.setFontType(1);
	uView.setCursor(0, 0);
	uView.print("SCHADE!");
	uView.circleFill(32, 30, 10, WHITE, NORM);
	uView.display();
}

void game_won()
{
	Serial.println("WON");

	running = false;
	over = true;

	uView.clear(PAGE);
	uView.setCursor(0, 0);
	uView.setFontType(1);
	uView.print("G U T !");
	uView.circleFill(32, 30, 10, WHITE, NORM);
	uView.display();
}

void game_cancel()
{
	Serial.println("CANCEL");

	running = false;
	over = true;

	uView.clear(PAGE);
	uView.setCursor(0, 0);
	uView.setFontType(1);
	uView.print(" STOP");
	uView.circleFill(32, 30, 10, WHITE, NORM);
	uView.display();
}

void game_run()
{
	Serial.println("RUN");

	running = true;
	over = false;

	started = millis();
	running_update = 0L;
	secs_remaining = MAX_RUN_SECS;
}

void game_running()
{
	unsigned long now = millis();
	if ((now - running_update) < 1000)
		return;
	running_update = now;

	uView.clear(PAGE);
	uView.setCursor(0, 0);
	uView.setFontType(1);
	uView.print("Los:");
	uView.print(secs_remaining);
	if (secs_remaining % 2)
		uView.circleFill(32, 30, 10, WHITE, NORM);
	else
		uView.circle(32, 30, 10, WHITE, NORM);
	uView.display();

	secs_remaining -= 1;
	if (secs_remaining < 0)
		game_lost();
}

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
	if (read_button(WIRE, HIGH))
		Serial.println("--wire");
	if (read_button(LEFT, HIGH))
		Serial.println("--left");
	if (read_button(RIGHT, HIGH))
		Serial.println("--right");
	return;

	if (running)
	{
		if (read_button(WIRE, HIGH))
			game_lost();
		// else if (read_button(GREEN))
		// 	game_won();
		else if (read_button(RED, LOW))
			game_cancel();
		else
			game_running();
	}
	else
	{
		if (read_button(GREEN, LOW))
		{
			Serial.println("GREEN");
			if (over)
			{
				game_init();
			}
			else
			{
				game_run();
				game_running();
			}
		}
	}
}
