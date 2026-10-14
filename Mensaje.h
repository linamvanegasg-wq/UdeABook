#ifndef MENSAJE_H
#define MENSAJE_H

#include <iostream>

class Mensaje
{
private:
    int emisor;
    int receptor;
    char* texto;
    char* fechaEnvio;
    bool leido;

    void copiarDe(const Mensaje& otro);
    void liberar();

public:
    Mensaje();
    Mensaje(int emi, int rec, const char* txt, const char* fecha, bool yaLeido);
    Mensaje(const Mensaje& otro);
    Mensaje& operator=(const Mensaje& otro);
    ~Mensaje();

    void marcarLeido() { leido = true; }

    int getEmisor() const { return emisor; }
    int getReceptor() const { return receptor; }
    const char* getTexto() const { return texto ? texto : ""; }
    const char* getFechaEnvio() const { return fechaEnvio ? fechaEnvio : ""; }
    bool estaLeido() const { return leido; }

    bool operator<(const Mensaje& otro) const;   // compara por fecha

    friend std::ostream& operator<<(std::ostream& os, const Mensaje& m);
};

#endif
