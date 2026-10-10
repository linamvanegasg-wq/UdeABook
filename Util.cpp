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

