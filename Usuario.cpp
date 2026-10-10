#include "Usuario.h"
#include "Util.h"

Usuario::Usuario()
    : nombreUsuario(nullptr), fechaNacimiento(nullptr), correo(nullptr), rutaFoto(nullptr),
    clave(nullptr), ciudad(nullptr), pais(nullptr), fechaInscripcion(nullptr) {}

Usuario::Usuario(const char* nombre, const char* nacimiento, const char* mail,const char* foto, const char* pass, const char* ciu,const char* pa, const char* inscripcion)
    : nombreUsuario(Util::copiar(nombre)), fechaNacimiento(Util::copiar(nacimiento)),
    correo(Util::copiar(mail)), rutaFoto(Util::copiar(foto)), clave(Util::copiar(pass)),
    ciudad(Util::copiar(ciu)), pais(Util::copiar(pa)),
    fechaInscripcion(Util::copiar(inscripcion)) {}

Usuario::Usuario(const Usuario& otro)
{
    copiarDe(otro);
}

Usuario& Usuario::operator=(const Usuario& otro)
{
    if (this != &otro)
    {
        liberar();
        copiarDe(otro);
    }
    return *this;
}

Usuario::~Usuario()
{
    liberar();
}

void Usuario::copiarDe(const Usuario& otro)
{
    nombreUsuario = Util::copiar(otro.nombreUsuario);
    fechaNacimiento = Util::copiar(otro.fechaNacimiento);
    correo = Util::copiar(otro.correo);
    rutaFoto = Util::copiar(otro.rutaFoto);
    clave = Util::copiar(otro.clave);
    ciudad = Util::copiar(otro.ciudad);
    pais = Util::copiar(otro.pais);
    fechaInscripcion = Util::copiar(otro.fechaInscripcion);
}

void Usuario::liberar()
{
    delete[] nombreUsuario;  delete[] fechaNacimiento;
    delete[] correo;         delete[] rutaFoto;
    delete[] clave;          delete[] ciudad;
    delete[] pais;           delete[] fechaInscripcion;
    nombreUsuario = fechaNacimiento = correo = rutaFoto = nullptr;
    clave = ciudad = pais = fechaInscripcion = nullptr;
}

bool Usuario::claveCorrecta(const char* intento) const
{
    return clave && Util::iguales(clave, intento);
}

bool Usuario::operator==(const Usuario& otro) const
{
    return Util::iguales(getNombreUsuario(), otro.getNombreUsuario());
}
