#include <iostream>
#include "Arreglo.h"
#include "Util.h"
#include "Usuario.h"

using namespace std;

int main()
{
    Arreglo<int> a;
    for (int i = 0; i < 10; i++) a.agregar(i * 10);
    a.eliminar(2);

    Arreglo<int> b = a;
    b[0] = 999;

    cout << "a: ";
    for (int i = 0; i < a.tamano(); i++) std::cout << a[i] << " ";
    cout << "\nb: ";
    for (int i = 0; i < b.tamano(); i++) std::cout << b[i] << " ";
    cout << "\n";

    Usuario u1("lina01", "2005-03-14", "lina@udea.edu.co", "/users/udea/lina01/albums/profile.png","clave123", "Medellin", "Colombia", "2026-01-10");
    Usuario u2 = u1;
    Usuario u3;
    u3 = u1;

    cout << u2.getNombreUsuario() << " | " << u3.getCorreo() << "\n";
    cout << "clave ok: " << u1.claveCorrecta("clave123") << "\n";
    cout << "clave mala: " << u1.claveCorrecta("otra") << "\n";
    cout << "iguales: " << (u1 == u2) << "\n";
    return 0;
}
