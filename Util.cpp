#include "Util.h"

int Util::longitud(const char* s) {
    if (!s) return 0;
    int n = 0;
    while (s[n]) n++;
    return n;
}

char* Util::copiar(const char* s) {
    if (!s) return nullptr;
    int n = longitud(s);
    char* d = new char[n + 1];
    for (int i = 0; i <= n; i++) d[i] = s[i];   // incluye el '\0'
    return d;
}

bool Util::iguales(const char* a, const char* b) {
    int i = 0;
    while (a[i] && a[i] == b[i]) i++;
    return a[i] == b[i];
}
int Util::aEntero(const char* s) {
    if (!s || !s[0]) return -1;
    long v = 0;
    for (int i = 0; s[i]; i++) {
        if (s[i] < '0' || s[i] > '9') return -1;
        v = v * 10 + (s[i] - '0');
        if (v > 1000000000L) return -1;
    }
    return (int)v;
}

unsigned long Util::aEnteroLargo(const char* s) {
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
