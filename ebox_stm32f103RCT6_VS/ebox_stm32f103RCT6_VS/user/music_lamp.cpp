#include "music_lamp.h"


LampModule::LampModule(uint8_t p_data[][3]) :
	rgbData(p_data)
{

}

void LampModule::setData(uint8_t rgbColors[][3], uint16_t start, uint16_t len)
{
	uint8_t i, j = 0;

	while (len)
	{
		len--;
		for (i = 0; i < 3; i++)  						// Set RGB LED color R -> i=0, G -> i=1, B -> i=2
		{
			rgbData[len + start][i] = rgbColors[len][i];
		}
	}
}

void LampModule::setAllDataRGB(COLOR_RGB &rgb)
{
	setAllDataRGB(rgb.r, rgb.g, rgb.b);
}

void LampModule::setAllDataRGB(uint8_t r, uint8_t g, uint8_t b)
{
	uint8_t i = 0;
	uint16_t len = LED_COUNT;

	while (len)
	{
		len--;
		rgbData[len][0] = g;
		rgbData[len][1] = r;
		rgbData[len][2] = b;
	}
}

void LampModule::setAllDataHSV(COLOR_HSV &hsv)
{
	COLOR_RGB rgb;
	hsv.h = hsv.h % 360;
	HSV_to_RGB(hsv, rgb);
	setAllDataRGB(rgb);
}

void LampModule::setAllDataHSV(int h, float s, float v)
{
	COLOR_HSV hsv;
	hsv.h = h;
	hsv.s = s;
	hsv.v = v;
	setAllDataHSV(hsv);
}

MusicLamp::MusicLamp(Gpio *p_pin) :
	WS2812(p_pin),
	belt(rgbData + MUSIC_LAMP_BELT_INDEX),
	innerRing(rgbData + MUSIC_LAMP_INNERRING_INDEX),
	outerRing(rgbData + MUSIC_LAMP_OUTERRING_INDEX),
	mode(Music_Lamp_Mode_Light),
	brightness(0.2),
	lightModeTemp(6000),
	rippleModeCurrentH(0),
	rippleModeIncrease(0.2)
{
	colorModeHSV.h = 0;
	colorModeHSV.s = 0;
	colorModeHSV.v = 1;
}

void MusicLamp::begin()
{
	WS2812::begin();
}

void MusicLamp::sendData()
{
	send_data();
}

void MusicLamp::setMode(Music_Lamp_Mode mode)
{
	this->mode = mode;
}

void MusicLamp::refresh()
{
	switch (mode)
	{
	case Music_Lamp_Mode_Light:
		COLOR_RGB rgb = temp2rgb(lightModeTemp);
		setAllDataRGB(rgb.r*brightness, rgb.g*brightness, rgb.b*brightness);
		break;
	case Music_Lamp_Mode_Color:
		setAllDataHSV(colorModeHSV);
		break;
	case Music_Lamp_Mode_Ripple:
		setAllDataHSV(rippleModeCurrentH, 1, brightness);
		rippleModeCurrentH += rippleModeIncrease;
		if (rippleModeCurrentH>360)
		{
			rippleModeCurrentH = 0;
		}
		break;
	case Music_Lamp_Mode_Music:

		break;
	default:
		break;
	}
	sendData();
}

void MusicLamp::setAllDataRGB(COLOR_RGB &rgb)
{
	setAllDataRGB(rgb.r, rgb.g, rgb.b);
}

void MusicLamp::setAllDataRGB(uint8_t r, uint8_t g, uint8_t b)
{
	belt.setAllDataRGB(r, g, b);
	innerRing.setAllDataRGB(r, g, b);
	outerRing.setAllDataRGB(r, g, b);
}

void MusicLamp::setAllDataHSV(COLOR_HSV &hsv)
{
	COLOR_RGB rgb;
	hsv.h = hsv.h % 360;
	HSV_to_RGB(hsv, rgb);
	setAllDataRGB(rgb);
}

void MusicLamp::setAllDataHSV(int h, float s, float v)
{
	COLOR_HSV hsv;
	hsv.h = h;
	hsv.s = s;
	hsv.v = v;
	setAllDataHSV(hsv);
}

void MusicLamp::setBrightness(float brightness)
{
	this->brightness = brightness;
	colorModeHSV.v = brightness;
}

void MusicLamp::setLightModeTemp(uint16_t temperature)
{
	this->lightModeTemp = temperature;
}

void MusicLamp::setColorModeHSV(int h, float s)
{
	colorModeHSV.h = h;
	colorModeHSV.s = s;
	colorModeHSV.v = brightness;
}

void MusicLamp::setColorModeHSV(int h, float s, float v)
{
	colorModeHSV.h = h;
	colorModeHSV.s = s;
	colorModeHSV.v = v;
	setBrightness(v);
}

void MusicLamp::setRippleModeIncrease(float increase)
{
	rippleModeIncrease = increase;
}

void MusicLamp::rainbowLoop(float brightness)
{
	rainbow_Loop(brightness);
}
