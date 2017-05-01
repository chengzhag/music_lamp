#ifndef __MUSIC_LAMP
#define __MUSIC_LAMP

#include "ws2812.h"
#include "color_convert.h"
#include "temp2rgb.h"

#define LED_COUNT 96
#define MUSIC_LAMP_BELT_INDEX 0
#define MUSIC_LAMP_BELT_COUNT 60
#define MUSIC_LAMP_INNERRING_INDEX \
(MUSIC_LAMP_BELT_INDEX+MUSIC_LAMP_BELT_COUNT)
#define MUSIC_LAMP_INNERRING_COUNT 12
#define MUSIC_LAMP_OUTERRING_INDEX \
(MUSIC_LAMP_INNERRING_INDEX+MUSIC_LAMP_INNERRING_COUNT)
#define MUSIC_LAMP_OUTERRING_COUNT 24

class LampModule
{
	uint8_t(*rgbData)[3];
public:
	LampModule(uint8_t p_data[][3]);

	//����ɫ�����ݣ�len*3����
	void setData(uint8_t rgbColors[][3], uint16_t start, uint16_t len);

	//�������еƵ���ɫ
	//uint8_t r uint8_t g uint8_t b
	void setAllDataRGB(uint8_t r, uint8_t g, uint8_t b);

	//�������еƵ���ɫ
	//COLOR_RGB &rgb
	void setAllDataRGB(COLOR_RGB &rgb);

	//�������еƵ���ɫ
	//int h [0,360] float s [0,1] float v [0,1]
	void setAllDataHSV(int 	h, float s, float v);

	//�������еƵ���ɫ
	//COLOR_HSV &hsv
	void setAllDataHSV(COLOR_HSV &hsv);
};

typedef enum
{
	Music_Lamp_Mode_Light,
	Music_Lamp_Mode_Color,
	Music_Lamp_Mode_Ripple,
	Music_Lamp_Mode_Music
}Music_Lamp_Mode;

class MusicLamp :WS2812
{
	int mode;
	//ȫ�ֲ���
	float brightness;
	//����ģʽ����
	uint16_t lightModeTemp;
	//��ɫģʽ����
	COLOR_HSV colorModeHSV;
	//��ɫ�任ģʽ����
	float rippleModeCurrentH;
	float rippleModeIncrease;
public:

	LampModule belt, innerRing, outerRing;

	MusicLamp(Gpio *p_pin);

	//��ʼ��dma��pwm
	void begin();

	//��������
	void sendData();

	//����̨��ģʽ
	void setMode(Music_Lamp_Mode mode);

	//ˢ��̨��״̬
	void refresh();

	//�������еƵ���ɫ
	//uint8_t r uint8_t g uint8_t b
	void setAllDataRGB(uint8_t r, uint8_t g, uint8_t b);

	//�������еƵ���ɫ
	//COLOR_RGB &rgb
	void setAllDataRGB(COLOR_RGB &rgb);

	//�������еƵ���ɫ
	//int h [0,360] float s [0,1] float v [0,1]
	void setAllDataHSV(int h, float s, float v);

	//�������еƵ���ɫ
	//COLOR_HSV &hsv
	void setAllDataHSV(COLOR_HSV &hsv);

	///��������

	//��������
	void setBrightness(float brightness);

	//����ɫ��
	void setLightModeTemp(uint16_t temperature);

	//������ɫģʽhsv
	void setColorModeHSV(int h, float s);
	void setColorModeHSV(int h, float s, float v);

	//������ɫ�任ģʽ�ٶ�
	void setRippleModeIncrease(float increase);

	//�ʺ�ѭ��
	void rainbowLoop(float brightness);

};


#endif
