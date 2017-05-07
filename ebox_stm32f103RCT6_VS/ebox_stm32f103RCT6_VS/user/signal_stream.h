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
		buf[indexHead] = signal;
	}
	T &operator [](int index)
	{
		return buf[(index + indexHead) % Size];
	}
};

template<typename T, int Size>
class SignalEnhance :SignalStream<T, Size>
{
	T min, max;
	void refreshMinMax()
	{
		min = SignalStream<T, Size>::operator [](0);
		max = SignalStream<T, Size>::operator [](0);
		for (int i = 1; i < Size; i++)
		{
			if (SignalStream<T, Size>::operator [](i) < min)
			{
				min = SignalStream<T, Size>::operator [](i);
			}
			if (SignalStream<T, Size>::operator [](i) > max)
			{
				max = SignalStream<T, Size>::operator [](i);
			}
		}
	}
public:
	SignalEnhance() :
		SignalStream<T, Size>(),
		min(0), max(0)
	{

	}
	void push(T signal)
	{
		SignalStream<T, Size>::push(signal);
	}
	T pushAndGetEnhancedSignal(T signal)
	{
		push(signal);
		refreshMinMax();
		return (signal - (float)min) / (max - min);
	}

};