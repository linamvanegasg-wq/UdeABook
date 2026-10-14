#ifndef ALBUM_H
#define ALBUM_H

#include "Foto.h"

class Album
{
public:
    enum Privacidad { PUBLICO = 0, AMIGOS = 1, PRIVADO = 2 };

private:
    char* nombre;
    char* ruta;
    int privacidad;
    Foto* fotos;
    int numFotos;

    void copiarDe(const Album& otro);
    void liberar();

public:
    Album();
    Album(const char* nombreAlbum, const char* rutaAlbum, int priv);
    Album(const Album& otro);
    Album& operator=(const Album& otro);
    ~Album();

    void agregarFoto(const Foto& f);

    const char* getNombre() const { return nombre ? nombre : ""; }
    const char* getRuta() const { return ruta ? ruta : ""; }
    int getPrivacidad() const { return privacidad; }
    int getNumFotos() const { return numFotos; }
    Foto& getFoto(int i) { return fotos[i]; }
    const Foto& getFoto(int i) const { return fotos[i]; }
};

#endif
