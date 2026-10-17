#include "Util.h"
#include "Metricas.h"
#include <cstdlib>
#include <ctime>

int Util::longitud(const char* s)
{
    if (!s) return 0;
    int n = 0;
    while (s[n]) n++;
    return n;
}

char* Util::copiar(const char* s) {
    if (!s) return nullptr;
    int n = longitud(s);
    char* d = new char[n + 1];
    for (int i = 0; i <= n; i++) d[i] = s[i];
    return d;
}

bool Util::iguales(const char* a, const char* b)
{
    int i = 0;
    while (a[i] && a[i] == b[i]) i++;
    return a[i] == b[i];
}

int Util::aEntero(const char* s)
{
    if (!s || !s[0]) return -1;
    long v = 0;
    for (int i = 0; s[i]; i++) {
        if (s[i] < '0' || s[i] > '9') return -1;
        v = v * 10 + (s[i] - '0');
        if (v > 1000000000L) return -1;
    }
    return (int)v;
}

unsigned long Util::aEnteroLargo(const char* s)
{
    unsigned long v = 0;
    for (int i = 0; s && s[i]; i++) {
        if (s[i] < '0' || s[i] > '9') break;
        v = v * 10 + (unsigned long)(s[i] - '0');
    }
    return v;
}

int Util::partir(char* linea, char sep, char** campos, int maxCampos)
{
    if (maxCampos <= 0) return 0;
    int n = 0;
    campos[n++] = linea;
    for (char* p = linea; *p; ++p) {
        if (*p == sep && n < maxCampos) {
            *p = '\0';
            campos[n++] = p + 1;
        }
    }
    return n;
}

int Util::comparar(const char* a, const char* b, int n)
{
    int i = 0;
    while ((n < 0 || i < n) && a[i] && a[i] == b[i]) i++;
    Metricas::sumar(i + 1);
    if (n >= 0 && i == n) return 0;
    return (int)(unsigned char)a[i] - (int)(unsigned char)b[i];
}

bool Util::fechaValida(const char* f)
{
    if (longitud(f) != 10) return false;
    for (int i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7)
        {
            if (f[i] != '-') return false;
        } else if (f[i] < '0' || f[i] > '9')
        {
            return false;
        }
    }
    int mes = (f[5] - '0') * 10 + (f[6] - '0');
    int dia = (f[8] - '0') * 10 + (f[9] - '0');
    return mes >= 1 && mes <= 12 && dia >= 1 && dia <= 31;
}

void Util::fechaActual(char* d)
{
    Metricas::sumarExterno(2);
    std::time_t t = std::time(nullptr);
    std::tm* p = std::localtime(&t);
    int a = p->tm_year + 1900, m = p->tm_mon + 1, dd = p->tm_mday;
    int h = p->tm_hour, mi = p->tm_min;
    d[0] = (char)('0' + (a / 1000) % 10);
    d[1] = (char)('0' + (a / 100) % 10);
    d[2] = (char)('0' + (a / 10) % 10);
    d[3] = (char)('0' + a % 10);
    d[4] = '-';
    d[5] = (char)('0' + m / 10);
    d[6] = (char)('0' + m % 10);
    d[7] = '-';
    d[8] = (char)('0' + dd / 10);
    d[9] = (char)('0' + dd % 10);
    d[10] = ' ';
    d[11] = (char)('0' + h / 10);
    d[12] = (char)('0' + h % 10);
    d[13] = ':';
    d[14] = (char)('0' + mi / 10);
    d[15] = (char)('0' + mi % 10);
    d[16] = '\0';
}

void Util::iniciarAleatorio()
{
    Metricas::sumarExterno(2);
    std::srand((unsigned int)std::time(nullptr));
}

double Util::aleatorioReal()
{
    Metricas::sumarExterno();
    return std::rand() / (RAND_MAX + 1.0);
}
