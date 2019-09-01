int motor1_speed = 255;
int motor2_speed = 255;


void motor_init()
{
	pinMode(motor1_pwm, OUTPUT);
	pinMode(motor1_in_1, OUTPUT);
	pinMode(motor1_in_2, OUTPUT);

	pinMode(motor2_pwm, OUTPUT);
	pinMode(motor2_in_1, OUTPUT);
	pinMode(motor2_in_2, OUTPUT);

	motor_stop();
}


void motor_speed(int s)
{
	motor1_speed = s;
	motor2_speed = s;
}


#if 0
void motor_run()
{
  Serial.println("--loop");

  //Serial.println("");
//For Clock wise motion , in_1 = High , in_2 = Low
  digitalWrite(motor1_in_1,HIGH) ;
  digitalWrite(motor1_in_2,LOW) ;
  analogWrite(motor1_pwm, 128) ;

/*setting pwm of the motor to 255
we can change the speed of rotaion
by chaning pwm input but we are only
using arduino so we are using higest
value to driver the motor  */

  //Clockwise for 3 secs
  delay(3000) ;     

  //For brake
  digitalWrite(motor1_in_1,HIGH) ;
  digitalWrite(motor1_in_2,HIGH) ;
  delay(1000) ;

  //For Anti Clock-wise motion - IN_1 = LOW , IN_2 = HIGH
  digitalWrite(motor1_in_1,LOW) ;
  digitalWrite(motor1_in_2,HIGH) ;
  delay(3000) ;

  //For brake
  motor_stop();
  delay(1000) ;
}
#endif


void motor_stop()
{
	digitalWrite(motor1_in_1, HIGH);
	digitalWrite(motor1_in_2, HIGH);

	digitalWrite(motor2_in_1, HIGH);
	digitalWrite(motor2_in_2, HIGH);
}


void motor_forward()
{
	digitalWrite(motor1_in_1, HIGH);
	digitalWrite(motor1_in_2, LOW);
	analogWrite(motor1_pwm, motor1_speed);

	digitalWrite(motor2_in_1, HIGH);
	digitalWrite(motor2_in_2, LOW);
	analogWrite(motor2_pwm, motor2_speed);
}


void motor_backward()
{
	digitalWrite(motor1_in_1, LOW) ;
	digitalWrite(motor1_in_2, HIGH) ;
	analogWrite(motor1_pwm, motor1_speed) ;

	digitalWrite(motor2_in_1, LOW) ;
	digitalWrite(motor2_in_2, HIGH) ;
	analogWrite(motor2_pwm, motor2_speed) ;
}


void motor_left()
{
	motor1_speed = 192;
	motor2_speed = 255;

	motor_forward();
	delay(1 * 1000);
	//  motor_stop();
	motor1_speed = 255;
	motor2_speed = 255;
}


void motor_right()
{
	motor1_speed = 192;
	motor2_speed = 128;

	motor_forward();
	delay(1 * 1000);
	//motor_stop();
	motor1_speed = 255;
	motor2_speed = 255;
}


void motor_demo()
{
	motor_forward();
	delay(3 * 1000);
	motor_left();

	motor_forward();
	delay(3 * 1000);
	motor_right();

	motor_forward();
	delay(3 * 1000);

	motor_backward();
	delay(3 * 1000);

	motor_stop();
}


void motor_test()
{
	for (int speed=128; speed<256; speed += 10)
	{
		digitalWrite(motor1_in_1, HIGH);
		digitalWrite(motor1_in_2, LOW);
		analogWrite(motor1_pwm, speed);

		digitalWrite(motor2_in_1, HIGH);
		digitalWrite(motor2_in_2, LOW);
		analogWrite(motor2_pwm, speed);

		delay(200);
	}

	motor_stop();
}


bool motor_cmd(int cmd, Stream& io)
{
	return false;
}
