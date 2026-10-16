#ifndef UTIL_H
#define UTIL_H

namespace Util
{
int   aEntero(const char* s);
unsigned long aEnteroLargo(const char* s);
int   partir(char* linea, char sep, char** campos, int maxCampos);
int   longitud(const char* s);
char* copiar(const char* s);
bool  iguales(const char* a, const char* b);
}

template <typename T>
void agregarAlFinal(T*& arreglo, int& cantidad, const T& valor)
{
    T* nuevo = new T[cantidad + 1];
    for (int i = 0; i < cantidad; i++) nuevo[i] = arreglo[i];
    nuevo[cantidad] = valor;
    delete[] arreglo;
    arreglo = nuevo;
    cantidad++;
}

template <typename T>
void eliminarEn(T*& arreglo, int& cantidad, int pos)
{
    if (pos < 0 || pos >= cantidad) return;
    T* nuevo = (cantidad > 1) ? new T[cantidad - 1] : nullptr;
    for (int i = 0, k = 0; i < cantidad; i++)
        if (i != pos) nuevo[k++] = arreglo[i];
    delete[] arreglo;
    arreglo = nuevo;
    cantidad--;
}

template <typename T>
T* copiarArreglo(const T* origen, int cantidad)
{
    if (cantidad <= 0) return nullptr;
    T* nuevo = new T[cantidad];
    for (int i = 0; i < cantidad; i++) nuevo[i] = origen[i];
    return nuevo;
}

#endif
