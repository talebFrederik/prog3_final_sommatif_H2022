#include <iostream>

#include "ByteDecorator.h"
#include "Bmp32.h"

int main()
{
    // Encodage/décodage de base
    Bmp32 bmp{ "C:/Users/ftaleb/Documents/H2022_prog3/Shapes.bmp" };
    bmp.Encode("La session est finie");
    bmp.Save("Steg_00.bmp");
    Bmp32 bmpBase{ "Steg_00.bmp" };
    std::cout << bmpBase.Decode() << "\n";

    // Encodage avec une clé 8 octets
    Bmp32 bmp8{ "C:/Users/ftaleb/Documents/H2022_prog3/Shapes.bmp" };
    ByteDecorator byteEncoder(bmp8);
    byteEncoder.Encode("La vita est bella");
    // ATTENTION : si on utilise un décorateur, il faut utiliser la méthode save du décorateur
    byteEncoder.Save("Steg_01.bmp");

    // Décodage de l'image avec la clé 8 octets
    Bmp32 bmpSteg{ "Steg_01.bmp" };
    ByteDecorator byteDecoder(bmpSteg);
    std::cout << byteDecoder.Decode();
}

