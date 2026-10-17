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
    int buscarUsuario(const char* nombre) const;
    int listarAmigos(int u, int* destino) const;
    int primerMensajeDe(int u) const;
    bool crearSolicitud(int u, int v);

    long long memoriaEstructuras() const;
    void reportarRecursos(long long bytesLocales) const;

    void sesion(int u);
    void menuAmistades(int u);
    void menuMensajes(int u);

    SistemaRed(const SistemaRed&) = delete;
    SistemaRed& operator=(const SistemaRed&) = delete;

public:
    SistemaRed();
    ~SistemaRed();

    bool cargarDatos(const char* carpeta);
    bool guardarDatos(const char* carpeta) const;
    int iniciarSesion(const char* nombre, const char* clave) const;
    void ejecutar();

    bool sonAmigos(int a, int b) const;
    int contarAmigos(int u) const;
    int amigosEnComun(int a, int b) const;

    void enviarSolicitud(int u, const char* nombre);
    void aceptarSolicitud(int u, int indice);
    int sugerirAmigos(int u, int k, int* top);
    void visitarPerfil(int u, const char* amigo);
    void enviarMensaje(int u, const char* amigo, const char* texto);
    void leerMensajesNuevos(int u);
    void buscarMensajes(int u, const char* inicio, const char* fin);

    int getNumUsuarios() const { return numUsuarios; }
    const Usuario& getUsuario(int i) const { return usuarios[i]; }
};

#endif
