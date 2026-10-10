TEMPLATE = app
TARGET = udeabook

# Aplicacion de consola en C++ puro (sin Qt, sin STL en el core)
CONFIG += console c++11
CONFIG -= app_bundle qt

# Carpeta de datos por defecto: la carpeta "datos" junto a este .pro
DEFINES += RUTA_DATOS=\\\"$$PWD/datos\\\"

SOURCES += \
    main.cpp \
    Album.cpp \
    Foto.cpp \
    Mensaje.cpp \
    Metricas.cpp \
    SistemaRed.cpp \
    SolicitudAmistad.cpp \
    Usuario.cpp \
    Util.cpp

HEADERS += \
    Album.h \
    Arreglo.h \
    Foto.h \
    Mensaje.h \
    Metricas.h \
    SistemaRed.h \
    SolicitudAmistad.h \
    Usuario.h \
    Util.h
