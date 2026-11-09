#ifndef VALIDACIONES_H
#define VALIDACIONES_H

/* Retorno: 1 = dato valido, 0 = EOF, -1 = error de E/S o contrato.
   Ante EOF/error no se modifica el destino. Nunca se usa fflush(stdin). */
int leer_entero(const char *mensaje, int minimo, int maximo, int *destino);
int leer_real(const char *mensaje, double minimo, double maximo, double *destino);
int leer_caracter(const char *mensaje, const char *permitidos, char *destino);

#endif
