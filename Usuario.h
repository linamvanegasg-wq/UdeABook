#ifndef USUARIO_H
#define USUARIO_H

#include "Album.h"

class Usuario
{
private:
    char* nombreUsuario;
    char* fechaNacimiento;
    char* correo;
    char* rutaFoto;
    char* clave;
    char* ciudad;
    char* pais;
    char* fechaInscripcion;
    Album* albumes;
    int numAlbumes;

    void copiarDe(const Usuario& otro);
    void liberar();

public:
    Usuario();
    Usuario(const char* nombre, const char* nacimiento, const char* correo,const char* foto, const char* clave, const char* ciudad, const char* pais, const char* inscripcion);
    Usuario(const Usuario& otro);
    Usuario& operator=(const Usuario& otro);
    ~Usuario();

    const char* getNombreUsuario() const { return nombreUsuario ? nombreUsuario : ""; }
    const char* getCorreo() const { return correo ? correo : ""; }
    const char* getRutaFoto() const { return rutaFoto ? rutaFoto : ""; }
    bool claveCorrecta(const char* intento) const;

    void agregarAlbum(const Album& a);
    int getNumAlbumes() const { return numAlbumes; }
    Album& getAlbum(int i) { return albumes[i]; }
    const Album& getAlbum(int i) const { return albumes[i]; }

    bool operator==(const Usuario& otro) const;

    long long memoriaDinamica() const;
};

#endif
