#ifndef SOLICITUDAMISTAD_H
#define SOLICITUDAMISTAD_H


class SolicitudAmistad
{
private:
    int solicitante;
    int receptor;
    char* fechaEnvio;

    void copiarDe(const SolicitudAmistad& otra);
    void liberar();

public:
    SolicitudAmistad();
    SolicitudAmistad(int sol, int rec, const char* fecha);
    SolicitudAmistad(const SolicitudAmistad& otra);
    SolicitudAmistad& operator=(const SolicitudAmistad& otra);
    ~SolicitudAmistad();

    int getSolicitante() const { return solicitante; }
    int getReceptor() const { return receptor; }
    const char* getFechaEnvio() const { return fechaEnvio ? fechaEnvio : ""; }

    bool operator==(const SolicitudAmistad& otra) const;
};

#endif
