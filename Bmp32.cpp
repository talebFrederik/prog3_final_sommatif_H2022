#include <iostream>
#include <fstream>
#include <vector>

#include "Bmp32.h"

int Bmp32::IndexFromPos(int x, int y)
{
    return (x + m_infoHeader.width * y) * 4;
}


Bmp32::Bmp32(const char* filePath)
{
    std::ifstream image{ filePath, std::ios::in | std::ios::binary };
    if (image.is_open())
    {
        image.read((char*)&m_fileHeader, sizeof(m_fileHeader));
        image.read((char*)&m_infoHeader, sizeof(m_infoHeader));
        image.read((char*)&m_colorHeader, sizeof(m_colorHeader));

        m_data.resize(m_infoHeader.size_image);

        image.seekg(m_fileHeader.offset, image.beg);
        image.read((char*)m_data.data(), m_data.size());

        image.close();
    }
}

Bmp32::Bmp32(int width, int heigth, int r=255, int g=255, int b=255)
{
    // Le décalage en octets pour obtenir les données des pixels
    // est équivalent à la taille des 3 en-têtes
    m_fileHeader.offset = sizeof(m_fileHeader) + sizeof(m_infoHeader) + sizeof(m_colorHeader);

    // La taille totale est calculée à partir de la taille des en-têtes et additionnée
    // à la tailles des pixels constituant l'image où chaque pixel a 4 composantes (BGRA) de 1 octet
    m_fileHeader.file_size = m_fileHeader.offset + width * heigth * 4;

    m_infoHeader.size = sizeof(m_infoHeader) + sizeof(m_colorHeader);
    m_infoHeader.width = width;
    m_infoHeader.height = heigth;
    m_infoHeader.size_image = width * heigth * 4;
    m_infoHeader.bit_count = 32;
    
    m_data.resize(m_infoHeader.size_image);
    for (int i = 0; i < m_data.size(); i+=4)
    {
        m_data[i] = b;
        m_data[i + 1] = g;
        m_data[i + 2] = r;
        m_data[i + 3] = 255;
    }
}

Bmp32::~Bmp32()
{
}

void Bmp32::Save(const char* filePath)
{
    std::ofstream outImage{ filePath, std::ios_base::binary };
    if (outImage)
    {
        outImage.write((char*)&m_fileHeader, sizeof(m_fileHeader));
        outImage.write((char*)&m_infoHeader, sizeof(m_infoHeader));
        outImage.write((char*)&m_colorHeader, sizeof(m_colorHeader));
        outImage.write((char*)m_data.data(), m_data.size());
        outImage.close();
    }
}

void Bmp32::DrawRect(int x, int y, int w, int h, int r, int g, int b, int a)
{
    int idx{ 0 };
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            idx = IndexFromPos(x + i, y + j);
            m_data[idx] = b;
            m_data[idx + 1] = g;
            m_data[idx + 2] = r;
            m_data[idx + 3] = a;
        }
    }
}

void Bmp32::Negative()
{
    for (int i = 0; i < m_data.size(); i+=4)
    {
        m_data[i] = ~m_data[i];
        m_data[i + 1] = ~m_data[i + 1];
        m_data[i + 2] = ~m_data[i + 2];
    }
}


void Bmp32::Encode(std::string message)
{
    int dataIdx{ 0 };
    for (int i = 0; i < message.size(); i++)
    {
        for (int j = 7; j >= 0; j--)
        {
            if ((message[i] >> j) & 1)
            {
                m_data[dataIdx] = m_data[dataIdx] | 1;
            }
            else
            {
                m_data[dataIdx] = m_data[dataIdx] & ~1;
            }
            dataIdx++;
        }
    }
    m_colorHeader.unused[0] = message.size();
}

std::string Bmp32::Decode()
{
    char character{ 0 };
    std::string message;
    int bitCnt{ 0 };

    for (uint32_t i = 0; i < m_colorHeader.unused[0]*8; i++)
    {
        if (m_data[i] & 1)
        {
            character |= 1;
        }

        bitCnt++;
        if (bitCnt > 7)
        {
            message.push_back(character);
            character = 0;
            bitCnt = 0;
        }
        else
        {
            character <<= 1;
        }
    }
    return message;
}
