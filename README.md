# 🛡️ input-validator-c

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Language: C](https://img.shields.io/badge/Language-C11-blue.svg)](https://en.wikipedia.org/wiki/C11_(C_standard_revision))
[![Security: CWE-20](https://img.shields.io/badge/Mitigates-CWE--20-red.svg)](https://cwe.mitre.org/data/definitions/20.html)
[![Security: CWE-120](https://img.shields.io/badge/Mitigates-CWE--120-red.svg)](https://cwe.mitre.org/data/definitions/120.html)
[![Platform: IoT](https://img.shields.io/badge/Platform-IoT%20%2F%205G-green.svg)]()
[![Build: GCC](https://img.shields.io/badge/Build-GCC%2015-orange.svg)]()

> Librería de validación de entradas en C para prevenir vulnerabilidades
> en sistemas embebidos, IoT y comunicaciones 5G.
> **Lema: practicar y practicar, así se aprende.**

---

## 🔥 El Problema Real

Imagina estos escenarios del mundo real:

- 🏥 **Sensor médico** que recibe datos por red: un atacante envía
  una cadena de 500 caracteres donde se esperan 20. El buffer
  se desborda y el dispositivo se reinicia durante una cirugía.

- 🏭 **PLC industrial** conectado por 5G: alguien inyecta un valor
  de temperatura `-99999` donde el rango válido es `0-150°C`.
  La lógica de control falla y una caldera explota.

- 🚗 **ECU de un vehículo** conectado: recibe una IP malformada
  como dirección de servidor de actualizaciones. Se conecta a un
  servidor malicioso y recibe firmware comprometido.

Estos no son escenarios ficticios. Son vulnerabilidades reales
documentadas en los CVE (Common Vulnerabilities and Exposures).

**Esta librería existe para que eso no ocurra.**

---

## ✅ Características

- 🔒 **Lectura segura de líneas** — sin buffer overflow (mitiga CWE-120)
- 🔤 **Validación alfanumérica** — sanitización de cadenas de entrada
- 🔢 **Parseo seguro de enteros** — con validación de rango y overflow
- 🌐 **Validación de IPv4** — formato y rango de octetos
- 🔌 **Validación de puertos** — rango 1-65535 con exclusión de
  puertos reservados
- ⚡ **Sin dependencias externas** — solo librería estándar de C
- 🧪 **Tests incluidos** — script de pruebas automatizadas
- 📚 **Documentada** — cada función explicada con ejemplos

---

## 🗂️ Estructura del Proyecto

```
input-validator-c/
├── src/
│   ├── vulnerable.c     ← Código INSEGURO (para comparar y aprender)
│   ├── safe.c           ← Versión segura del mismo código
│   ├── validator.c      ← Implementación de la librería
│   └── main_demo.c      ← Demo de uso completa
├── include/
│   └── validator.h      ← API pública de la librería
├── tests/
│   └── test_validator.sh ← Tests automatizados con bash
├── docs/
│   └── explicacion.md   ← Teoría: CWE-20, CWE-120 explicados
├── .gitignore
├── LICENSE              ← MIT
└── README.md            ← Este archivo
```

> 💡 **Nota pedagógica:** El fichero `vulnerable.c` existe
> deliberadamente para mostrar el contraste entre código inseguro
> y seguro. Nunca uses ese código en producción.

---

## ⚙️ Requisitos

| Herramienta | Versión mínima | Cómo verificar |
|---|---|---|
| GCC | 9.0+ | `gcc --version` |
| GNU Make | 4.0+ | `make --version` |
| GDB (opcional) | 10.0+ | `gdb --version` |
| Valgrind (opcional) | 3.15+ | `valgrind --version` |

**Sistema operativo:** Linux (probado en Kali Linux 2024+)

```bash
# Instalar dependencias en Debian/Kali/Ubuntu
sudo apt install gcc make gdb valgrind
```

---

## 🔨 Compilación

### Compilación rápida (demo)

```bash
git clone git@github.com:Francisko7/input-validator-c.git
cd input-validator-c

gcc -Wall -Wextra -Wpedantic -std=c11 \
    -fstack-protector-strong -D_FORTIFY_SOURCE=2 -O2 \
    src/validator.c src/main_demo.c -o demo

./demo
```

### Flags de compilación explicados

| Flag | Propósito |
|---|---|
| `-Wall -Wextra -Wpedantic` | Activa todos los warnings. Un warning es un bug esperando ocurrir. |
| `-std=c11` | Estándar C11: moderno y portable |
| `-fstack-protector-strong` | Añade canarios en la pila para detectar buffer overflows en tiempo de ejecución |
| `-D_FORTIFY_SOURCE=2` | El compilador añade checks extra en funciones como `strcpy`, `sprintf` |
| `-O2` | Optimización sin sacrificar seguridad |

> 🛡️ **Regla de oro:** Compilar siempre con estas flags en proyectos
> de seguridad. Nunca compilar con `-O0` sin `-fstack-protector` en
> código que llegue a producción.

---

## 🚀 Uso

### Incluir la librería

```c
#include "validator.h"
```

### Funciones disponibles

#### `leer_linea_segura(buffer, tam)`

```c
char nombre[32];

// ❌ INSEGURO — puede desbordar el buffer
scanf("%s", nombre);

// ✅ SEGURO — con esta librería
int resultado = leer_linea_segura(nombre, sizeof(nombre));
if (resultado < 0) {
    fprintf(stderr, "Error: entrada demasiado larga\n");
}
```

#### `es_alfanumerico(str)`

```c
char usuario[32];
leer_linea_segura(usuario, sizeof(usuario));

if (!es_alfanumerico(usuario)) {
    // Rechazar: posible inyección de código
    fprintf(stderr, "Error: nombre de usuario inválido\n");
}
```

#### `parsear_entero(str, *out)`

```c
int temperatura;
char entrada[16];

leer_linea_segura(entrada, sizeof(entrada));

if (parsear_entero(entrada, &temperatura) != 0) {
    fprintf(stderr, "Error: valor de temperatura inválido\n");
}
```

#### `es_ipv4_valida(str)`

```c
char ip[16];
leer_linea_segura(ip, sizeof(ip));

if (!es_ipv4_valida(ip)) {
    fprintf(stderr, "Error: dirección IP malformada\n");
    // En IoT: no conectar. Puede ser un servidor malicioso.
}
```

#### `es_puerto_valido(puerto)`

```c
int puerto = 8080;

if (!es_puerto_valido(puerto)) {
    fprintf(stderr, "Error: puerto fuera de rango\n");
}
```

---

## 🔐 Conceptos de Seguridad Cubiertos

### CWE-120 — Buffer Overflow clásico

**¿Qué es?** Escribir más datos en un buffer de los que cabe.

```
Buffer en memoria: [  32 bytes  ]
Entrada recibida:  [  500 bytes  ← DESBORDAMIENTO ]
                                   ↑ machaca la dirección
                                     de retorno de la función
                                   ↑ el atacante controla el PC
```

**Consecuencia en IoT:** Un sensor industrial puede ejecutar
código arbitrario del atacante, apagar sistemas de seguridad
o exfiltrar datos de la red 5G privada de la fábrica.

**Cómo lo mitiga esta librería:** `leer_linea_segura()` nunca
escribe más bytes que `tam-1`, garantizando el terminador nulo.

---

### CWE-20 — Improper Input Validation

**¿Qué es?** Usar datos del exterior sin verificar que son válidos.

```
Entrada esperada: temperatura entre 0 y 150 °C
Entrada recibida: -99999
Resultado:        underflow en cálculo → decisión errónea
```

**Consecuencia en IoT/5G:** En redes 5G industriales (Network
Slicing), un mensaje malformado puede corromper el slice de red
de un hospital completo si no se valida en el edge node.

**Cómo lo mitiga esta librería:** `parsear_entero()` valida
formato, rango y overflow antes de devolver el valor.

---

## 🗺️ Roadmap

- [x] 📁 Estructura del proyecto
- [x] 📖 README profesional
- [ ] 🔧 Implementar `validator.c` y `validator.h`
- [ ] ⚡ Demo `vulnerable.c` vs `safe.c`
- [ ] 🧪 Tests automatizados con `test_validator.sh`
- [ ] 📝 Documentación técnica en `docs/explicacion.md`
- [ ] 🔗 Makefile completo con targets `all`, `test`, `clean`
- [ ] 🧰 Integración con Valgrind para detección de memory leaks
- [ ] 📦 Versión estática compilable como `.a` (librería estática)
- [ ] 🌐 Ejemplos de uso en escenarios IoT/5G reales

---

## 🤝 Contribuir

Las contribuciones son bienvenidas, especialmente:

- Nuevas funciones de validación (UUID, MAC address, JSON básico)
- Casos de test adicionales
- Ejemplos de uso en plataformas IoT (Raspberry Pi, ESP32, STM32)
- Mejoras en la documentación

**Pasos para contribuir:**

```bash
# 1. Hacer fork del repositorio en GitHub

# 2. Clonar tu fork
git clone git@github.com:TU_USUARIO/input-validator-c.git

# 3. Crear una rama descriptiva
git checkout -b feat/validar-uuid

# 4. Commit con Conventional Commits
git commit -m "feat: añadir validación de formato UUID v4"

# 5. Push y abrir Pull Request
git push origin feat/validar-uuid
```

**Formato de commits (Conventional Commits):**

| Prefijo | Cuándo usarlo |
|---|---|
| `feat:` | Nueva funcionalidad |
| `fix:` | Corrección de bug |
| `docs:` | Solo documentación |
| `test:` | Añadir o modificar tests |
| `refactor:` | Refactorización sin cambio de funcionalidad |
| `security:` | Corrección de vulnerabilidad de seguridad |

---

## 📄 Licencia

Distribuido bajo licencia **MIT**.
Consulta el fichero [LICENSE](LICENSE) para más información.

> En resumen: puedes usar, modificar y distribuir este código
> libremente, incluso en proyectos comerciales, manteniendo
> el aviso de copyright.

---

## 👤 Autor

**Francisko7**

- GitHub: [@Francisko7](https://github.com/Francisko7)
- Proyecto creado como parte del aprendizaje de
  programación segura en C para IoT y 5G.

---

## 📚 Referencias y Recursos

- [CWE-120: Buffer Copy without Checking Size of Input](https://cwe.mitre.org/data/definitions/120.html)
- [CWE-20: Improper Input Validation](https://cwe.mitre.org/data/definitions/20.html)
- [OWASP IoT Security Testing Guide](https://owasp.org/www-project-iot-security-testing-guide/)
- [SEI CERT C Coding Standard](https://wiki.sei.cmu.edu/confluence/display/c/SEI+CERT+C+Coding+Standard)
- [ETSI EN 303 645 — Cybersecurity for IoT](https://www.etsi.org/deliver/etsi_en/303600_303699/303645/02.01.01_60/en_303645v020101p.pdf)

---

*"practicar y practicar, así se aprende"* 🚀
