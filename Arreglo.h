#ifndef ARREGLO_H
#define ARREGLO_H

template <typename T>
class Arreglo
{
private:
    T* datos;
    int tam;
    int cap;

    void redimensionar(int nuevaCap)
    {
        T* nuevo = new T[nuevaCap];
        for (int i = 0; i < tam; i++) nuevo[i] = datos[i];
        delete[] datos;
        datos = nuevo;
        cap = nuevaCap;
    }

public:
    Arreglo() : datos(nullptr), tam(0), cap(0)
    {}

    Arreglo(const Arreglo& otro) : datos(nullptr), tam(0), cap(0)
    {
        if (otro.tam > 0)
        {
            datos = new T[otro.tam];
            cap = otro.tam;
            for (int i = 0; i < otro.tam; i++) datos[i] = otro.datos[i];
            tam = otro.tam;
        }
    }

    ~Arreglo()
    {
        delete[] datos;
    }

    Arreglo& operator=(const Arreglo& otro)
    {
        if (this != &otro)
        {
            T* nuevo = nullptr;
            if (otro.tam > 0)
            {
                nuevo = new T[otro.tam];
                for (int i = 0; i < otro.tam; i++) nuevo[i] = otro.datos[i];
            }
            delete[] datos;
            datos = nuevo;
            tam = otro.tam;
            cap = otro.tam;
        }
        return *this;
    }

    void agregar(const T& valor)
    {
        if (tam == cap) redimensionar(cap == 0 ? 4 : cap * 2);
        datos[tam++] = valor;
    }

    void eliminar(int i)
    {
        if (i < 0 || i >= tam) return;
        for (int k = i; k < tam - 1; k++) datos[k] = datos[k + 1];
        tam--;
    }

    T& operator[](int i)
    {
        return datos[i];
    }
    const T& operator[](int i) const
    {
        return datos[i];
    }
    int tamano() const
    {
        return tam;
    }
};

#endif
