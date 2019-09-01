static int m_motor_speed = 0;
static int m_motor_direction = CW;


void motor_speed(int speed)
{
	my_log("<<<<<<<<<<<<<<<<<< MOTOR SPEED");
	if (abs(m_motor_speed - speed) > 1)
	{
		my_log(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> BOOM");
//		printf("%d - %d\n", m_motor_speed, speed);
		return;
	}
	m_motor_speed = speed;

	// TODO PWM to motor
	if (m_motor_speed == 0)
	{
		motorOff(0);
		motorOff(1);
		delay(100);
	}
	else
	{
		int s = map(m_motor_speed, 0, 10, 0, 125);
		Serial.print(m_motor_speed);
		Serial.print(" =s> ");
		Serial.println(s);
		motorGo(0, m_motor_direction, s);
		motorGo(1, m_motor_direction, s);
		delay(100);
	}
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
	m_motor_direction = CCW;
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
	m_motor_direction = CW;
}
