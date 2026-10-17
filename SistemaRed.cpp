#include "SistemaRed.h"
#include "Util.h"
#include "Metricas.h"
#include <iostream>
#include <fstream>

using std::cin;
using std::cout;
using std::ifstream;
using std::ofstream;

namespace {

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

bool leerLinea(char* buf, int n)
{
    cin.getline(buf, n);
    if (cin.eof() && cin.fail()) return false;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(100000, '\n');
    }
    quitarCR(buf);
    return true;
}

bool esOpcion(const char* s, char a)
{
    return s[0] == a && s[1] == '\0';
}

const char* etiquetaPrivacidad(int p) {
    if (p == Album::PUBLICO) return "publico";
    if (p == Album::AMIGOS) return "amigos";
    return "privado";
}


int elegirPonderado(const int* ids, const double* pesos, bool* usado, int n, int cuantos, int* salida) {
    int cant = 0;
    for (int s = 0; s < cuantos; s++)
    {
        double total = 0;
        for (int i = 0; i < n; i++) {
            Metricas::sumar();
            if (!usado[i]) total += pesos[i];
        }
        if (total <= 0) break;
        double r = Util::aleatorioReal() * total;
        double acum = 0;
        int elegido = -1;
        for (int i = 0; i < n; i++) {
            Metricas::sumar();
            if (usado[i]) continue;
            acum += pesos[i];
            elegido = i;
            if (r < acum) break;
        }
        usado[elegido] = true;
        salida[cant++] = ids[elegido];
    }
    return cant;
}

