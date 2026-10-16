#include <iostream>
#include <fstream>
#include "SistemaRed.h"
#include "Metricas.h"

using namespace std;

static bool buscarCarpetaDatos(char* destino)
{
    for (int niveles = 0; niveles <= 6; niveles++)
    {
        int k = 0;
        for (int n = 0; n < niveles; n++) { destino[k++] = '.'; destino[k++] = '.'; destino[k++] = '/'; }
        const char* nombre = "datos";
        for (int i = 0; nombre[i]; i++) destino[k++] = nombre[i];
        destino[k] = '\0';

        char archivo[160];
        int j = 0;
        for (int i = 0; destino[i]; i++) archivo[j++] = destino[i];
        const char* f = "/usuarios.txt";
        for (int i = 0; f[i]; i++) archivo[j++] = f[i];
        archivo[j] = '\0';

        ifstream prueba(archivo);
        if (prueba) return true;
    }
    return false;
}

int main(int argc, char** argv)
{
    char carpeta[160];
    if (argc > 1) {
        int i = 0;
        for (; argv[1][i] && i < 159; i++) carpeta[i] = argv[1][i];
        carpeta[i] = '\0';
    } else if (!buscarCarpetaDatos(carpeta)) {
        cout << "No se encontro la carpeta 'datos'.\n";
        return 1;
    }

    SistemaRed red;
    if (!red.cargarDatos(carpeta)) {
        cout << "No se pudieron cargar los datos.\n";
        return 1;
    }
    cout << "Iteraciones al cargar: " << Metricas::iteraciones() << "\n";

    Metricas::reiniciar();
    int u = red.iniciarSesion("usuario0001", "clave0001");
    cout << "login correcto: " << (u >= 0) << " (indice " << u << ")  iteraciones: "<< Metricas::iteraciones() << "\n";
    cout << "login con clave mala: " << red.iniciarSesion("usuario0001", "mala") << "\n";

    Metricas::reiniciar();
    cout << "amigos de usuario0001: " << red.contarAmigos(u) << "\n";
    cout << "amigos en comun (0,1): " << red.amigosEnComun(0, 1) << "  son amigos: " << red.sonAmigos(0, 1) << "\n";
    cout << "iteraciones: " << Metricas::iteraciones() << "\n";
    return 0;
}
