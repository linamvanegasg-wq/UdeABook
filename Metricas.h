#ifndef METRICAS_H
#define METRICAS_H

namespace Metricas
{
void reiniciar();
void sumar(unsigned long long n = 1);
void sumarExterno(unsigned long long n = 1);
unsigned long long iteraciones();
unsigned long long externos();
}

#endif
