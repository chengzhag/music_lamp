#ifndef __MUSIC_LAMP
#define __MUSIC_LAMP

#include "ws2812.h"
#include "color_convert.h"

#define LED_COUNT 96
#define MUSIC_LAMP_BELT_INDEX 0
#define MUSIC_LAMP_BELT_COUNT 60
#define MUSIC_LAMP_INNERRING_INDEX \
MUSIC_LAMP_BELT_INDEX+MUSIC_LAMP_BELT_COUNT
#define MUSIC_LAMP_INNERRING_COUNT 12
#define MUSIC_LAMP_OUTERRING_INDEX \
MUSIC_LAMP_INNERRING_INDEX+MUSIC_LAMP_INNERRING_COUNT
#define MUSIC_LAMP_OUTERRING_COUNT 24

class LampModule
{
	uint8_t(*rgbData)[3];
public:
	LampModule(uint8_t p_data[][3]) :
		rgbData(p_data)
	{

	}

	//设置色彩数据，len*3数组
	void setData(uint8_t rgbColors[][3], uint16_t start, uint16_t len)
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

	//设置所有灯的颜色
	//uint8_t r uint8_t g uint8_t b
	void setAllDataRGB(uint8_t r, uint8_t g, uint8_t b)
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

	//设置所有灯的颜色
	//COLOR_RGB &rgb
	void setAllDataRGB(COLOR_RGB &rgb)
	{
		setAllDataRGB(rgb.r, rgb.g, rgb.b);
	}

	//设置所有灯的颜色
	//int h [0,360] float s [0,1] float v [0,1]
	void setAllDataHSV(int 	h, float s, float v)
	{
		COLOR_HSV hsv;
		hsv.h = h;
		hsv.s = s;
		hsv.v = v;
		setAllDataHSV(hsv);
	}

	//设置所有灯的颜色
	//COLOR_HSV &hsv
	void setAllDataHSV(COLOR_HSV &hsv)
	{
		COLOR_RGB rgb;
		hsv.h = hsv.h % 360;
		HSV_to_RGB(hsv, rgb);
		setAllDataRGB(rgb);
	}
};

class MusicLamp :WS2812
{
public:
	LampModule belt, innerRing, outerRing;

	MusicLamp(Gpio *p_pin):
		WS2812(p_pin),
		belt(rgbData+MUSIC_LAMP_BELT_INDEX),
		innerRing(rgbData+MUSIC_LAMP_INNERRING_INDEX),
		outerRing(rgbData+MUSIC_LAMP_OUTERRING_INDEX)
	{
		
	}

	//初始化dma、pwm
	void begin()
	{
		WS2812::begin();
	}

	//发送数据
	void sendData()
	{
		send_data();
	}

	//设置所有灯的颜色
	//uint8_t r uint8_t g uint8_t b
	void setAllDataRGB(uint8_t r, uint8_t g, uint8_t b)
	{
		belt.setAllDataRGB(r, g, b);
		innerRing.setAllDataRGB(r, g, b);
		outerRing.setAllDataRGB(r, g, b);
	}

	//设置所有灯的颜色
	//COLOR_RGB &rgb
	void setAllDataRGB(COLOR_RGB &rgb)
	{
		setAllDataRGB(rgb.r, rgb.g, rgb.b);
	}

	//设置所有灯的颜色
	//int h [0,360] float s [0,1] float v [0,1]
	void setAllDataHSV(int 	h, float s, float v)
	{
		COLOR_HSV hsv;
		hsv.h = h;
		hsv.s = s;
		hsv.v = v;
		setAllDataHSV(hsv);
	}

	//设置所有灯的颜色
	//COLOR_HSV &hsv
	void setAllDataHSV(COLOR_HSV &hsv)
	{
		COLOR_RGB rgb;
		hsv.h = hsv.h % 360;
		HSV_to_RGB(hsv, rgb);
		setAllDataRGB(rgb);
	}

	//彩虹循环
	void rainbowLoop(float brightness)
	{
		rainbow_Loop(brightness);
	}

};


#endif
