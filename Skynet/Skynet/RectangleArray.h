#pragma once

#include <vector>

template <typename T>
class RectangleArray
{
public:
	RectangleArray(unsigned int width, unsigned int height, T val = T())
		: mData(width, std::vector<T>(height, val))
	{}

	RectangleArray()
	{}

	void resize(unsigned int width, unsigned int height, T val = T())
	{
		mData.resize(width);
		for(unsigned int i = 0; i < width; ++i)
			mData[i].resize(height, val);
	}

	const std::vector<T> &operator[](int i) const
	{
		return mData[i];
	}

	std::vector<T> &operator[](int i)
	{
		return mData[i];
	}

private:
	std::vector<std::vector<T>> mData;
};