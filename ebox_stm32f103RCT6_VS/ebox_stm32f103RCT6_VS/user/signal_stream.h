#pragma once
template<typename T, int Size>
class SignalStream
{
	int indexHead;
public:
	T buf[Size];
	SignalStream() :
		indexHead(0)
	{
		clear();
	}
	void clear()
	{
		for (int i = 0; i < Size; i++)
		{
			buf[i] = 0;
		}
	}
	int size()
	{
		return Size;
	}
	void push(T signal)
	{
		indexHead--;
		indexHead %= Size;
		if (indexHead < 0)
		{
			indexHead += Size;
		}
		buf[indexHead] = signal;
	}
	T &operator [](int index)
	{
		return buf[(index + indexHead) % Size];
	}
};

