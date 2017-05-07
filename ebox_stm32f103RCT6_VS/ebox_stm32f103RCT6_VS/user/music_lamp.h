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


//台灯led参数
#define LED_COUNT 96
#define MUSIC_LAMP_BELT_INDEX 0
#define MUSIC_LAMP_BELT_COUNT 60
#define MUSIC_LAMP_INNERRING_INDEX \
(MUSIC_LAMP_BELT_INDEX+MUSIC_LAMP_BELT_COUNT)
#define MUSIC_LAMP_INNERRING_COUNT 12
#define MUSIC_LAMP_OUTERRING_INDEX \
(MUSIC_LAMP_INNERRING_INDEX+MUSIC_LAMP_INNERRING_COUNT)
#define MUSIC_LAMP_OUTERRING_COUNT 24

//台灯音乐模式参数
#define MUSIC_LAMP_SIGNAL_STREAM_BUFFER_SIZE 70
#define MUSIC_LAMP_SIGNAL_STREAM_FILTER_WINDOW_SIZE 10
#define MUSIC_LAMP_SIGNAL_STREAM_MIN_ENHANCE_FACTOR 0.1

class LampModule
{

	uint8_t(*rgbData)[3];
public:
	LampModule(uint8_t p_data[][3]);

	//设置色彩数据，len*3数组
	void setData(uint8_t rgbColors[][3], uint16_t start, uint16_t len);

	//设置所有灯的颜色
	//uint8_t r uint8_t g uint8_t b
	void setAllDataRGB(uint8_t r, uint8_t g, uint8_t b);

	//设置所有灯的颜色
	//COLOR_RGB &rgb
	void setAllDataRGB(COLOR_RGB &rgb);

	//设置所有灯的颜色
	//int h [0,360] float s [0,1] float v [0,1]
	void setAllDataHSV(int 	h, float s, float v);

	//设置所有灯的颜色
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
	//处理声音包络，滤波和增强
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

	

	//串口通信
	UartString uart;
	//串口字符串处理函数
	void stringReceivedEvent(char* str);

	//模式
	int mode;
	//全局参数 (0~1)
	uint8_t power;
	float brightness;
	//照明模式参数
	uint16_t lightModeTemp;
	//彩色模式参数
	COLOR_HSV colorModeHSV;
	//颜色变换模式参数、函数
	float rippleModeCurrentH;
	float rippleModeIncrease;
	void rippleModeRefresh(float brightness);
	//音乐模式参数、函数
	Gpio *analogPin;
	
	void musicModeRefresh();

public:
	SoundEnhance<float, MUSIC_LAMP_SIGNAL_STREAM_BUFFER_SIZE> volumes;//历史音量处理 (0~1)
	LampModule belt, innerRing, outerRing;

	//音乐台灯，p_pin为灯带数据接口，a_pin为麦克风检波电路模拟接口，uartX用于指令接收
	MusicLamp(Gpio *p_pin, Gpio *a_pin, Uart *uartX);

	//通过串口发送调试数据
	void printf(const char *fmt, ...);

	//初始化dma、pwm
	void begin();

	//发送数据
	void sendData();

	//刷新台灯状态
	void refresh();

	//设置所有灯的颜色
	//uint8_t r uint8_t g uint8_t b
	void setAllDataRGB(uint8_t r, uint8_t g, uint8_t b);

	//设置所有灯的颜色
	//COLOR_RGB &rgb
	void setAllDataRGB(COLOR_RGB &rgb);

	//设置所有灯的颜色
	//int h [0,360] float s [0,1] float v [0,1]
	void setAllDataHSV(int h, float s, float v);

	//设置所有灯的颜色
	//COLOR_HSV &hsv
	void setAllDataHSV(COLOR_HSV &hsv);



	///参数设置
	void setPower(int power);
	//设置台灯模式
	void setMode(Music_Lamp_Mode mode);

	//设置亮度
	void setBrightness(float brightness);

	//设置色温
	void setLightModeTemp(uint16_t temperature);

	//设置颜色模式hsv
	void setColorModeHSV(int h);
	void setColorModeHSV(int h, float s);
	void setColorModeHSV(int h, float s, float v);

	//设置颜色变换模式速度
	void setRippleModeIncrease(float increase);

};


#endif
