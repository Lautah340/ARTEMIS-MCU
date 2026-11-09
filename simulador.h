#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <stddef.h>
#include <stdio.h>

/* Unidades del Sistema Internacional; no se redondean los calculos. */
#define MASA_TIERRA 5.972e24
#define RADIO_TIERRA 6371000.0
#define G 6.674e-11
#define MASA_ORION 33000.0
#define ALTITUD_LEO 192000.0
#define RADIO_ORBITAL (RADIO_TIERRA + ALTITUD_LEO)
#define ACELERACION_LATERAL 0.50
#define DURACION_PULSO 10.0
#define PI 3.14159265358979323846
#define SEGUNDOS_MINUTO 60.0
#define ALTITUD_MIN 100000.0
#define ALTITUD_MAX 2000000.0
#define MASA_MIN 1.0
#define MASA_MAX 1000000.0
#define ACELERACION_MAX 1.0
#define DURACION_MAX 30.0
#define CAPACIDAD_HISTORIAL 10
#define TAM_ENTRADA 256
#define ARCHIVO_REPORTE "reporte_artemis.txt"

typedef struct {
    double altitud, masa, aceleracion_lateral, duracion;
} Parametros;

typedef struct {
    double radio, velocidad, periodo, aceleracion;
    double fuerza_gravitatoria, fuerza_centripeta;
    double desviacion, delta_v, avance_local;
} Resultados;

typedef struct {
    Parametros parametros;
    Resultados resultados;
} Ensayo;

typedef struct {
    Ensayo actual;
    Ensayo historial[CAPACIDAD_HISTORIAL];
    size_t cantidad;
} Simulador;

void inicializar_simulador(Simulador *simulador);
int calcular_resultados(const Parametros *parametros, Resultados *resultados);
void imprimir_analisis(FILE *salida, const Ensayo *ensayo, int apartado);
int generar_reporte(const Simulador *simulador);
int menu_principal(Simulador *simulador);

#endif
