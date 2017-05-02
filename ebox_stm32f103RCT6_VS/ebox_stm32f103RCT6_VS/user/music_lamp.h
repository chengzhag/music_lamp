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

#define UART_STRING_BUFFER_SIZE 100
class UartString
{
	Uart *uart;
	uint16_t bufferIndex;
	char buffer[UART_STRING_BUFFER_SIZE];

	//�����յ��ֽ��жϴ�����
	void rxEvent()
	{
		if (bufferIndex < UART_STRING_BUFFER_SIZE)
		{
			uint8_t c;
			c = uart->read();
			buffer[bufferIndex] = c;
			bufferIndex++;
			if (c == '\n')
			{
				buffer[bufferIndex] = '\0';
				stringEvent(buffer);
				bufferIndex = 0;
			}
		}
	}

	void(*stringEvent)(char *str);
public:

	UartString(Uart *uartX) :
		bufferIndex(0),
		uart(uartX)
	{
	}
	void begin(uint32_t baud_rate, uint8_t _use_dma = 1)
	{
		uart->begin(baud_rate, _use_dma);
		uart->attach(this, &UartString::rxEvent, RxIrq);
	}
	void begin(uint32_t baud_rate, uint8_t data_bit, uint8_t parity, float stop_bit, uint8_t _use_dma)
	{
		uart->begin(baud_rate, data_bit, parity, stop_bit, _use_dma);
		uart->attach(this, &UartString::rxEvent, RxIrq);
	}
	void printf(const char *fmt, ...)
	{
		uart->printf(fmt);
	}
	void attach(void(*stringEvent)(char *str))
	{
		this->stringEvent = stringEvent;
	}
	template<typename T>
	void attatch(T *pObj, void (T::*classStringEvent)(char *str))
	{
		this->stringEvent = pObj->classStringEvent;
	}
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
