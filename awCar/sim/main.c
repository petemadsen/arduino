#include <stdlib.h>
#include <stdio.h>



extern void motor_forward();
extern void motor_backward();
extern void motor_speed(int speed);



static int car_direction = 0; // -1 back, 0 stop, +1 forward

#define CAR_FAST_SPEED 9
#define CAR_SLOW_SPEED 5
static int car_speed = 0;
static int car_max_speed = CAR_SLOW_SPEED;

static int car_throttle = 0;


void my_log(const char* what)
{
	int len = printf("D %02d  S %d/%d  T %d | ",
			car_direction, car_speed, car_max_speed, car_throttle);
	while (++len < 30)
		printf(" ");
	printf("%s\n", what);
}

static void my_log_nl(const char* what)
{
	printf("\n");
	my_log(what);
}


static void car_motors_off()
{
	if (car_speed == 0)
		return;

	while (--car_speed != 0)
	{
		motor_speed(car_speed);
	}
	motor_speed(0);
}


static void car_motors_set_direction()
{
	switch (car_direction)
	{
	case 1:
		motor_forward();
		break;
	case -1:
		motor_backward();
		break;
	case 0:
		break;
	}
}


static void car_motors_on()
{
	if (car_direction == car_max_speed)
		return;

	// slow down
	if (car_speed > car_max_speed)
	{
		while (--car_speed != car_max_speed)
		{
			motor_speed(car_speed);
		}
		motor_speed(car_speed);
		return;
	}

	while (++car_speed != car_max_speed)
	{
		motor_speed(car_speed);
	}
	motor_speed(car_speed);
}


static void car_direction_switch(int dir)
{
	int prev_dir = car_direction;
	car_direction = dir;

	// basically, stop, no matter what throttle says
	if (dir == 0)
	{
		my_log("stop direction");
		car_motors_off();
		return;
	}

	// moving same direction? nothing to do then.
	if (prev_dir == dir)
		return;

	// change direction
	if (car_throttle)
	{
		my_log("change direction");
		car_motors_off();
		car_motors_set_direction();
		car_motors_on();
	}
}


static void car_speed_slow(int slow)
{
	if (slow)
	{
		my_log("SLOW");
		car_max_speed = CAR_SLOW_SPEED;
		if (car_throttle)
			car_motors_on();
	}
	else
	{
		my_log("FAST");
		car_max_speed = CAR_FAST_SPEED;
		if (car_throttle)
			car_motors_on();
	}
}


static void car_throttle_down(int pressed)
{
	if (pressed)
	{
		my_log("THROTTLE");
		car_throttle = 1;
		if (car_direction != 0)
			car_motors_on();
	}
	else
	{
		my_log("STOP");
		car_throttle = 0;
		car_motors_off();
	}
}


int main()
{
	my_log_nl("--- drive forward, slowly");
	car_direction_switch(+1);
	car_speed_slow(1);
	car_throttle_down(1);

	my_log_nl("--- STOP");
	car_throttle_down(0);

	my_log_nl("--- drive forward, fast");
	car_speed_slow(0);
	car_throttle_down(1);

	my_log_nl("--- drive forward, slow/fast/slow");
	car_speed_slow(1);
	car_speed_slow(0);
	car_speed_slow(1);

	my_log_nl("--drive backward");
	car_direction_switch(-1);

	my_log_nl("--drive forward/backward/forward");
	car_direction_switch(+1);
	car_direction_switch(-1);
	car_direction_switch(+1);
	car_direction_switch(0);

	my_log_nl("--STOP");
	car_throttle_down(0);

	return 0;
}
