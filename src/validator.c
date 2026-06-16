/* =====================================================
 * validator.c — Implementación de validaciones seguras
 * Mitiga: CWE-120 (buffer overflow)
 *         CWE-20  (improper input validation)
 * ===================================================== */

#include <stdio.h>
#include <stdlib.h>    /* strtol()        */
#include <ctype.h>     /* isalnum()       */
#include <errno.h>     /* errno, ERANGE   */
#include <limits.h>    /* INT_MAX, INT_MIN */
#include "validator.h"

/* === leer_linea_segura === */
int leer_linea_segura(char *buffer, size_t tam)
{
    size_t i = 0;
    int c;

    if (buffer == NULL || tam == 0) {
        return VALIDATOR_ERROR;
    }

    while (i < tam - 1) {
        c = getchar();
        if (c == EOF || c == '\n') {
            break;
        }
        buffer[i] = (char)c;
        i++;
    }

    buffer[i] = '\0';

    if (i == tam - 1) {
        while (getchar() != '\n');
    }

    return (int)i;
}

/* === es_alfanumerico === */
int es_alfanumerico(const char *str)
{
    int i = 0;

    if (str == NULL || str[0] == '\0') {
        return 0;
    }

    while (str[i] != '\0') {
        if (!isalnum((unsigned char)str[i])) {
            return 0;
        }
        i++;
    }

    return 1;
}

/* === parsear_entero === */
int parsear_entero(const char *str, int *out)
{
    char *endptr;
    long  valor;

    if (str == NULL || out == NULL) {
        return VALIDATOR_ERROR;
    }

    errno = 0;
    valor = strtol(str, &endptr, 10);

    if (errno != 0 || endptr == str || *endptr != '\0') {
        return VALIDATOR_ERROR;
    }

    if (valor > INT_MAX || valor < INT_MIN) {
        return VALIDATOR_ERROR;
    }

    *out = (int)valor;
    return VALIDATOR_OK;
}

/* === es_ipv4_valida === */
int es_ipv4_valida(const char *str)
{
    int a, b, c, d;
    char extra;

    if (str == NULL) {
        return 0;
    }

    if (sscanf(str, "%d.%d.%d.%d%c", &a, &b, &c, &d, &extra) != 4) {
        return 0;
    }

    if (a < 0 || a > 255) return 0;
    if (b < 0 || b > 255) return 0;
    if (c < 0 || c > 255) return 0;
    if (d < 0 || d > 255) return 0;

    return 1;
}

/* === es_puerto_valido === */
int es_puerto_valido(int puerto)
{
    return (puerto >= PUERTO_MIN && puerto <= PUERTO_MAX);
}