void mostrarFotosAmigos(const Usuario* usuarios, const int* ids, int cant) {
    if (cant == 0) {
        cout << "(No tienes amigos para mostrar)\n";
        return;
    }
    for (int i = 0; i < cant; i++)
        cout << "Foto de perfil del amigo " << (i + 1) << " (" << usuarios[ids[i]].getNombreUsuario()
             << "): " << usuarios[ids[i]].getRutaFoto() << "\n";
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

bool SistemaRed::sonAmigos(int a, int b) const
{
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

int SistemaRed::listarAmigos(int u, int* destino) const {
    int p = palabrasPorFila(), c = 0;
    for (int k = 0; k < p; k++) {
        Metricas::sumar();
        unsigned long long w = amistades[(long long)u * p + k];
        if (w == 0) continue;
        for (int b = 0; b < 64; b++) {
            Metricas::sumar();
            int j = k * 64 + b;
            if (j >= numUsuarios) break;
            if ((w >> b) & 1ULL) destino[c++] = j;
        }
    }
    return c;
}

int SistemaRed::primerMensajeDe(int u) const
{
    int lo = 0, hi = numMensajes;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        Metricas::sumar();
        if (mensajes[mid].getReceptor() < u) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

int SistemaRed::buscarUsuario(const char* nombre) const
{
    for (int i = 0; i < numUsuarios; i++) {
        Metricas::sumar();
        if (Util::iguales(usuarios[i].getNombreUsuario(), nombre)) return i;
    }
    return -1;
}


long long SistemaRed::memoriaEstructuras() const
{
    long long t = (long long)sizeof(*this);
    t += (long long)numUsuarios * (long long)sizeof(Usuario);
    for (int i = 0; i < numUsuarios; i++) t += usuarios[i].memoriaDinamica();
    t += (long long)numMensajes * (long long)sizeof(Mensaje);
    for (int i = 0; i < numMensajes; i++)
        t += Util::longitud(mensajes[i].getTexto()) + 1 + Util::longitud(mensajes[i].getFechaEnvio()) + 1;
    t += (long long)numSolicitudes * (long long)sizeof(SolicitudAmistad);
    for (int i = 0; i < numSolicitudes; i++)
        t += Util::longitud(solicitudes[i].getFechaEnvio()) + 1;
    t += (long long)numUsuarios * palabrasPorFila() * (long long)sizeof(unsigned long long);
    return t;
}


void SistemaRed::reportarRecursos(long long bytesLocales) const
{
    cout << "\n--- Consumo de recursos ---\n";
    cout << "Iteraciones: " << Metricas::iteraciones() << "\n";
    cout << "Llamadas a componentes externos (rand/time): " << Metricas::externos() << "\n";
    cout << "Memoria en uso: " << (memoriaEstructuras() + bytesLocales + LINEA) << " bytes\n";
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
    amistades = new unsigned long long[(long long)numUsuarios * palabrasPorFila()]();

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
        while (f.getline(linea, LINEA)) {
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
        while (f.getline(linea, LINEA))
        {
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
    if (n > 0)
    {
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
    if (n > 0)
    {
        mensajes = new Mensaje[n];
        ifstream f(ruta);
        while (f.getline(linea, LINEA))
        {
            quitarCR(linea);
            if (linea[0] == '\0') continue;
            Metricas::sumar();
            if (Util::partir(linea, '|', c, 5) < 5) continue;
            int e = Util::aEntero(c[0]), r = Util::aEntero(c[1]);
            if (e < 0 || r < 0 || e >= numUsuarios || r >= numUsuarios) continue;
            mensajes[numMensajes++] = Mensaje(e, r, c[4], c[2], c[3][0] == '1');
        }
    }


    if (numMensajes > 0)
    {
        int* inicio = new int[numUsuarios + 1]();
        for (int i = 0; i < numMensajes; i++) {
            Metricas::sumar();
            inicio[mensajes[i].getReceptor() + 1]++;
        }
        for (int i = 0; i < numUsuarios; i++) {
            Metricas::sumar();
            inicio[i + 1] += inicio[i];
        }
        Mensaje* ordenados = new Mensaje[numMensajes];
        for (int i = 0; i < numMensajes; i++) {
            Metricas::sumar();
            ordenados[inicio[mensajes[i].getReceptor()]++] = mensajes[i];
        }
        delete[] inicio;
        delete[] mensajes;
        mensajes = ordenados;
    }

    cout << "Datos cargados: " << numUsuarios << " usuarios, " << numMensajes << " mensajes, "<< numSolicitudes << " solicitudes pendientes.\n";reportarRecursos((long long)(sizeof(ruta) + sizeof(linea) + sizeof(c) + sizeof(n)));
    return true;
}

bool SistemaRed::guardarDatos(const char* carpeta) const
{
    char ruta[512];

    unirRuta(ruta, carpeta, "amistades.txt");
    {
        ofstream f(ruta);
        if (!f) return false;
        for (int a = 0; a < numUsuarios; a++)
            for (int b = a + 1; b < numUsuarios; b++)
                if (sonAmigos(a, b)) f << a << '|' << b << '\n';
    }

    unirRuta(ruta, carpeta, "solicitudes.txt");
    {
        ofstream f(ruta);
        if (!f) return false;
        for (int i = 0; i < numSolicitudes; i++)
            f << solicitudes[i].getSolicitante() << '|' << solicitudes[i].getReceptor() << '|'<< solicitudes[i].getFechaEnvio() << '\n';
    }

    unirRuta(ruta, carpeta, "mensajes.txt");
    {
        ofstream f(ruta);
        if (!f) return false;
        for (int i = 0; i < numMensajes; i++)
            f << mensajes[i].getEmisor() << '|' << mensajes[i].getReceptor() << '|'<< mensajes[i].getFechaEnvio() << '|' << (mensajes[i].estaLeido() ? 1 : 0) << '|'<< mensajes[i].getTexto() << '\n';
    }

    unirRuta(ruta, carpeta, "fotos.txt");
    {
        ofstream f(ruta);
        if (!f) return false;
        for (int u = 0; u < numUsuarios; u++)
            for (int a = 0; a < usuarios[u].getNumAlbumes(); a++) {
                const Album& al = usuarios[u].getAlbum(a);
                for (int p = 0; p < al.getNumFotos(); p++) {
                    const Foto& fo = al.getFoto(p);
                    f << u << '|' << a << '|' << fo.getFechaCaptura() << '|' << fo.getTamano() << '|'
                      << fo.getRuta() << '|' << fo.getLikes() << '\n';
                }
            }
    }
    return true;
}


int SistemaRed::iniciarSesion(const char* nombre, const char* clave) const {
    for (int i = 0; i < numUsuarios; i++) {
        Metricas::sumar();
        if (Util::iguales(usuarios[i].getNombreUsuario(), nombre))
            return usuarios[i].claveCorrecta(clave) ? i : -1;
    }
    return -1;
}

void SistemaRed::ejecutar() {
    char nombre[LINEA];
    char clave[LINEA];
    while (true) {
        cout << "\n=== Ude@Book ===\nUsuario (S para salir): ";
        if (!leerLinea(nombre, LINEA)) break;
        if (nombre[0] == '\0') continue;
        if (esOpcion(nombre, 'S') || esOpcion(nombre, 's')) break;
        cout << "Contrasena: ";
        if (!leerLinea(clave, LINEA)) break;
        Metricas::reiniciar();
        int u = iniciarSesion(nombre, clave);
        if (u < 0) {
            cout << "Credenciales incorrectas.\n";
            continue;
        }
        sesion(u);
    }
}

void SistemaRed::sesion(int u)
{
    int nAm = contarAmigos(u);
    int tam = nAm > 0 ? nAm : 1;
    int* amigos = new int[tam];
    double* pesos = new double[tam];
    bool* usado = new bool[tam]();
    listarAmigos(u, amigos);
    for (int i = 0; i < nAm; i++) {
        int comun = amigosEnComun(u, amigos[i]);
        pesos[i] = (comun <= 10) ? 1.0 : (comun <= 20 ? 2.5 : 3.5);
    }
    int set[3];
    int cant = elegirPonderado(amigos, pesos, usado, nAm, 3, set);
    bool actualizado = false;

    mostrarFotosAmigos(usuarios, set, cant);
    cout << "\nBienvenido a UdeaBook " << usuarios[u].getNombreUsuario() << "\n";
    long long locales = (long long)(sizeof(u) + sizeof(nAm) + sizeof(tam) + sizeof(set) + sizeof(cant) +sizeof(actualizado) + 3 * sizeof(void*)) +(long long)tam * (long long)(sizeof(int) + sizeof(double) + sizeof(bool));reportarRecursos(locales);

    char op[LINEA];
    while (true)
    {
        cout << "\nMenu de opciones:\n"
                "1.- Administrar amistades\n"
                "2.- Visitar perfil de un amigo\n"
                "3.- Ver sugerencias de amistad\n"
                "4.- Administrar mensajes\n"
                ".- Ingrese 'N' o 'n' para cambiar las 3 fotos de amigos (solo una vez).\n"
                ".- Ingrese 'S' o 's' para cerrar su sesion.\n"
                "Su opcion: ";
        if (!leerLinea(op, LINEA)) break;

        if (esOpcion(op, 'S') || esOpcion(op, 's')) break;

        if (esOpcion(op, 'N') || esOpcion(op, 'n')) {
            Metricas::reiniciar();
            if (actualizado) {
                cout << "Las fotos de amigos solo se pueden actualizar una vez.\n";
            } else {
                actualizado = true;
                cant = elegirPonderado(amigos, pesos, usado, nAm, 3, set);
                mostrarFotosAmigos(usuarios, set, cant);
            }
            reportarRecursos(locales);
        } else if (esOpcion(op, '1')) {
            menuAmistades(u);
        } else if (esOpcion(op, '2')) {
            char nombre[LINEA];
            cout << "Nombre de usuario del amigo: ";
            if (!leerLinea(nombre, LINEA)) break;
            Metricas::reiniciar();
            visitarPerfil(u, nombre);
        } else if (esOpcion(op, '3')) {
            char buf[LINEA];
            cout << "Cuantas sugerencias desea ver (1-20): ";
            if (!leerLinea(buf, LINEA)) break;
            int k = Util::aEntero(buf);
            if (k < 1 || k > 20) {
                cout << "K debe estar entre 1 y 20.\n";
                continue;
            }
            int top[20];
            Metricas::reiniciar();
            int nTop = sugerirAmigos(u, k, top);
            if (nTop > 0) {
                cout << "\nNumero de la sugerencia a la que enviar solicitud (0 = ninguna): ";
                if (!leerLinea(buf, LINEA)) break;
                int sel = Util::aEntero(buf);
                if (sel >= 1 && sel <= nTop) {
                    Metricas::reiniciar();
                    enviarSolicitud(u, usuarios[top[sel - 1]].getNombreUsuario());
                }
            }
        } else if (esOpcion(op, '4')) {
            menuMensajes(u);
        } else {
            cout << "Opcion no valida.\n";
        }
    }

    delete[] amigos;
    delete[] pesos;
    delete[] usado;
    cout << "Sesion cerrada.\n";
}


void SistemaRed::menuAmistades(int u) {
    char op[LINEA];
    while (true) {
        cout << "\n-- Administrar amistades --\n"
                "1.- Enviar solicitud de amistad\n"
                "2.- Aceptar solicitudes recibidas\n"
                "0.- Volver\n"
                "Su opcion: ";
        if (!leerLinea(op, LINEA)) return;

        if (esOpcion(op, '1')) {
            char nombre[LINEA];
            cout << "Nombre de usuario destino: ";
            if (!leerLinea(nombre, LINEA)) return;
            Metricas::reiniciar();
            enviarSolicitud(u, nombre);
        } else if (esOpcion(op, '2')) {
            Metricas::reiniciar();
            int cuenta = 0;
            for (int i = 0; i < numSolicitudes; i++) {
                Metricas::sumar();
                if (solicitudes[i].getReceptor() == u) cuenta++;
            }
            if (cuenta == 0) {
                cout << "No tienes solicitudes pendientes.\n";
                reportarRecursos((long long)(sizeof(u) + sizeof(cuenta)));
                continue;
            }
            int* pos = new int[cuenta];
            int k = 0;
            cout << "Solicitudes recibidas:\n";
            for (int i = 0; i < numSolicitudes; i++) {
                Metricas::sumar();
                if (solicitudes[i].getReceptor() == u) {
                    pos[k++] = i;
                    cout << "  " << k << ". " << usuarios[solicitudes[i].getSolicitante()].getNombreUsuario() << " (" << solicitudes[i].getFechaEnvio() << ")\n";
                }
            }
            char buf[LINEA];
            cout << "Numero de la solicitud a aceptar (0 = ninguna): ";
            if (!leerLinea(buf, LINEA)) {
                delete[] pos;
                return;
            }
            int sel = Util::aEntero(buf);
            if (sel >= 1 && sel <= cuenta) {
                aceptarSolicitud(u, pos[sel - 1]);
            } else if (sel != 0) {
                cout << "Seleccion no valida.\n";
            }
            delete[] pos;
        } else if (esOpcion(op, '0')) {
            return;
        } else {
            cout << "Opcion no valida.\n";
        }
    }
}

bool SistemaRed::crearSolicitud(int u, int v) {
    if (v == u) {
        cout << "No puedes enviarte una solicitud a ti mismo.\n";
        return false;
    }
    if (sonAmigos(u, v)) {
        cout << "Ya eres amigo de " << usuarios[v].getNombreUsuario() << ".\n";
        return false;
    }
    for (int i = 0; i < numSolicitudes; i++) {
        Metricas::sumar();
        int s = solicitudes[i].getSolicitante(), r = solicitudes[i].getReceptor();
        if (s == u && r == v) {
            cout << "Ya enviaste una solicitud pendiente a ese usuario.\n";
            return false;
        }
        if (s == v && r == u) {
            cout << "Ese usuario ya te envio una solicitud: aceptala en 'Aceptar solicitudes recibidas'.\n";
            return false;
        }
    }
    char fecha[17];
    Util::fechaActual(fecha);
    agregarAlFinal(solicitudes, numSolicitudes, SolicitudAmistad(u, v, fecha));
    cout << "Solicitud enviada a " << usuarios[v].getNombreUsuario() << ".\n";
    return true;
}

void SistemaRed::enviarSolicitud(int u, const char* nombre) {
    int v = buscarUsuario(nombre);
    if (v < 0) cout << "Ese usuario no existe.\n";
    else crearSolicitud(u, v);
    reportarRecursos((long long)(sizeof(u) + sizeof(nombre) + sizeof(v)));
}

void SistemaRed::aceptarSolicitud(int u, int indice) {
    if (indice < 0 || indice >= numSolicitudes || solicitudes[indice].getReceptor() != u) {
        cout << "Solicitud no valida.\n";
    } else {
        int amigo = solicitudes[indice].getSolicitante();
        activarAmistad(u, amigo);
        eliminarEn(solicitudes, numSolicitudes, indice);
        cout << "Ahora eres amigo de " << usuarios[amigo].getNombreUsuario() << ".\n";
    }
    reportarRecursos((long long)(sizeof(u) + sizeof(indice)));
}


void SistemaRed::visitarPerfil(int u, const char* amigo) {
    int v = buscarUsuario(amigo);
    if (v < 0 || !sonAmigos(u, v)) {
        cout << "Ese usuario no existe o no es tu amigo.\n";
        reportarRecursos((long long)(sizeof(u) + sizeof(amigo) + sizeof(v)));
        return;
    }

    int na = usuarios[v].getNumAlbumes();
    int* visibles = new int[na > 0 ? na : 1];
    int nv = 0;
    cout << "Albumes de " << usuarios[v].getNombreUsuario() << ":\n";
    for (int i = 0; i < na; i++) {
        Metricas::sumar();
        const Album& al = usuarios[v].getAlbum(i);
        if (al.getPrivacidad() == Album::PUBLICO || al.getPrivacidad() == Album::AMIGOS) {
            visibles[nv++] = i;
            cout << "  " << nv << ". " << al.getNombre() << " (" << etiquetaPrivacidad(al.getPrivacidad())
                 << ", " << al.getNumFotos() << " fotos)\n";
        }
    }
    long long locales = (long long)(sizeof(u) + sizeof(amigo) + sizeof(v) + sizeof(na) + sizeof(nv) +sizeof(visibles)) + (long long)na * (long long)sizeof(int);
    if (nv == 0) {
        cout << "No hay albumes visibles.\n";
        reportarRecursos(locales);
        delete[] visibles;
        return;
    }

    char buf[LINEA];
    cout << "Numero del album a abrir (0 = volver): ";
    if (!leerLinea(buf, LINEA)) {
        delete[] visibles;
        return;
    }
    int sel = Util::aEntero(buf);
    if (sel < 1 || sel > nv) {
        reportarRecursos(locales);
        delete[] visibles;
        return;
    }

    Album& album = usuarios[v].getAlbum(visibles[sel - 1]);
    int nf = album.getNumFotos();
    if (nf == 0) {
        cout << "El album esta vacio.\n";
        reportarRecursos(locales);
        delete[] visibles;
        return;
    }

    bool* gustadas = new bool[nf]();
    int pos = 0;
    while (true) {
        Metricas::sumar();
        cout << "\nFoto " << (pos + 1) << "/" << nf << ": " << album.getFoto(pos) << "\n"<< "[S] siguiente   [P] previa   [L] dar/quitar like   [X] salir del album: ";
        if (!leerLinea(buf, LINEA)) break;
        if (esOpcion(buf, 'X') || esOpcion(buf, 'x')) break;
        else if (esOpcion(buf, 'S') || esOpcion(buf, 's')) pos = (pos + 1) % nf;
        else if (esOpcion(buf, 'P') || esOpcion(buf, 'p')) pos = (pos - 1 + nf) % nf;
        else if (esOpcion(buf, 'L') || esOpcion(buf, 'l')) {
            gustadas[pos] = !gustadas[pos];
            album.getFoto(pos).darQuitarLike(gustadas[pos]);
        } else cout << "Opcion no valida.\n";
    }
    reportarRecursos(locales + (long long)(sizeof(sel) + sizeof(album) + sizeof(nf) + sizeof(gustadas) + sizeof(pos)) + (long long)nf * (long long)sizeof(bool));
    delete[] gustadas;
    delete[] visibles;
}

int SistemaRed::sugerirAmigos(int u, int k, int* top) {
    int* tally = new int[numUsuarios]();
    int* amigos = new int[numUsuarios];
    int nAm = listarAmigos(u, amigos);
    int p = palabrasPorFila();

    for (int a = 0; a < nAm; a++) {
        int f = amigos[a];
        for (int w = 0; w < p; w++) {
            Metricas::sumar();
            unsigned long long palabra = amistades[(long long)f * p + w];
            if (palabra == 0) continue;
            for (int b = 0; b < 64; b++) {
                Metricas::sumar();
                int c = w * 64 + b;
                if (c >= numUsuarios) break;
                if (((palabra >> b) & 1ULL) && c != u && !sonAmigos(u, c)) tally[c]++;
            }
        }
    }

    int cuentas[20];
    int nTop = 0;
    for (int c = 0; c < numUsuarios; c++) {
        Metricas::sumar();
        if (tally[c] == 0) continue;
        int pos = nTop;
        while (pos > 0 && cuentas[pos - 1] < tally[c]) {
            Metricas::sumar();
            pos--;
        }
        if (pos >= k) continue;
        int fin = (nTop < k) ? nTop : k - 1;
        for (int j = fin; j > pos; j--) {
            Metricas::sumar();
            top[j] = top[j - 1];
            cuentas[j] = cuentas[j - 1];
        }
        top[pos] = c;
        cuentas[pos] = tally[c];
        if (nTop < k) nTop++;
    }

    if (nTop == 0) {
        cout << "No hay sugerencias disponibles.\n";
    } else {
        cout << "Sugerencias de amistad:\n";
        for (int i = 0; i < nTop; i++)
            cout << "  " << (i + 1) << ". " << usuarios[top[i]].getNombreUsuario() << " - "
                 << cuentas[i] << " amigos en comun\n";
    }
    reportarRecursos((long long)(sizeof(u) + sizeof(k) + sizeof(top) + sizeof(nAm) + sizeof(p) +sizeof(cuentas) + sizeof(nTop) + 2 * sizeof(void*)) +2LL * numUsuarios * (long long)sizeof(int));
    delete[] tally;
    delete[] amigos;
    return nTop;
}

void SistemaRed::menuMensajes(int u)
{
    char op[LINEA];
    while (true) {
        cout << "\n-- Administrar mensajes --\n"
                "1.- Enviar mensaje\n"
                "2.- Leer nuevos mensajes\n"
                "3.- Buscar mensajes por rango de fecha\n"
                "0.- Volver\n"
                "Su opcion: ";
        if (!leerLinea(op, LINEA)) return;

        if (esOpcion(op, '1'))
        {
            char nombre[LINEA];
            char texto[LINEA];
            cout << "Nombre del amigo: ";
            if (!leerLinea(nombre, LINEA)) return;
            cout << "Mensaje (max. 512 caracteres): ";
            if (!leerLinea(texto, LINEA)) return;
            Metricas::reiniciar();
            enviarMensaje(u, nombre, texto);
        } else if (esOpcion(op, '2')) {
            Metricas::reiniciar();
            leerMensajesNuevos(u);
        } else if (esOpcion(op, '3')) {
            char ini[LINEA];
            char fin[LINEA];
            cout << "Fecha de inicio (AAAA-MM-DD): ";
            if (!leerLinea(ini, LINEA)) return;
            cout << "Fecha de fin (AAAA-MM-DD): ";
            if (!leerLinea(fin, LINEA)) return;
            Metricas::reiniciar();
            buscarMensajes(u, ini, fin);
        } else if (esOpcion(op, '0')) {
            return;
        } else {
            cout << "Opcion no valida.\n";
        }
    }
}

void SistemaRed::enviarMensaje(int u, const char* amigo, const char* texto)
{
    int v = buscarUsuario(amigo);
    int largo = Util::longitud(texto);
    if (v < 0 || !sonAmigos(u, v)) {
        cout << "Ese usuario no existe o no es tu amigo.\n";
    } else if (largo == 0) {
        cout << "No se pueden enviar mensajes vacios.\n";
    } else if (largo > 512) {
        cout << "El mensaje supera los 512 caracteres.\n";
    } else {
        char fecha[17];
        Util::fechaActual(fecha);
        insertarEn(mensajes, numMensajes, primerMensajeDe(v + 1), Mensaje(u, v, texto, fecha, false));
        cout << "Mensaje enviado a " << usuarios[v].getNombreUsuario() << ".\n";
    }
    reportarRecursos((long long)(sizeof(u) + sizeof(amigo) + sizeof(texto) + sizeof(v) + sizeof(largo) + 17) +(largo > 0 && largo <= 512 ? (long long)sizeof(Mensaje) + largo + 1 + 17 : 0));
}

void SistemaRed::leerMensajesNuevos(int u) {
    int mostrados = 0;
    int fin = primerMensajeDe(u + 1);
    for (int i = primerMensajeDe(u); i < fin; i++) {
        Metricas::sumar();
        if (!mensajes[i].estaLeido()) {
            if (mostrados == 0) cout << "Mensajes nuevos:\n";
            cout << "  De " << usuarios[mensajes[i].getEmisor()].getNombreUsuario() << " "<< mensajes[i] << "\n";
            mensajes[i].marcarLeido();
            mostrados++;
        }
    }
    if (mostrados == 0) cout << "No tienes mensajes nuevos.\n";
    reportarRecursos((long long)(sizeof(u) + sizeof(mostrados) + sizeof(fin)));
}

void SistemaRed::buscarMensajes(int u, const char* inicio, const char* fin) {
    if (!Util::fechaValida(inicio) || !Util::fechaValida(fin) || Util::comparar(inicio, fin) > 0) {
        cout << "Fechas no validas (formato AAAA-MM-DD y inicio <= fin).\n";
        reportarRecursos((long long)(sizeof(u) + sizeof(inicio) + sizeof(fin)));
        return;
    }
    int desde = primerMensajeDe(u);
    int hasta = primerMensajeDe(u + 1);
    int lo = desde, hi = hasta;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        Metricas::sumar();
        if (Util::comparar(mensajes[mid].getFechaEnvio(), inicio, 10) < 0) lo = mid + 1;
        else hi = mid;
    }
    int encontrados = 0;
    for (int i = lo; i < hasta; i++) {
        Metricas::sumar();
        if (Util::comparar(mensajes[i].getFechaEnvio(), fin, 10) > 0) break;   // paso el rango
        cout << "  De " << usuarios[mensajes[i].getEmisor()].getNombreUsuario() << " "<< mensajes[i] << "\n";
        encontrados++;
    }
    cout << encontrados << " mensaje(s) en el rango.\n";
    reportarRecursos((long long)(sizeof(u) + sizeof(inicio) + sizeof(fin) + sizeof(desde) + sizeof(hasta) + sizeof(lo) + sizeof(hi) + sizeof(encontrados)));
}
