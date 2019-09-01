#include <Servo.h>

#define SERVO_CENTER 87

Servo m_head;
int m_head_pos = 0; // +x = left, 0 = center, -x = right
const int m_head_max = 90;


void head_init()
{
	m_head.attach(pin_head);
	m_head.write(SERVO_CENTER); // center
}


void head_center()
{
	head_to(0);
}


void head_max_left()
{
	head_to(+m_head_max);
}


void head_max_right()
{
	head_to(-m_head_max);
}


void head_to(int pos)
{
	Serial.print("head-to from ");
	Serial.print(m_head_pos);
	Serial.print(" to ");
	Serial.print(pos);
	Serial.println();
  
	if (pos == m_head_pos)
		return;

	int dir = pos < m_head_pos ? -1 : +1;

	while (m_head_pos != pos)
	{
		m_head_pos += dir;

		m_head.write(m_head_pos + SERVO_CENTER);
    
		Serial.print("##");
		Serial.print(m_head_pos);
		Serial.print(" / ");
		Serial.println(m_head_pos + 90);
		delay(25);
	}
}


bool head_cmd(int cmd, Stream& io)
{
	switch (cmd)
	{
	case 'l':
		head_max_left();
		return true;
	case 'r':
		head_max_right();
		return true;
	case 'c':
		head_center();
		return true;
	default:
		io.println(F("[head] I don't know("));
		io.print(cmd);
		io.println(F(")"));
	}

	return false;
}
