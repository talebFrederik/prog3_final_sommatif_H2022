#include <fstream>
#include "ByteDecorator.h"

char ByteDecorator::lireCle(const char* path)
{
    char cle{ 0 };
    std::ifstream keyFile{ path, std::ios::in | std::ios::binary };
    if (keyFile.is_open())
    {
        keyFile.read(&cle, 1);

        keyFile.close();
    }

    return cle;
}

ByteDecorator::ByteDecorator(Bmp32 image): EncodingDecorator(image){}

void ByteDecorator::Encode(std::string message)
{
    char key{ lireCle("cle.steg") };

    for (int i = 0; i < message.size(); i++)
    {
        message[i] = message[i] ^ key;
    }

    m_image.Encode(message);
}

std::string ByteDecorator::Decode()
{
    char key{ lireCle("cle.steg") };
    std::string message = m_image.Decode();

    for (int i = 0; i < message.size(); i++)
    {
        message[i] = message[i] ^ key;
    }

    return message;
}
