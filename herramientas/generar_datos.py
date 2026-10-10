#!/usr/bin/env python3
"""Genera datos de prueba para Ude@Book (solo para pruebas; no es parte de la solucion en C++).

Uso: python3 generar_datos.py [num_usuarios] [carpeta_salida]
Usuarios: usuario0001, usuario0002, ...   Clave: clave0001, clave0002, ...
"""
import random
import sys
import os

random.seed(2026)
N = int(sys.argv[1]) if len(sys.argv) > 1 else 1000
OUT = sys.argv[2] if len(sys.argv) > 2 else "datos"
os.makedirs(OUT, exist_ok=True)

BASE = "/users/udea"
CIUDADES = ["Medellin", "Bogota", "Cali", "Barranquilla", "Cartagena", "Bucaramanga", "Pereira", "Rionegro"]
ALBUMES = ["Viaje2026", "Familia", "Amigos", "Universidad", "Fiestas", "Mascotas"]
FRASES = ["Hola, como vas?", "Nos vemos en la U manana", "Ya entregaste el desafio?",
          "Vamos por un cafe", "Feliz cumpleanos!", "Te paso los apuntes", "Que tal el parcial?",
          "Mira esta foto del viaje", "Llego en 10 minutos", "Gracias por la ayuda"]


def nombre(i):
    return "usuario%04d" % (i + 1)


# ---------------- usuarios ----------------
with open(f"{OUT}/usuarios.txt", "w") as f:
    for i in range(N):
        nac = "%d-%02d-%02d" % (random.randint(1995, 2007), random.randint(1, 12), random.randint(1, 28))
        ins = "%d-%02d-%02d" % (random.randint(2024, 2026), random.randint(1, 8), random.randint(1, 28))
        f.write("|".join([nombre(i), nac, nombre(i) + "@udea.edu.co",
                          f"{BASE}/{nombre(i)}/albums/profile.png", "clave%04d" % (i + 1),
                          random.choice(CIUDADES), "Colombia", ins]) + "\n")

# ---------------- amistades (comunidades de 40 -> muchos amigos en comun) ----------------
pares = set()
for i in range(N):
    cl = i // 40
    miembros = [j for j in range(cl * 40, min(N, cl * 40 + 40)) if j != i]
    for j in random.sample(miembros, min(len(miembros), random.randint(12, 24))):
        pares.add((min(i, j), max(i, j)))
    for _ in range(random.randint(1, 4)):
        j = random.randrange(N)
        if j != i:
            pares.add((min(i, j), max(i, j)))
with open(f"{OUT}/amistades.txt", "w") as f:
    for a, b in sorted(pares):
        f.write(f"{a}|{b}\n")
amigos = [[] for _ in range(N)]
for a, b in pares:
    amigos[a].append(b)
    amigos[b].append(a)

# ---------------- albumes y fotos ----------------
with open(f"{OUT}/albumes.txt", "w") as fa, open(f"{OUT}/fotos.txt", "w") as ff:
    for u in range(N):
        na = random.randint(2, 4)
        for a, alb in enumerate(random.sample(ALBUMES, na)):
            priv = random.choice([0, 1, 1, 2]) if u != 0 else (0 if a == 0 else 1)
            fa.write(f"{u}|{alb}|{BASE}/{nombre(u)}/albums/{alb}|{priv}\n")
            for p in range(random.randint(5, 8)):
                fecha = "2026-%02d-%02d" % (random.randint(1, 9), random.randint(1, 28))
                ff.write(f"{u}|{a}|{fecha}|{random.randint(200000, 4000000)}|"
                         f"{BASE}/{nombre(u)}/albums/{alb}/Foto{p + 1:02d}.png|{random.randint(0, 50)}\n")

# ---------------- solicitudes pendientes ----------------
sol = set()
while len(sol) < 300:
    a, b = random.randrange(N), random.randrange(N)
    if a != b and (min(a, b), max(a, b)) not in pares and (b, a) not in sol:
        sol.add((a, b))
for extra in (500, 600, 700):                       # solicitudes recibidas por usuario0001
    if (extra, 0) not in sol and (min(0, extra), max(0, extra)) not in pares:
        sol.add((extra, 0))
with open(f"{OUT}/solicitudes.txt", "w") as f:
    for a, b in sorted(sol):
        f.write(f"{a}|{b}|2026-10-0{random.randint(1, 9)} 10:00\n")

# ---------------- mensajes (ordenados por fecha, ~12 semanas) ----------------
msgs = []


def fecha_aleatoria():
    mes, dia = random.choice([(7, d) for d in range(20, 32)] + [(8, d) for d in range(1, 32)] +
                             [(9, d) for d in range(1, 31)] + [(10, d) for d in range(1, 10)])
    return "2026-%02d-%02d %02d:%02d" % (mes, dia, random.randint(0, 23), random.randint(0, 59))


for _ in range(N * 15):
    e = random.randrange(N)
    if amigos[e]:
        r = random.choice(amigos[e])
        msgs.append((fecha_aleatoria(), e, r, random.random() < 0.8, random.choice(FRASES)))
for _ in range(60):                                  # mensajes para usuario0001 (varias semanas)
    if amigos[0]:
        e = random.choice(amigos[0])
        msgs.append((fecha_aleatoria(), e, 0, random.random() < 0.6, random.choice(FRASES)))
msgs.sort(key=lambda m: m[0])
with open(f"{OUT}/mensajes.txt", "w") as f:
    for fecha, e, r, leido, txt in msgs:
        f.write(f"{e}|{r}|{fecha}|{1 if leido else 0}|{txt}\n")

print(f"Generados {N} usuarios, {len(pares)} amistades, {len(msgs)} mensajes en '{OUT}/'")
