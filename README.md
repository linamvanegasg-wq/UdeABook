# Ude@Book — Desafío II (Informática II, 2026-2)

## Compilar y ejecutar
```
make          # genera ./udeabook
./udeabook datos
```
En **Qt Creator**: Archivo > Abrir archivo o proyecto > `UdeABook.pro` > Configurar proyecto > Ejecutar.
(En Ejecutar, activa "Ejecutar en terminal" para poder escribir en la consola. El `.pro` ya apunta a la carpeta `datos/`.)

Usuario de prueba: `usuario0001` / `clave0001` (hay 1000 usuarios: `usuarioNNNN` / `claveNNNN`).
Regenerar datos: `python3 herramientas/generar_datos.py 1000 datos` (solo para pruebas).

## Archivos del código
| Archivo | Contenido |
|---|---|
| `Arreglo.h` | Plantilla de arreglo dinámico propio (`new[]`/`delete[]`, duplica capacidad) |
| `Metricas.h/.cpp` | Contador de iteraciones y llamadas externas |
| `Util.h/.cpp` | Cadenas propias (sin `<cstring>`), fechas, aleatorios |
| `Foto`, `Album`, `Mensaje`, `SolicitudAmistad`, `Usuario` | Clases del diagrama (copia profunda, `operator==`, `operator<`, `operator<<` amigo) |
| `SistemaRed.h/.cpp` | Controlador: carga/guardado, matriz de adyacencia en bits, menús, métricas |
| `main.cpp` | Punto de entrada |

## Formato de los archivos de datos (separador `|`, un registro por línea)
El **id** de un usuario es su número de línea en `usuarios.txt` (empezando en 0).

| Archivo | Campos |
|---|---|
| `usuarios.txt` | nombre\|nacimiento\|correo\|rutaFoto\|clave\|ciudad\|pais\|inscripcion |
| `amistades.txt` | idA\|idB (una línea por amistad) |
| `solicitudes.txt` | idSolicitante\|idReceptor\|fecha (solo pendientes) |
| `mensajes.txt` | idEmisor\|idReceptor\|AAAA-MM-DD HH:MM\|leido(0/1)\|texto — **ordenado por fecha** |
| `albumes.txt` | idUsuario\|nombre\|ruta\|privacidad (0 público, 1 amigos, 2 privado) |
| `fotos.txt` | idUsuario\|indiceAlbum\|fechaCaptura\|tamaño\|ruta\|likes (indiceAlbum = orden del álbum de ese usuario en `albumes.txt`) |

## Decisiones de diseño (para el informe y la sustentación)
- **Amistades:** matriz de adyacencia en bits (`unsigned long long*`). Amigos en común = bits de `fila A AND fila B`.
- **Sugerencias:** por cada amigo se recorren sus amigos y se acumula un contador por candidato; luego se toman los K mayores.
- **Mensajes:** un arreglo global ordenado por fecha + por usuario dos listas de índices (`recibidos`, `noLeidos`). Rango de fechas = búsqueda binaria + recorrido.
- **Fotos de amigos al ingresar:** muestreo ponderado sin repetición (pesos 1 / 2.5 / 3.5 según amigos en común); el cambio de grupo se permite una sola vez.
- **Memoria:** cada clase calcula su `memoriaDinamica()`; `SistemaRed::memoriaEstructuras()` los suma y se añaden las variables locales de cada funcionalidad. No incluye el overhead interno de `malloc`.
- **Mejoras posibles:** buscar usuario por nombre es lineal (se podría usar una tabla hash propia).

## Pendiente por tu parte
Informe final, commits regulares en el repositorio público, video y entender cada línea para la sustentación.
