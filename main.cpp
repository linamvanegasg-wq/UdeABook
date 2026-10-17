#include <iostream>
#include <fstream>
#include "SistemaRed.h"
#include "Util.h"

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

    Util::iniciarAleatorio();
    SistemaRed red;
    if (!red.cargarDatos(carpeta)) {
        cout << "No se pudieron cargar los datos.\n";
        return 1;
    }

    red.ejecutar();

    if (!red.guardarDatos(carpeta))
        cout << "Advertencia: no se pudieron guardar los datos.\n";
    return 0;
}
