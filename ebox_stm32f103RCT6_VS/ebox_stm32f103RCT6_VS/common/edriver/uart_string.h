#ifndef __UART_STRING
#define __UART_STRING

#include "ebox.h"

#define UART_STRING_BUFFER_SIZE 100
class UartString
{
	Uart *uart;
	uint16_t bufferIndex;
	char buffer[UART_STRING_BUFFER_SIZE];

	//串口收到字节中断处理函数
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

#endif