#include <iostream>
#include "Arreglo.h"
#include "Util.h"
#include "Usuario.h"
#include "Album.h"
#include "Mensaje.h"
#include"SolicitudAmistad.h"

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

    Usuario u1("lina05", "1996-12-05", "lina@udea.edu.co", "/users/udea/lina01/albums/profile.png","clave123", "San Vicente", "Colombia", "2026-01-10");
    Usuario u2 = u1;
    Usuario u3;
    u3 = u1;

    cout << u2.getNombreUsuario() << " | " << u3.getCorreo() << "\n";
    cout << "clave ok: " << u1.claveCorrecta("clave123") << "\n";
    cout << "clave mala: " << u1.claveCorrecta("otra") << "\n";
    cout << "iguales: " << (u1 == u2) << "\n";

    Album alb("Viaje2026", "/users/udea/lina01/albums/Viaje2026", Album::AMIGOS);
    alb.agregarFoto(Foto("2026-02-01", 350000, "/users/udea/lina01/albums/Viaje2026/Foto01.png", 3));
    alb.agregarFoto(Foto("2026-02-02", 410000, "/users/udea/lina01/albums/Viaje2026/Foto02.png", 0));

    Album copia = alb;
    copia.getFoto(0).darQuitarLike(true);

    cout << alb.getFoto(0) << "\n";
    cout << copia.getFoto(0) << "\n";

    Mensaje m1(0, 1, "Hola, que haces?", "2026-10-12 09:30", false);
    Mensaje m2(1, 0, "Haciendo un informe", "2026-10-12 10:05", false);
    Mensaje m3 = m1;
    m3.marcarLeido();

    cout << m1 << " leido=" << m1.estaLeido() << "\n";
    cout << m3 << " leido=" << m3.estaLeido() << "\n";
    cout << "m1 < m2: " << (m1 < m2) << "\n";

    SolicitudAmistad s1(0, 5, "2026-10-12 11:00");
    SolicitudAmistad s2(0, 5, "2026-10-13 08:00");
    SolicitudAmistad s3(5, 0, "2026-10-13 08:00");
    cout << "s1==s2: " << (s1 == s2) << "\n";
    cout << "s1==s3: " << (s1 == s3) << "\n";
    return 0;
}
