/****************************************************************************
 *
 * * Select Arduino Uno as board
 *
 **/
#include <MicroView.h>

const int RED = 5;
const int GREEN = 6;
const int WIRE = 3;
const int BUZZER = 2;

const int MAX_RUN_SECS = 180;

bool running = false;
unsigned long started = 0L;
unsigned long running_update = 0L;
int running_secs = 0;
// set when game finishes - used to do a proper screen swap when GREEN pressed
bool over = false;

void setup()
{
	pinMode(RED, INPUT);
	pinMode(GREEN, INPUT);
	pinMode(WIRE, INPUT);
	pinMode(BUZZER, OUTPUT);

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

	digitalWrite(BUZZER, HIGH);
	delay(100);
	digitalWrite(BUZZER, LOW);
	delay(100);
	digitalWrite(BUZZER, HIGH);
	delay(100);
	digitalWrite(BUZZER, LOW);

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
	running_secs = 0L;
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
	uView.print(running_secs);
	if (running_secs % 2)
		uView.circleFill(32, 30, 10, WHITE, NORM);
	else
		uView.circle(32, 30, 10, WHITE, NORM);
	uView.display();

	running_secs += 1;
	if (running_secs > MAX_RUN_SECS)
		game_lost();
}

bool read_button(int key)
{
	if (digitalRead(key))
	{
		while (digitalRead(key))
			delay(50);
		return true;
	}

	return false;
}

void loop()
{
	if (running)
	{
		if (read_button(WIRE))
			game_lost();
		// else if (read_button(GREEN))
		// 	game_won();
		else if (read_button(RED))
			game_cancel();
		else
			game_running();
	}
	else
	{
		if (read_button(GREEN))
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
