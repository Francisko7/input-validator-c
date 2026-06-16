/* =====================================================
 * main_demo.c — Demostración de input-validator-c
 * Muestra cada función con casos válidos e inválidos
 * ===================================================== */

#include <stdio.h>
#include "validator.h"

/* === CONSTANTES === */
#define TAM_BUFFER 32

int main(void)
{
    /* === VARIABLES === */
    char buffer[TAM_BUFFER];
    int  resultado;
    int  numero;

    printf("=== input-validator-c DEMO ===\n\n");

    /* --- TEST 1: leer_linea_segura --- */
    printf("Escribe tu nombre de usuario: ");
    leer_linea_segura(buffer, sizeof(buffer));
    printf("Leído: '%s'\n\n", buffer);

    /* --- TEST 2: es_alfanumerico --- */
    printf("¿'%s' es alfanumérico? %s\n\n",
           buffer,
           es_alfanumerico(buffer) ? "SÍ ✓" : "NO ✗");

    /* --- TEST 3: parsear_entero --- */
    printf("Escribe un número de puerto: ");
    leer_linea_segura(buffer, sizeof(buffer));

    resultado = parsear_entero(buffer, &numero);
    if (resultado == VALIDATOR_OK) {
        printf("Número parseado: %d\n", numero);

        /* --- TEST 4: es_puerto_valido --- */
        printf("¿Puerto %d válido? %s\n\n",
               numero,
               es_puerto_valido(numero) ? "SÍ ✓" : "NO ✗");
    } else {
        printf("Error: '%s' no es un número válido\n\n", buffer);
    }

    /* --- TEST 5: es_ipv4_valida --- */
    printf("Escribe una dirección IP: ");
    leer_linea_segura(buffer, sizeof(buffer));
    printf("¿'%s' es IPv4 válida? %s\n\n",
           buffer,
           es_ipv4_valida(buffer) ? "SÍ ✓" : "NO ✗");

    printf("=== FIN DEMO ===\n");

    return 0;
}
