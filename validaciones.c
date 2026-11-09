#include "validaciones.h"
#include "simulador.h"
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/* Lectura acotada caracter a caracter, equivalente a una linea con fgets,
   pero detecta tambien NUL embebidos. Consume TODA la linea demasiado larga.
   2 significa linea rechazada; EOF sin datos termina sin reintentos infinitos. */
static int leer_linea(const char *mensaje, char *linea, size_t capacidad)
{
    size_t usados = 0;
    int c, invalida = 0, hubo_datos = 0;
    if (fputs(mensaje, stdout) == EOF || fflush(stdout) == EOF) return -1;
    while ((c = fgetc(stdin)) != EOF && c != '\n') {
        hubo_datos = 1;
        if (c == '\0') invalida = 1;
        if (usados + 1 < capacidad) linea[usados++] = (char)c;
        else invalida = 1;
    }
    linea[usados] = '\0';
    if (ferror(stdin)) return -1;
    if (c == EOF && !hubo_datos) return 0;
    if (invalida) {
        puts("Entrada demasiado larga o con caracteres NUL. Reintente.");
        return 2;
    }
    return 1;
}

static int solo_espacios(const char *texto)
{
    while (isspace((unsigned char)*texto)) ++texto;
    return *texto == '\0';
}

int leer_entero(const char *mensaje, int minimo, int maximo, int *destino)
{
    char linea[TAM_ENTRADA], *fin;
    long valor;
    int estado;
    if (!mensaje || !destino || minimo > maximo) return -1;
    for (;;) {
        estado = leer_linea(mensaje, linea, sizeof linea);
        if (estado <= 0) return estado;
        if (estado == 2) continue;
        errno = 0;
        valor = strtol(linea, &fin, 10);
        if (fin != linea && errno == 0 && solo_espacios(fin) &&
            valor >= minimo && valor <= maximo) {
            *destino = (int)valor;
            return 1;
        }
        printf("Ingrese un entero completo entre %d y %d.\n", minimo, maximo);
    }
}

/* Gramatica decimal explicita: rechaza hexadecimales, nan, inf y basura.
   Admite punto O coma decimal y notacion cientifica; no separadores de miles. */
static int decimal_valido(char *texto)
{
    char *p = texto;
    int digitos = 0;
    while (isspace((unsigned char)*p)) ++p;
    if (*p == '+' || *p == '-') ++p;
    while (isdigit((unsigned char)*p)) { ++p; ++digitos; }
    if (*p == '.' || *p == ',') {
        *p++ = '.';
        while (isdigit((unsigned char)*p)) { ++p; ++digitos; }
    }
    if (!digitos) return 0;
    if (*p == 'e' || *p == 'E') {
        ++p;
        if (*p == '+' || *p == '-') ++p;
        if (!isdigit((unsigned char)*p)) return 0;
        while (isdigit((unsigned char)*p)) ++p;
    }
    return solo_espacios(p);
}

int leer_real(const char *mensaje, double minimo, double maximo, double *destino)
{
    char linea[TAM_ENTRADA], *fin;
    double valor;
    int estado;
    if (!mensaje || !destino || !isfinite(minimo) || !isfinite(maximo) ||
        minimo > maximo) return -1;
    for (;;) {
        estado = leer_linea(mensaje, linea, sizeof linea);
        if (estado <= 0) return estado;
        if (estado == 2) continue;
        if (decimal_valido(linea)) {
            errno = 0;
            valor = strtod(linea, &fin);
            if (fin != linea && errno == 0 && solo_espacios(fin) &&
                isfinite(valor) && valor >= minimo && valor <= maximo) {
                *destino = valor;
                return 1;
            }
        }
        printf("Ingrese un decimal finito entre %.6g y %.6g, sin miles.\n",
               minimo, maximo);
    }
}

int leer_caracter(const char *mensaje, const char *permitidos, char *destino)
{
    char linea[TAM_ENTRADA], *p;
    int estado, c;
    if (!mensaje || !permitidos || !*permitidos || !destino) return -1;
    for (;;) {
        estado = leer_linea(mensaje, linea, sizeof linea);
        if (estado <= 0) return estado;
        if (estado == 2) continue;
        p = linea;
        while (isspace((unsigned char)*p)) ++p;
        if (*p) {
            c = tolower((unsigned char)*p++);
            if (solo_espacios(p) && strchr(permitidos, c)) {
                *destino = (char)c;
                return 1;
            }
        }
        printf("Ingrese un solo caracter de: %s (sin distinguir mayusculas).\n",
               permitidos);
    }
}
