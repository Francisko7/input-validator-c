#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <stddef.h>

/* === CONSTANTES === */
#define VALIDATOR_OK       0
#define VALIDATOR_ERROR   -1
#define PUERTO_MIN         1
#define PUERTO_MAX     65535

/* === PROTOTIPOS === */
int leer_linea_segura(char *buffer, size_t tam);
int es_alfanumerico(const char *str);
int parsear_entero(const char *str, int *out);
int es_ipv4_valida(const char *str);
int es_puerto_valido(int puerto);

#endif /* VALIDATOR_H */
