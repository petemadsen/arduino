#ifndef GB_EMU
#define GB_EMU

class GbEmuButtons
{
public:
	boolean pressed(uint8_t button) { return false; }
};


class GbEmuDisplay : public Print
{
	virtual size_t write(uint8_t) { return 0; }
};


class GbEmu
{
public:
	GbEmu() {} 

	void begin() {}

	void titleScreen(const __FlashStringHelper* name) {}

	boolean update() {}

	GbEmuButtons buttons;
	GbEmuDisplay display;

	uint32_t frameCount;
};

#endif
