/**
 * http://donalmorrissey.blogspot.com/2010/04/sleeping-arduino-part-5-wake-up-via.html
 */
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>


// time secs to choose from
#define JOB_FROM	300
#define JOB_TO		400


const int pin_motor = 6;
const int pin_buzzer = 5;

volatile int f_wdt = 1;

#define LED_PIN LED_BUILTIN

const int secs_per_sleep = 8;
int sleeping_secs = 0;
int do_job_at_secs = secs_per_sleep;


ISR(WDT_vect)
{
	if (f_wdt == 0)
	{
		f_wdt=1;
	}
	else
	{
		Serial.println("WDT Overrun!!!");
	}
}


void enter_wonderland()
{
	Serial.println(F("--enter wonderland "));
	delay(100);

//	set_sleep_mode(SLEEP_MODE_PWR_SAVE);   /* EDIT: could also use SLEEP_MODE_PWR_DOWN for lowest power consumption. */
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();

	/* Now enter sleep mode. */
	sleep_mode();

	/* The program will continue from here after the WDT timeout*/
	sleep_disable(); /* First thing to do is disable sleep. */

	/* Re-enable the peripherals. */
	power_all_enable();
}


void setup()
{
	Serial.begin(115200);

	pinMode(LED_PIN, OUTPUT);
	pinMode(pin_motor, OUTPUT);
	pinMode(pin_buzzer, OUTPUT);

	/*** Setup the WDT ***/

	/* Clear the reset flag. */
	MCUSR &= ~(1<<WDRF);

	/* In order to change WDE or the prescaler, we need to
	 * set WDCE (This will allow updates for 4 clock cycles).
	 */
	WDTCSR |= (1<<WDCE) | (1<<WDE);

	/* set new watchdog timeout prescaler value */
	WDTCSR = 1<<WDP0 | 1<<WDP3; /* 8.0 seconds */

	/* Enable the WD interrupt (note no reset). */
	WDTCSR |= _BV(WDIE);

	// -- ready
	Serial.println(F("--ready"));
	delay(100);
}


void loop()
{
	sleeping_secs += secs_per_sleep;
	Serial.print(F("--sleep "));
	Serial.print(sleeping_secs);
	Serial.print(F(" > "));
	Serial.println(do_job_at_secs);
	delay(100);

	// -- something to do?
	if (sleeping_secs >= do_job_at_secs)
	{
		Serial.println(F("--JOB"));

		make_noise();

		// next job
		sleeping_secs = 0;
		do_job_at_secs = random(JOB_FROM, JOB_TO);
	}

	// -- to do bed again
	if (f_wdt == 1)
	{
		/* Toggle the LED */
		digitalWrite(LED_PIN, !digitalRead(LED_PIN));

		/* Don't forget to clear the flag. */
		f_wdt = 0;

		/* Re-enter sleep mode. */
		enter_wonderland();
	}
}


void make_noise()
{
	Serial.println(F("--NOISE 1"));
	analogWrite(pin_motor, 102);
	delay(2000);
	analogWrite(pin_motor, 153);
	delay(2000);
	analogWrite(pin_motor, 204);
	delay(2000);
	// off
	analogWrite(pin_motor, 0);

	Serial.println(F("--NOISE 2"));
	for (int i=0; i<3; ++i)
	{
		delay(random(250, 500));

		for (int k=0; k<6; ++k)
		{
			delay(random(50, 100));
			digitalWrite(pin_buzzer, HIGH);
			delay(random(50, 100));
			digitalWrite(pin_buzzer, LOW);
		}
#if 0
		delay(random(250, 2500));
		digitalWrite(pin_buzzer, HIGH);
		delay(100);
		digitalWrite(pin_buzzer, LOW);
#endif
	}

	Serial.println(F("--NOISE 3"));

	delay(100);
}
