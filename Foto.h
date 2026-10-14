#ifndef FOTO_H
#define FOTO_H

#include <iostream>

class Foto
{
private:
    char* fechaCaptura;
    unsigned long tamano;
    char* ruta;
    unsigned int likes;

    void copiarDe(const Foto& otra);
    void liberar();

public:
    Foto();
    Foto(const char* fecha, unsigned long tam, const char* rutaArchivo, unsigned int likesIniciales);
    Foto(const Foto& otra);
    Foto& operator=(const Foto& otra);
    ~Foto();

    void darQuitarLike(bool dar);

    const char* getFechaCaptura() const { return fechaCaptura ? fechaCaptura : ""; }
    unsigned long getTamano() const { return tamano; }
    const char* getRuta() const { return ruta ? ruta : ""; }
    unsigned int getLikes() const { return likes; }

    bool operator==(const Foto& otra) const;

    friend std::ostream& operator<<(std::ostream& os, const Foto& f);
};

#endif
