#include "Mensaje.h"
#include "Util.h"

Mensaje::Mensaje() : emisor(-1), receptor(-1), texto(nullptr), fechaEnvio(nullptr), leido(false) {}

Mensaje::Mensaje(int emi, int rec, const char* txt, const char* fecha, bool yaLeido)
    : emisor(emi), receptor(rec), texto(Util::copiar(txt)),
    fechaEnvio(Util::copiar(fecha)), leido(yaLeido) {}

Mensaje::Mensaje(const Mensaje& otro) { copiarDe(otro); }

Mensaje& Mensaje::operator=(const Mensaje& otro) {
    if (this != &otro) {
        liberar();
        copiarDe(otro);
    }
    return *this;
}

Mensaje::~Mensaje() { liberar(); }

void Mensaje::copiarDe(const Mensaje& otro) {
    emisor = otro.emisor;
    receptor = otro.receptor;
    texto = Util::copiar(otro.texto);
    fechaEnvio = Util::copiar(otro.fechaEnvio);
    leido = otro.leido;
}

void Mensaje::liberar() {
    delete[] texto;
    delete[] fechaEnvio;
    texto = nullptr;
    fechaEnvio = nullptr;
}

bool Mensaje::operator<(const Mensaje& otro) const {

    const char* a = getFechaEnvio();
    const char* b = otro.getFechaEnvio();
    int i = 0;
    while (a[i] && a[i] == b[i]) i++;
    return a[i] < b[i];
}

std::ostream& operator<<(std::ostream& os, const Mensaje& m) {
    os << "[" << m.getFechaEnvio() << "] " << m.getTexto();
    return os;
}
