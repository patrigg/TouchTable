#pragma once
#include <vector>
#include <cstdint>

struct Color
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

struct ColorAlpha
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

template <typename T>
class Image
{
public:
	typedef std::vector<T> Data;
	/*
	class Row
	{
	private:
		Image& m_index;
	};

	class RowIterator
	{
	public:
		RowIterator(Image& image, size_t index);
		RowIterator& operator++();
		RowIterator operator++(int);
		RowIterator& operator--();
		RowIterator operator--(int);
		bool operator==(const RowIterator& rhs) const;
		bool operator!=(const RowIterator& rhs) const;
		Row operator*();
	private:
		Image& m_image;
		size_t m_index;
	};
	*/
	Image(Data data, int width, int height);
	Image(int width, int height);
	~Image();

	int width() const;
	int height() const;
	const Data& data() const;
	Data& data();
	void data(Data data);
private:
	Data m_data;
	int m_width;
	int m_height;
};

typedef Image<uint16_t> DepthImage;
typedef Image<Color> RgbImage;
typedef Image<ColorAlpha> RgbaImage;

