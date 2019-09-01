void setup()
{
	Serial.begin(115200);
	Serial3.begin(115200);

	Serial.println("--init");
	VNH2SP30_begin();

	Serial.println("--ready");
}


void loop()
{
	while (Serial.available())
		parse(Serial.read());

	while (Serial3.available())
	{
		uint8_t ch = Serial3.read();
		Serial.print((char)ch);
		parse(ch);
	}
}


static uint8_t last_ch = ' ';
void parse(uint8_t ch)
{
	if (last_ch != '/')
	{
		last_ch = ch;
		return;
	}

	switch (ch)
	{
	case 'w':
		car_throttle_down(true);
		break;
	case 's':
		car_throttle_down(false);
		break;
	case '1':
		car_direction_switch(1);
		break;
	case '2':
		car_direction_switch(0);
		break;
	case '3':
		car_direction_switch(-1);
		break;
	case '9':
		car_speed_slow(false);
		break;
	case '0':
		car_speed_slow(true);
		break;
	}
}
