#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const byte rxAddr[6] = "00001"; // TX adress (Same as on RX)
int counter = 0; // Count TX (Only for demo example)
RF24 radio(8, 9); // Define Radio (CE-PIN,CSN-PIN)

void setup()
{
	Serial.begin(115200);

	radio.begin();
	radio.setPALevel(RF24_PA_MAX); // Transmit Power (MAX,HIGH,LOW,MIN)
	radio.setDataRate( RF24_250KBPS ); //Transmit Speeed (250 Kbits)
	radio.setRetries(15, 15);
	radio.openWritingPipe(rxAddr);
	// Disable Receiver
	radio.stopListening();

	Serial.println("--ready");
}


void loop()
{
	if (Serial.available())
	{
		char char_array[2];
		char_array[0] = '/';
		char_array[1] = Serial.read();
		if (char_array[1] >= '0' && char_array[1] <= 'z')
		{
			radio.write(&char_array, 2);
			Serial.print("SENDING...");
			Serial.println(char_array[1]);
		}
	}

#if 1
	counter++;
	String str = "XSend Counter: "; 
	str += String(counter);
	int str_len = str.length() + 1; 
	char char_array[str_len];
	str.toCharArray(char_array, str_len);
	radio.write(&char_array, sizeof(char_array));

	Serial.print("Actual Transmission: ");
	Serial.println( char_array );
	delay(1000);
#endif
}
