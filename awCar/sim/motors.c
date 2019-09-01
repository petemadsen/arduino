#include <stdlib.h>
#include <stdio.h>
#include <math.h>


extern void my_log(const char* what);


static int m_motor_speed = 0;


void motor_speed(int speed)
{
	my_log("<<<<<<<<<<<<<<<<<< MOTOR SPEED");
	if (abs(m_motor_speed - speed) > 1)
	{
		my_log(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> BOOM");
		printf("%d - %d\n", m_motor_speed, speed);
		return;
	}
	m_motor_speed = speed;

	// TODO PWM to motor
}


void motor_forward()
{
	my_log("<<<<<<<<<<<<<<<<<< MOTOR FORWARD");
	if (m_motor_speed != 0)
	{
		my_log(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> BOOM");
		return;
	}

	// TODO set pins
}


void motor_backward()
{
	my_log("<<<<<<<<<<<<<<<<<< MOTOR BACKWARD");
	if (m_motor_speed != 0)
	{
		my_log(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> BOOM");
		return;
	}

	// TODO set pins
}

