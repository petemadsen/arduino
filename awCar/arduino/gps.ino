#include <stdlib.h>

#include <SoftwareSerial.h>

// Default baud of NEO-6M is 9600
int GPSBaud = 9600;


#define GPS_MAX_BUFLEN 160
char gps_buffer[GPS_MAX_BUFLEN + 1];
unsigned int gps_buffer_len = 0;



#define GPS_NMEA_POS1	"$GPRMC"
#define GPS_NMEA_POS2	"$GPGGA"


SoftwareSerial gpsSerial(pin_gps_rx, pin_gps_tx);

void gps_init()
{
	gpsSerial.begin(GPSBaud);
}

void gps_loop()
{
	while (gpsSerial.available() > 0)
	{
//		Serial.write(gpsSerial.read());
		gps_add(gpsSerial.read());
	}
}

void gps_add(char ch)
{
	if (ch == 10 || ch == 13)
	{
		if (gps_buffer_len)
			gps_parse();
		gps_buffer_len = 0;
		return;
	}

	if (gps_buffer_len < GPS_MAX_BUFLEN)
	{
		gps_buffer[gps_buffer_len++] = ch;
	}
}


// parse 10 msg
int gps_msg = 0;
void gps_parse()
{
//	if (gps_msg > 3)
//		return;

	if (gps_buffer[0] != '$')
		return;
	if (gps_buffer_len < strlen(GPS_NMEA_POS1))
		return;
	if (gps_buffer_len < strlen(GPS_NMEA_POS2))
		return;

	if (strncmp(gps_buffer, GPS_NMEA_POS1, strlen(GPS_NMEA_POS1)) == 0)
	{
//		Serial.println("--new-pos1");
//		Serial.println(gps_buffer);
//		++gps_msg;
	}
	else if (strncmp(gps_buffer, GPS_NMEA_POS2, strlen(GPS_NMEA_POS2)) == 0)
	{
		Serial.println("--new-pos2");
		Serial.println(gps_buffer);

		const char s[2] = ",";
		int pos = 0;
		char* token;
		char* buffer = gps_buffer;
		while ( (token = strsep(&buffer, s)) != NULL)
		{
			if (pos == 2 || pos == 4 || pos == 7 || pos == 9)
			{
				Serial.print("--fff [");
				Serial.print(pos);
				Serial.print("]");
				Serial.print(strlen(token));
				Serial.print(" ");
				Serial.println(token);
//				float f = atof(token, NULL);
//				Serial.println(f);
			}

			pos++;
		}

#if 1
		
#else
		char* token = strtok(gps_buffer, s);
		int pos = 1;
		while (token != NULL)
		{
			if (pos == 3 || pos == 5)
			{
				Serial.print("--fff [");
				Serial.print(pos);
				Serial.print("] ");
				Serial.println(token);
//				float f = atof(token, NULL);
//				Serial.println(f);
			}

			token = strtok(NULL, s);
			pos++;
		}
#endif
		++gps_msg;
	}
//	Serial.println(gps_buffer_len);
}

