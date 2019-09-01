void setup()
{
	Serial.begin(9600);

	cmd_init();
	Serial.println(F("[init] cmd"));

	motor_init();
	Serial.println(F("[init] motor"));

	head_init();
	Serial.println(F("[init] head"));

	ir_dist_init();
	Serial.println(F("[init] ir distance"));

	ir_rcv_init();
	Serial.println(F("[init] ir receiver"));

	gps_init();
	Serial.println(F("[init] gps"));

	Serial.println(F("[init] done"));

	//delay(5 * 1000);
	//motor_demo();
}


void loop()
{
	while (Serial.available())
	{
		cmd_put(Serial);
	}

	ir_dist_loop();
	ir_rcv_loop();

	gps_loop();
}
