#include "SistemaRed.h"
#include "Util.h"
#include "Metricas.h"
#include <iostream>
#include <fstream>

using std::cout;
using std::ifstream;

namespace
{

const int LINEA = 2048;

void quitarCR(char* s)
{
    int n = Util::longitud(s);
    if (n > 0 && s[n - 1] == '\r') s[n - 1] = '\0';
}

void unirRuta(char* destino, const char* carpeta, const char* archivo)
{
    int k = 0;
    for (int i = 0; carpeta[i] && k < 500; i++) destino[k++] = carpeta[i];
    if (k > 0 && destino[k - 1] != '/' && destino[k - 1] != '\\') destino[k++] = '/';
    for (int i = 0; archivo[i] && k < 510; i++) destino[k++] = archivo[i];
    destino[k] = '\0';
}

int contarLineas(const char* ruta)
{
    ifstream f(ruta);
    if (!f) return 0;
    char linea[LINEA];
    int c = 0;
    while (f.getline(linea, LINEA))
    {
        Metricas::sumar();
        if (linea[0] != '\0') c++;
    }
    return c;
}

int bitsEncendidos(unsigned long long w)
{
    int c = 0;
    while (w) {
        w &= (w - 1);
        c++;
        Metricas::sumar();
    }
    return c;
}
}


SistemaRed::SistemaRed()
    : usuarios(nullptr), mensajes(nullptr), solicitudes(nullptr), amistades(nullptr),
    numUsuarios(0), numMensajes(0), numSolicitudes(0) {}

SistemaRed::~SistemaRed() { liberar(); }

void SistemaRed::liberar()
{
    delete[] usuarios;
    delete[] mensajes;
    delete[] solicitudes;
    delete[] amistades;
    usuarios = nullptr;
    mensajes = nullptr;
    solicitudes = nullptr;
    amistades = nullptr;
    numUsuarios = numMensajes = numSolicitudes = 0;
}

void SistemaRed::activarAmistad(int a, int b)
{
    int p = palabrasPorFila();
    amistades[(long long)a * p + (b >> 6)] |= (1ULL << (b & 63));
    amistades[(long long)b * p + (a >> 6)] |= (1ULL << (a & 63));
}

bool SistemaRed::sonAmigos(int a, int b) const {
    int p = palabrasPorFila();
    return (amistades[(long long)a * p + (b >> 6)] >> (b & 63)) & 1ULL;
}

int SistemaRed::contarAmigos(int u) const
{
    int p = palabrasPorFila(), total = 0;
    for (int k = 0; k < p; k++) {
        Metricas::sumar();
        total += bitsEncendidos(amistades[(long long)u * p + k]);
    }
    return total;
}

int SistemaRed::amigosEnComun(int a, int b) const
{
    int p = palabrasPorFila(), total = 0;
    for (int k = 0; k < p; k++) {
        Metricas::sumar();
        total += bitsEncendidos(amistades[(long long)a * p + k] & amistades[(long long)b * p + k]);
    }
    return total;
}

bool SistemaRed::cargarDatos(const char* carpeta)
{
    liberar();
    Metricas::reiniciar();
    char ruta[512];
    char linea[LINEA];
    char* c[8];

    unirRuta(ruta, carpeta, "usuarios.txt");
    int n = contarLineas(ruta);
    if (n <= 0) return false;
    usuarios = new Usuario[n];
    {
        ifstream f(ruta);
        while (f.getline(linea, LINEA)) {
            quitarCR(linea);
            if (linea[0] == '\0') continue;
            Metricas::sumar();
            if (Util::partir(linea, '|', c, 8) < 8) continue;
            usuarios[numUsuarios++] = Usuario(c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7]);
        }
    }
    if (numUsuarios == 0) return false;
    amistades = new unsigned long long[(long long)numUsuarios * palabrasPorFila()]();   // todo en 0

    unirRuta(ruta, carpeta, "albumes.txt");
    {
        ifstream f(ruta);
        while (f.getline(linea, LINEA)) {
            quitarCR(linea);
            if (linea[0] == '\0') continue;
            Metricas::sumar();
            if (Util::partir(linea, '|', c, 4) < 4) continue;
            int id = Util::aEntero(c[0]);
            if (id < 0 || id >= numUsuarios) continue;
            usuarios[id].agregarAlbum(Album(c[1], c[2], Util::aEntero(c[3])));
        }
    }

    unirRuta(ruta, carpeta, "fotos.txt");
    {
        ifstream f(ruta);
        while (f.getline(linea, LINEA))
        {
            quitarCR(linea);
            if (linea[0] == '\0') continue;
            Metricas::sumar();
            if (Util::partir(linea, '|', c, 6) < 6) continue;
            int id = Util::aEntero(c[0]);
            int al = Util::aEntero(c[1]);
            if (id < 0 || id >= numUsuarios) continue;
            if (al < 0 || al >= usuarios[id].getNumAlbumes()) continue;
            usuarios[id].getAlbum(al).agregarFoto(
                Foto(c[2], Util::aEnteroLargo(c[3]), c[4], (unsigned int)Util::aEnteroLargo(c[5])));
        }
    }

    unirRuta(ruta, carpeta, "amistades.txt");
    {
        ifstream f(ruta);
        while (f.getline(linea, LINEA)) {
            quitarCR(linea);
            if (linea[0] == '\0') continue;
            Metricas::sumar();
            if (Util::partir(linea, '|', c, 2) < 2) continue;
            int a = Util::aEntero(c[0]), b = Util::aEntero(c[1]);
            if (a < 0 || b < 0 || a >= numUsuarios || b >= numUsuarios || a == b) continue;
            activarAmistad(a, b);
        }
    }

    unirRuta(ruta, carpeta, "solicitudes.txt");
    n = contarLineas(ruta);
    if (n > 0) {
        solicitudes = new SolicitudAmistad[n];
        ifstream f(ruta);
        while (f.getline(linea, LINEA)) {
            quitarCR(linea);
            if (linea[0] == '\0') continue;
            Metricas::sumar();
            if (Util::partir(linea, '|', c, 3) < 3) continue;
            int a = Util::aEntero(c[0]), b = Util::aEntero(c[1]);
            if (a < 0 || b < 0 || a >= numUsuarios || b >= numUsuarios) continue;
            solicitudes[numSolicitudes++] = SolicitudAmistad(a, b, c[2]);
        }
    }

    unirRuta(ruta, carpeta, "mensajes.txt");
    n = contarLineas(ruta);
    if (n > 0) {
        mensajes = new Mensaje[n];
        ifstream f(ruta);
        while (f.getline(linea, LINEA)) {
            quitarCR(linea);
            if (linea[0] == '\0') continue;
            Metricas::sumar();
            if (Util::partir(linea, '|', c, 5) < 5) continue;
            int e = Util::aEntero(c[0]), r = Util::aEntero(c[1]);
            if (e < 0 || r < 0 || e >= numUsuarios || r >= numUsuarios) continue;
            mensajes[numMensajes++] = Mensaje(e, r, c[4], c[2], c[3][0] == '1');
        }
    }

    cout << "Datos cargados: " << numUsuarios << " usuarios, " << numMensajes << " mensajes, " << numSolicitudes << " solicitudes pendientes.\n";
    return true;
}

int SistemaRed::iniciarSesion(const char* nombre, const char* clave) const {
    for (int i = 0; i < numUsuarios; i++)
    {
        Metricas::sumar();
        if (Util::iguales(usuarios[i].getNombreUsuario(), nombre))
            return usuarios[i].claveCorrecta(clave) ? i : -1;
    }
    return -1;
}
