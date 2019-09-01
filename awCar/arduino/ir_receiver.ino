#include <IRremote.h>

IRrecv ir_rcv(pin_ir_rcv);
decode_results ir_rcv_res;


const int ir_rcv_keys = 21;
const unsigned int ir_rcv_raw[] /*PROGMEM*/ = {
	0xa25d, 0x629d, 0xe21d,
	0x22dd, 0x02fd, 0xc23d,
	0xe01f, 0xa857, 0x906f,
	0x6897, 0x9867, 0xb04f,
	0x30cf, 0x18e7, 0x7a85,
	0x10ef, 0x38c7, 0x5aa5,
	0x42bd, 0x4ab5, 0x52ad,
};
const char ir_rcv_human[] /*PROGMEM*/ = {
	'(', '?', ')', // ch-, ch, ch+
	'L', 'R', 'P', // prev, next, play/pause
	'-', '+', '=', // vol-, vol+
	'0', ' ', ' ', // 0, 100+, 200+
	'1', '2', '3',
	'4', '5', '6',
	'7', '8', '9',
};


void ir_rcv_init()
{
	ir_rcv.enableIRIn();
}


void ir_rcv_loop()
{
	if (ir_rcv.decode(&ir_rcv_res))
	{
		unsigned int raw = ir_rcv_res.value & 0xffff;
//		Serial.println(raw, HEX);

		for (int i=0; i<ir_rcv_keys; ++i)
		{
			if (raw == ir_rcv_raw[i])
			{
				Serial.print(F("[irr] "));
				Serial.print(ir_rcv_human[i]);
				Serial.println();
				break;
			}
		}
		ir_rcv.resume();
	}
}
