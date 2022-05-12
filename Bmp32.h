#pragma once

#include <vector>
#include <string>

#include "Headers.h"

class Bmp32
{
private:
    std::vector<uint8_t> m_data{};
    BMPFilheader m_fileHeader;
    BMPInfoHeader m_infoHeader;
    BMPColorHeader m_colorHeader;

    int IndexFromPos(int, int);
protected:
public:
    Bmp32(const char*);
    Bmp32(int, int, int, int, int);
    ~Bmp32();
    void Save(const char*);
    void DrawRect(int, int, int, int, int r = 255, int g = 255, int b = 255, int a = 255);
    void Negative();
    void Encode(std::string message);
    std::string Decode();
};

