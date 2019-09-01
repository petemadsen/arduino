#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


RF24 radio(8, 9); // Define radio (CE-PIN,CSN-PIN)
const byte rxAddr[6] = "00001"; // RX adress (Same as on TX)
String received_data;


void setup()
{
	// Start the serial
	Serial.begin(115200);
	Serial.println("NRF24 Receiver");
	radio.begin();
	radio.setPALevel(RF24_PA_LOW); // Transmit Power (MAX,HIGH,LOW,MIN)
	radio.setDataRate( RF24_250KBPS ); //Transmit Speeed (250 Kbits)
	radio.openReadingPipe(0, rxAddr);
	radio.startListening();
}


int step = 1;
void loop()
{
	if (radio.available())
	{
		int avail = radio.available();

		char text[100] = {0}; //Buffer
		radio.read(&text, sizeof(text));

		Serial.print("#");
		Serial.print(step);
		Serial.print(" [");
		Serial.print(avail);
		Serial.print("] ");
		Serial.println((int)text[0]);
#if 1
		received_data = String(text);
		Serial.println(received_data);
#endif

		step += 1;
		delay(1000);
	} 
}
