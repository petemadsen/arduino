const int ir_first_pin = pin_ir4;
const int ir_last_pin = pin_ir4;


#define IR_DIST_TRIGGERED 0


void ir_dist_init()
{
	pinMode(pin_ir1, INPUT);
//gps	pinMode(pin_ir2, INPUT);
//gps	pinMode(pin_ir3, INPUT);
	pinMode(pin_ir4, INPUT);
}


void ir_dist_loop()
{
	for (int pin=ir_first_pin; pin<=ir_last_pin; ++pin)
	{
		if (digitalRead(pin) == IR_DIST_TRIGGERED)
		{
			Serial.print(F("[ird] "));
			Serial.println(pin);
		}
	}
}


bool ir_dist_cmd(int cmd, Stream& io)
{
	switch (cmd)
	{
	case 's':
		io.print("[ird]");
		for (int pin=ir_first_pin; pin<=ir_last_pin; ++pin)
		{
			int val = digitalRead(pin);
			io.print(" ");
			io.print(val);
		}
		io.println();
		return true;
	default:
		io.println(F("[ird] I don't know("));
		io.print(cmd);
		io.println(F(")"));
		return false;
	}

	return false;
}
