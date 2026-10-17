#include "Album.h"
#include "Util.h"

Album::Album() : nombre(nullptr), ruta(nullptr), privacidad(PUBLICO), fotos(nullptr), numFotos(0) {}

Album::Album(const char* nombreAlbum, const char* rutaAlbum, int priv): nombre(Util::copiar(nombreAlbum)), ruta(Util::copiar(rutaAlbum)), privacidad(priv),fotos(nullptr), numFotos(0) {}

Album::Album(const Album& otro)
{
    copiarDe(otro);
}

Album& Album::operator=(const Album& otro)
{
    if (this != &otro)
    {
        liberar();
        copiarDe(otro);
    }
    return *this;
}

Album::~Album()
{
    liberar();
}

void Album::copiarDe(const Album& otro)
{
    nombre = Util::copiar(otro.nombre);
    ruta = Util::copiar(otro.ruta);
    privacidad = otro.privacidad;
    numFotos = otro.numFotos;
    fotos = copiarArreglo(otro.fotos, otro.numFotos);
}

void Album::liberar()
{
    delete[] nombre;
    delete[] ruta;
    delete[] fotos;
    nombre = nullptr;
    ruta = nullptr;
    fotos = nullptr;
    numFotos = 0;
}

void Album::agregarFoto(const Foto& f)
{
    agregarAlFinal(fotos, numFotos, f);
}
