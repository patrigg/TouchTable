#include "Image.h"

template<typename T>
Image<T>::Image(Data data, int width, int height)
: m_data(data), m_width(width), m_height(height)
{
	if (data.size() != width * height)
	{
		throw "incompatible data size";
	}
}

template<typename T>
Image<T>::Image(int width, int height)
: m_width(width), m_height(height)
{
	m_data.resize(width * height);
}

template<typename T>
Image<T>::Image()
	: m_width(0), m_height(0)
{
}

template<typename T>
Image<T>::~Image()
{
}

template<typename T>
int Image<T>::width() const
{
	return m_width;
}

template<typename T>
int Image<T>::height() const
{
	return m_height;
}

template<typename T>
const typename Image<T>::Data& Image<T>::data() const
{
	return m_data;
}

template<typename T>
typename Image<T>::Data& Image<T>::data()
{
	return m_data;
}

template<typename T>
void Image<T>::data(Data data)
{
	if (data.size() != m_data.size())
	{
		throw "incompatible data size!";
	}

	m_data = data;
}
/*
template<typename T>
Image<T>::RowIterator::RowIterator(Image<T>& image, size_t index)
: m_image(image), m_index(index)
{

}

template<typename T>
typename Image<T>::RowIterator& Image<T>::RowIterator::operator++()
{
	m_index += m_image.width;
	return *this;
}

template<typename T>
typename Image<T>::RowIterator Image<T>::RowIterator::operator++(int)
{
	auto tmp(*this);
	operator++();
	return tmp;
}

template<typename T>
typename Image<T>::RowIterator& Image<T>::RowIterator::operator--()
{
	m_index -= m_image.width;
	return *this;
}

template<typename T>
typename Image<T>::RowIterator Image<T>::RowIterator::operator--(int)
{
	auto tmp(*this);
	operator--();
	return tmp;
}

template<typename T>
bool Image<T>::RowIterator::operator==(const typename Image<T>::RowIterator& rhs) const
{
	return &m_image == &rhs.m_image && m_index == rhs.m_index;
}

template<typename T>
bool Image<T>::RowIterator::operator!=(const typename Image<T>::RowIterator& rhs) const
{
	return &m_image != &rhs.m_image || m_index != rhs.m_index;
}

template<typename T>
T& Image<T>::RowIterator::operator*()
{
	return m_image[m_index];
}
*/

template class Image<int>;
template class Image<uint8_t>;
template class Image<uint16_t>;
template class Image<Color>;
