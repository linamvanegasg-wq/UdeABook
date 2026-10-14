#include "SolicitudAmistad.h"
#include "Util.h"

SolicitudAmistad::SolicitudAmistad() : solicitante(-1), receptor(-1), fechaEnvio(nullptr) {}

SolicitudAmistad::SolicitudAmistad(int sol, int rec, const char* fecha)
    : solicitante(sol), receptor(rec), fechaEnvio(Util::copiar(fecha)) {}

SolicitudAmistad::SolicitudAmistad(const SolicitudAmistad& otra) { copiarDe(otra); }

SolicitudAmistad& SolicitudAmistad::operator=(const SolicitudAmistad& otra) {
    if (this != &otra) {
        liberar();
        copiarDe(otra);
    }
    return *this;
}

SolicitudAmistad::~SolicitudAmistad() { liberar(); }

void SolicitudAmistad::copiarDe(const SolicitudAmistad& otra) {
    solicitante = otra.solicitante;
    receptor = otra.receptor;
    fechaEnvio = Util::copiar(otra.fechaEnvio);
}

void SolicitudAmistad::liberar() {
    delete[] fechaEnvio;
    fechaEnvio = nullptr;
}

bool SolicitudAmistad::operator==(const SolicitudAmistad& otra) const {
    return solicitante == otra.solicitante && receptor == otra.receptor;
}
