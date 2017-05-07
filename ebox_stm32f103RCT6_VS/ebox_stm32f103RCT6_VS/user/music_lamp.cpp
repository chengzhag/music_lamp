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

template<typename T, int Size>
void MusicLamp::SoundEnhance<T, Size>::refreshMinMax()
{
	min = SignalStream<T, Size>::buf[0];
	max = SignalStream<T, Size>::buf[0];
	for (int i = 1; i < Size; i++)
	{
		if (SignalStream<T, Size>::buf[i] < min)
		{
			min = SignalStream<T, Size>::buf[i];
		}
		if (SignalStream<T, Size>::buf[i] > max)
		{
			max = SignalStream<T, Size>::buf[i];
		}
	}
}

template<typename T, int Size>
MusicLamp::SoundEnhance<T, Size>::SoundEnhance() :
	SignalStream<T, Size>(),
	min(0), max(0)
{

}

template<typename T, int Size>
void MusicLamp::SoundEnhance<T, Size>::push(T signal)
{
	SignalStream<T, Size>::push(signal);
}

template<typename T, int Size>
T MusicLamp::SoundEnhance<T, Size>::pushAndGetEnhancedSignal(T signal)
{
	push(signal);
	refreshMinMax();
	float factor = max - min;
	//在环境音较低时，防止继续增加亮度
	if (factor < MUSIC_LAMP_SIGNAL_STREAM_MIN_ENHANCE_FACTOR)
	{
		factor = MUSIC_LAMP_SIGNAL_STREAM_MIN_ENHANCE_FACTOR;
	}
	//对最近的声音信号进行加权平均，作为滤波
	for (int i = 1; i < MUSIC_LAMP_SIGNAL_STREAM_FILTER_WINDOW_SIZE; i++)
	{
		signal += SignalStream<T, Size>::operator [](i) * 2 * (1 - i / MUSIC_LAMP_SIGNAL_STREAM_FILTER_WINDOW_SIZE);
	}
	signal /= MUSIC_LAMP_SIGNAL_STREAM_FILTER_WINDOW_SIZE;
	float enhancedSignal = (signal - min) / factor;
	return;
}

void MusicLamp::stringReceivedEvent(char* str)
{
	cJSON *json,*jsonMethod,*jsonParams;
	json = cJSON_Parse(str);
	if (!json)
	{
		printf("Error before: [%s]\n", cJSON_GetErrorPtr());
	}
	else
	{
		//解析函数名
		jsonMethod = cJSON_GetObjectItem(json, "method");
		char *method;
		if (jsonMethod->type == cJSON_String)
		{
			// 从valueint中获得结果  
			method = jsonMethod->valuestring;
		}
		//解析参数列表
		jsonParams = cJSON_GetObjectItem(json, "params");
		//根据函数名调用对应函数

		//setMode
		if (strcmp(method,"setMode")==0)
		{
			if (jsonParams->type == cJSON_Number)
			{
				setMode((Music_Lamp_Mode)(jsonParams->valueint));
			}
		}
		else if (strcmp(method, "setBrightness") == 0)
		{
			if (jsonParams->type == cJSON_Number)
			{
				setBrightness((float)(jsonParams->valuedouble));
			}
		}
		else if (strcmp(method, "setLightModeTemp") == 0)
		{
			if (jsonParams->type == cJSON_Number)
			{
				setLightModeTemp((float)(jsonParams->valueint));
			}
		}
		else if (strcmp(method, "setColorModeHSV") == 0)
		{
			if (jsonParams->type == cJSON_Number)
			{
				setColorModeHSV((jsonParams->valueint));
			}
			else if (jsonParams->type == cJSON_Array)
			{
				int size = cJSON_GetArraySize(jsonParams);
				cJSON *item;
				if (size==1)
				{
					setColorModeHSV(
						cJSON_GetArrayItem(jsonParams, 0)->valueint
					);
				}
				if (size==2)
				{
					setColorModeHSV(
						cJSON_GetArrayItem(jsonParams, 0)->valueint,
						(float)(cJSON_GetArrayItem(jsonParams, 1)->valuedouble)
					);
				}
				else if (size == 3)
				{
					setColorModeHSV(
						cJSON_GetArrayItem(jsonParams, 0)->valueint,
						(float)(cJSON_GetArrayItem(jsonParams, 1)->valuedouble),
						(float)(cJSON_GetArrayItem(jsonParams, 2)->valuedouble)
					);
				}
			}
		}
		else if (strcmp(method, "setRippleModeIncrease") == 0)
		{
			if (jsonParams->type == cJSON_Number)
			{
				setRippleModeIncrease((float)(jsonParams->valuedouble));
			}
		}
		else if (strcmp(method, "setPower") == 0)
		{
			if (jsonParams->type == cJSON_Number)
			{
				setPower(jsonParams->valueint);
			}
		}
	}
}

void MusicLamp::rippleModeRefresh(float brightness)
{
	setAllDataHSV(rippleModeCurrentH, 1, brightness);
	rippleModeCurrentH += rippleModeIncrease;
	if (rippleModeCurrentH > 360)
	{
		rippleModeCurrentH = 0;
	}
}

void MusicLamp::musicModeRefresh()
{
	uint16_t ain = analog_read(analogPin);
	float factor = volumes.pushAndGetEnhancedSignal(ain / 4096.0);
	rippleModeRefresh(brightness*factor);
}

MusicLamp::MusicLamp(Gpio *p_pin, Gpio *a_pin, Uart *uartX) :
	WS2812(p_pin),
	belt(rgbData + MUSIC_LAMP_BELT_INDEX),
	innerRing(rgbData + MUSIC_LAMP_INNERRING_INDEX),
	outerRing(rgbData + MUSIC_LAMP_OUTERRING_INDEX),
	mode(Music_Lamp_Mode_Light),
	brightness(0.2),
	lightModeTemp(6000),
	rippleModeCurrentH(0),
	rippleModeIncrease(0.2),
	uart(uartX),
	analogPin(a_pin),
	power(0)
{
	colorModeHSV.h = 0;
	colorModeHSV.s = 1;
	colorModeHSV.v = 0.2;
}

void MusicLamp::printf(const char *fmt, ...)
{
	uart.printf(fmt);
}

void MusicLamp::begin()
{
	WS2812::begin();
	uart.begin(9600);
	uart.attach(this, &MusicLamp::stringReceivedEvent);
	analogPin->mode(AIN);
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
	if (power)
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
			rippleModeRefresh(brightness);
			break;
		case Music_Lamp_Mode_Music:
			musicModeRefresh();
			break;
		default:
			break;
		}
	}
	else
	{
		setAllDataRGB(0, 0, 0);
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

void MusicLamp::setPower(int power)
{
	this->power = power;
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

void MusicLamp::setColorModeHSV(int h)
{
	colorModeHSV.h = h;
}

void MusicLamp::setRippleModeIncrease(float increase)
{
	rippleModeIncrease = increase;
}

