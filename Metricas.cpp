#include "Metricas.h"

namespace
{
unsigned long long contIteraciones = 0;
unsigned long long contExternos = 0;
}

void Metricas::reiniciar() { contIteraciones = 0; contExternos = 0; }
void Metricas::sumar(unsigned long long n) { contIteraciones += n; }
void Metricas::sumarExterno(unsigned long long n) { contExternos += n; }
unsigned long long Metricas::iteraciones() { return contIteraciones; }
unsigned long long Metricas::externos() { return contExternos; }
