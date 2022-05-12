#include "EncodingDecorator.h"

EncodingDecorator::EncodingDecorator(Bmp32 image) : m_image{ image } {}

void EncodingDecorator::Encode(std::string message)
{
	m_image.Encode(message);
}

std::string EncodingDecorator::Decode()
{
	return m_image.Decode();
}

void EncodingDecorator::Save(const char* path)
{
	m_image.Save(path);
}
