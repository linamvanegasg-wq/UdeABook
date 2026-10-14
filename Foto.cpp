#include "Foto.h"
#include "Util.h"

Foto::Foto() : fechaCaptura(nullptr), tamano(0), ruta(nullptr), likes(0) {}

Foto::Foto(const char* fecha, unsigned long tam, const char* rutaArchivo, unsigned int likesIniciales)
    : fechaCaptura(Util::copiar(fecha)), tamano(tam), ruta(Util::copiar(rutaArchivo)),
    likes(likesIniciales) {}

Foto::Foto(const Foto& otra)
{
    copiarDe(otra);
}

Foto& Foto::operator=(const Foto& otra)
{
    if (this != &otra)
    {
        liberar();
        copiarDe(otra);
    }
    return *this;
}

Foto::~Foto()
{ liberar();
}

void Foto::copiarDe(const Foto& otra)
{
    fechaCaptura = Util::copiar(otra.fechaCaptura);
    tamano = otra.tamano;
    ruta = Util::copiar(otra.ruta);
    likes = otra.likes;
}

void Foto::liberar()
{
    delete[] fechaCaptura;
    delete[] ruta;
    fechaCaptura = nullptr;
    ruta = nullptr;
}

void Foto::darQuitarLike(bool dar)
{
    if (dar) likes++;
    else if (likes > 0) likes--;
}

bool Foto::operator==(const Foto& otra) const
{
    return Util::iguales(getRuta(), otra.getRuta());
}

std::ostream& operator<<(std::ostream& os, const Foto& f)
{
    os << f.getRuta() << " | capturada: " << f.getFechaCaptura()
    << " | " << f.tamano << " bytes | likes: " << f.likes;
    return os;
}
