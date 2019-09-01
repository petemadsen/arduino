#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


RF24 radio(28, 29); // Define radio (CE-PIN,CSN-PIN)
const byte rxAddr[6] = "00001"; // RX adress (Same as on TX)
String received_data;


void setup()
{
	Serial.begin(115200);
	Serial3.begin(115200);

	Serial.println("--init");
	VNH2SP30_begin();

	radio.begin();
	radio.setPALevel(RF24_PA_LOW); // Transmit Power (MAX,HIGH,LOW,MIN)
	radio.setDataRate(RF24_250KBPS); //Transmit Speeed (250 Kbits)
	radio.openReadingPipe(0, rxAddr);
	radio.startListening();

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

	if (radio.available())
	{
		char text[100] = {0};
		radio.read(&text, sizeof(text));
		received_data = String(text);

//		Serial.print("--radio--received--");
//		Serial.println(received_data);

		parse(text[0]);
		parse(text[1]);
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
