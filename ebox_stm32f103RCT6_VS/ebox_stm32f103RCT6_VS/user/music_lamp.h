#ifndef __MUSIC_LAMP
#define __MUSIC_LAMP

#include "ebox.h"
#include "ws2812.h"
#include "color_convert.h"
#include "temp2rgb.h"
#include "uart_string.h"
#include "signal_stream.h"
#include "cJSON.h"
#include <string.h>


//̨��led����
#define LED_COUNT 96
#define MUSIC_LAMP_BELT_INDEX 0
#define MUSIC_LAMP_BELT_COUNT 60
#define MUSIC_LAMP_INNERRING_INDEX \
(MUSIC_LAMP_BELT_INDEX+MUSIC_LAMP_BELT_COUNT)
#define MUSIC_LAMP_INNERRING_COUNT 12
#define MUSIC_LAMP_OUTERRING_INDEX \
(MUSIC_LAMP_INNERRING_INDEX+MUSIC_LAMP_INNERRING_COUNT)
#define MUSIC_LAMP_OUTERRING_COUNT 24

//̨������ģʽ����
#define MUSIC_LAMP_SIGNAL_STREAM_BUFFER_SIZE 70
#define MUSIC_LAMP_SIGNAL_STREAM_FILTER_WINDOW_SIZE 10
#define MUSIC_LAMP_SIGNAL_STREAM_MIN_ENHANCE_FACTOR 0.1

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
	//�����������磬�˲�����ǿ
	template<typename T, int Size>
	class SoundEnhance :public SignalStream<T, Size>
	{
		T min, max;
		void refreshMinMax();
	public:
		SoundEnhance();
		void push(T signal);
		T pushAndGetEnhancedSignal(T signal);
	};

	

	//����ͨ��
	UartString uart;
	//�����ַ���������
	void stringReceivedEvent(char* str);

	//ģʽ
	int mode;
	//ȫ�ֲ��� (0~1)
	uint8_t power;
	float brightness;
	//����ģʽ����
	uint16_t lightModeTemp;
	//��ɫģʽ����
	COLOR_HSV colorModeHSV;
	//��ɫ�任ģʽ����������
	float rippleModeCurrentH;
	float rippleModeIncrease;
	void rippleModeRefresh(float brightness);
	//����ģʽ����������
	Gpio *analogPin;
	
	void musicModeRefresh();

public:
	SoundEnhance<float, MUSIC_LAMP_SIGNAL_STREAM_BUFFER_SIZE> volumes;//��ʷ�������� (0~1)
	LampModule belt, innerRing, outerRing;

	//����̨�ƣ�p_pinΪ�ƴ����ݽӿڣ�a_pinΪ��˷�첨��·ģ��ӿڣ�uartX����ָ�����
	MusicLamp(Gpio *p_pin, Gpio *a_pin, Uart *uartX);

	//ͨ�����ڷ��͵�������
	void printf(const char *fmt, ...);

	//��ʼ��dma��pwm
	void begin();

	//��������
	void sendData();

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
	void setPower(int power);
	//����̨��ģʽ
	void setMode(Music_Lamp_Mode mode);

	//��������
	void setBrightness(float brightness);

	//����ɫ��
	void setLightModeTemp(uint16_t temperature);

	//������ɫģʽhsv
	void setColorModeHSV(int h);
	void setColorModeHSV(int h, float s);
	void setColorModeHSV(int h, float s, float v);

	//������ɫ�任ģʽ�ٶ�
	void setRippleModeIncrease(float increase);

};


#endif
