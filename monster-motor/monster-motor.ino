#define BRAKEVCC 0
#define CW   1
#define CCW  2
#define BRAKEGND 3
#define CS_THRESHOLD 100
/*  VNH2SP30 pin definitions xxx[0] controls '1' outputs xxx[1] controls '2' outputs */
int inApin[2] = {7, 4};  // INA: Clockwise input
int inBpin[2] = {8, 9}; // INB: Counter‐clockwise input
int pwmpin[2] = {5, 6};
// PWM input
int cspin[2] = {2, 3}; // CS: Current sense ANALOG input
int enpin[2] = {0, 1}; // EN: Status of switches output (Analog pin)

int statpin = 13;void setup()
{  Serial.begin(9600);  pinMode(statpin, OUTPUT);  // Initialize digital pins as outputs
for (int i=0; i<2; i++)  {
  pinMode(inApin[i], OUTPUT);    pinMode(inBpin[i], OUTPUT);    pinMode(pwmpin[i], OUTPUT);  }  
  // Initialize braked  
  for (int i=0; i<2; i++)  {    digitalWrite(inApin[i], LOW);    digitalWrite(inBpin[i], LOW);  }  // motorGo(0, CW, 1023);  // motorGo(1, CCW, 1023);}
}



void motorOff(int motor);
void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm);


int speed = 50;
int direction = CW;
void loop()
{
  if (Serial.available())
  {
    uint8_t ch = Serial.read();

    bool do_stop = false;
    bool do_drive = false;

    switch (ch)
    {
      case 'w':
        do_stop = direction != CW;
        direction = CW;
        do_drive = true;
        break;
      case 's':
        do_stop = direction != CCW;
        direction = CCW;
        do_drive = true;
        break;
      case ' ':
        do_stop = true;
        do_drive = false;
        break;
      case '+':
        speed = min(speed + 15, 255);
        do_drive = true;
        break;
      case '-':
        speed = max(speed - 15, 10);
        do_drive = true;
        break;
    }

    if (do_stop)
    {
      Serial.println("--stop");

      while (speed > 0)
      {
        motorGo(0, direction, speed);
        motorGo(1, direction, speed);
        delay(300);
        speed -= 10;
      }
      speed = 50;
      
      motorOff(0);
      motorOff(1);
      delay(500);
      do_stop = false;
    }

    if (do_drive)
    {
      Serial.print("--drive ");
      Serial.print(direction);
      Serial.print(" ");
      Serial.println(speed);
      
        motorGo(0, direction, speed);
        motorGo(1, direction, speed);
    }
  }

#if 0
  for (int speed=600; speed<1024; speed += 200)
  {
    Serial.print("#1-");
    Serial.println(speed);
    
    motorGo(0, CW, speed);
    motorGo(1, CCW, speed);
    delay(2000);

    motorOff(0);
    motorOff(1);
    delay(1000);

    Serial.print("#2-");
    Serial.println(speed);
    motorGo(0, CCW, speed);  
    motorGo(1, CW, speed);
    delay(2000);

    motorOff(0);
    motorOff(1);
    delay(1000);
#endif

    if ((analogRead(cspin[0]) < CS_THRESHOLD) && (analogRead(cspin[1]) < CS_THRESHOLD))
    {
      //Serial.println("--wtf");
      digitalWrite(statpin, HIGH);
    }
}


void motorOff(int motor){  // Initialize braked 
    for (int i=0; i<2; i++)  {    digitalWrite(inApin[i], LOW);    digitalWrite(inBpin[i], LOW);  }  analogWrite(pwmpin[motor], 0);}


void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm)
{
  //Serial.print("--motor-go", 
  if (motor <= 1)  {
  if (direct <=4)    {      // Set inA[motor]      
    if (direct <=1)
    digitalWrite(inApin[motor], HIGH);
      else
    digitalWrite(inApin[motor], LOW);
          
    // Set inB[motor]      
    if ((direct==0)||(direct==2))        
      digitalWrite(inBpin[motor], HIGH);      
    else        
      digitalWrite(inBpin[motor], LOW);      
      analogWrite(pwmpin[motor], pwm);
    } 
  }
}
