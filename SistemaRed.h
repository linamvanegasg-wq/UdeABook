#ifndef SISTEMARED_H
#define SISTEMARED_H

#include "Usuario.h"
#include "Mensaje.h"
#include "SolicitudAmistad.h"

class SistemaRed
{
private:
    Usuario* usuarios;
    Mensaje* mensajes;
    SolicitudAmistad* solicitudes;
    unsigned long long* amistades;
    int numUsuarios;
    int numMensajes;
    int numSolicitudes;

    int palabrasPorFila() const { return (numUsuarios + 63) / 64; }
    void activarAmistad(int a, int b);
    void liberar();

    SistemaRed(const SistemaRed&) = delete;
    SistemaRed& operator=(const SistemaRed&) = delete;

public:
    SistemaRed();
    ~SistemaRed();

    bool cargarDatos(const char* carpeta);
    int iniciarSesion(const char* nombre, const char* clave) const;

    bool sonAmigos(int a, int b) const;
    int contarAmigos(int u) const;
    int amigosEnComun(int a, int b) const;

    int getNumUsuarios() const { return numUsuarios; }
    const Usuario& getUsuario(int i) const { return usuarios[i]; }
};

#endif
