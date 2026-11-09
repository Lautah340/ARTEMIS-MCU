#include "simulador.h"
#include "validaciones.h"
#include <math.h>
#include <stdlib.h>

int calcular_resultados(const Parametros *p, Resultados *destino)
{
    Resultados r;
    if (!p || !destino) return 0;
    if (!isfinite(p->altitud) || !isfinite(p->masa) ||
        !isfinite(p->aceleracion_lateral) || !isfinite(p->duracion) ||
        p->altitud < ALTITUD_MIN || p->altitud > ALTITUD_MAX ||
        p->masa < MASA_MIN || p->masa > MASA_MAX ||
        p->aceleracion_lateral < 0 || p->aceleracion_lateral > ACELERACION_MAX ||
        p->duracion < 0 || p->duracion > DURACION_MAX) return 0;
    r.radio = RADIO_TIERRA + p->altitud;
    /* G*M*m/r^2 = m*v^2/r: la masa de Orion se cancela. */
    r.velocidad = sqrt(G * MASA_TIERRA / r.radio);
    r.periodo = 2.0 * PI * r.radio / r.velocidad;
    r.aceleracion = r.velocidad * r.velocidad / r.radio;
    r.fuerza_gravitatoria = G * MASA_TIERRA * p->masa / (r.radio * r.radio);
    r.fuerza_centripeta = p->masa * r.aceleracion;
    /* Perturbacion local con velocidad lateral inicial relativa nula.
       No representa la caida gravitatoria total respecto de la tangente. */
    r.delta_v = p->aceleracion_lateral * p->duracion;
    r.desviacion = 0.5 * p->aceleracion_lateral * p->duracion * p->duracion;
    r.avance_local = r.velocidad * p->duracion;
    *destino = r;
    return 1;
}

static void guardar_ensayo(Simulador *s, const Ensayo *ensayo)
{
    size_t i;
    if (s->cantidad == CAPACIDAD_HISTORIAL) {
        for (i = 1; i < s->cantidad; ++i) s->historial[i - 1] = s->historial[i];
        --s->cantidad;
    }
    s->actual = *ensayo;
    s->historial[s->cantidad++] = *ensayo;
}

void inicializar_simulador(Simulador *s)
{
    Ensayo nominal = { { ALTITUD_LEO, MASA_ORION,
                         ACELERACION_LATERAL, DURACION_PULSO }, {0} };
    if (!s) return;
    s->cantidad = 0;
    if (calcular_resultados(&nominal.parametros, &nominal.resultados))
        guardar_ensayo(s, &nominal);
}

void imprimir_analisis(FILE *f, const Ensayo *e, int apartado)
{
    const Parametros *p;
    const Resultados *r;
    if (!f || !e) return;
    p = &e->parametros;
    r = &e->resultados;
    fprintf(f, "\nOrion: h=%.3f m; m=%.3f kg; r=%.3f m\n", p->altitud, p->masa, r->radio);
    if (apartado == 0 || apartado == 1) {
        fprintf(f, "\nA. Gravitacion y velocidad orbital\n"
                "Fg = G*M*m/r^2; Fc = m*v^2/r. Fg = Fc implica v = sqrt(G*M/r).\n"
                "La gravedad apunta al centro y proporciona TODA la fuerza centripeta;\n"
                "no son dos fuerzas independientes que deban sumarse.\n"
                "Fg = %.6f N; Fc = %.6f N; v = %.6f m/s (%.6f km/s).\n",
                r->fuerza_gravitatoria, r->fuerza_centripeta,
                r->velocidad, r->velocidad / 1000.0);
    }
    if (apartado == 0 || apartado == 2)
        fprintf(f, "\nB. T = 2*pi*r/v = %.6f s = %.6f min.\n",
                r->periodo, r->periodo / SEGUNDOS_MINUTO);
    if (apartado == 0 || apartado == 3)
        fprintf(f, "\nC. ac = v^2/r = G*M/r^2 = %.6f m/s^2.\n"
                "Nave y astronautas estan en caida libre: esta aceleracion\n"
                "gravitatoria no equivale a una carga de apoyo sentida.\n", r->aceleracion);
    if (apartado == 0 || apartado == 4) {
        fprintf(f, "\nD. Pulso lateral: a=%.6f m/s^2; dt=%.6f s\n"
                "Modelo local: x=v*t; y=0.5*a*t^2; y(x)=a*x^2/(2*v^2).\n"
                "Desviacion adicional al final: %.6f m; delta_v lateral: %.6f m/s.\n"
                "Avance tangencial local: %.6f m; dt/T=%.6f.\n",
                p->aceleracion_lateral, p->duracion, r->desviacion,
                r->delta_v, r->avance_local, p->duracion / r->periodo);
        if (p->duracion == 0 || p->aceleracion_lateral == 0) {
            fprintf(f, "No hay perturbacion: continua la orbita circular ideal.\n");
        } else {
            fprintf(f, "Durante el pulso la perturbacion es parabolica en la aproximacion\n"
                    "local de ejes fijos, con eje lateral perpendicular a la velocidad inicial.\n"
                    "La trayectoria geocentrica deja de ser el MCU original. Al apagar\n"
                    "el motor sigue una conica determinada por posicion y velocidad finales.\n"
                    "El valor y no es una solucion orbital exacta ni una altitud final.\n"
                    "Exigir empuje perpendicular a la velocidad INSTANTANEA requiere\n"
                    "especificar su direccion radial o fuera del plano e integrar la dinamica.\n");
        }
    }
}

static void imprimir_reporte(FILE *f, const Simulador *s)
{
    size_t i;
    fprintf(f, "SIMULADOR DE MISIONES ARTEMIS - SITUACION 2\n"
            "G=%.9g N*m^2/kg^2; M_T=%.9g kg; R_T=%.1f m\n"
            "Caso nominal: h=%.1f m; r=%.1f m; m=%.1f kg\n"
            "Modelo: Tierra esferica, gravedad newtoniana, sin atmosfera ni otras perturbaciones.\n"
            "La estabilidad circular es ideal; no predice la duracion real de una orbita LEO.\n",
            G, MASA_TIERRA, RADIO_TIERRA, ALTITUD_LEO, RADIO_ORBITAL, MASA_ORION);
    imprimir_analisis(f, &s->actual, 0);
    fprintf(f, "\nUltimos %zu ensayos independientes (no son maniobras acumuladas):\n"
            " N | h(m) | masa(kg) | a_lat(m/s^2) | dt(s) | v(m/s) | T(s) | d(m)\n", s->cantidad);
    for (i = 0; i < s->cantidad; ++i) {
        const Ensayo *e = &s->historial[i];
        fprintf(f, "%2zu | %.1f | %.1f | %.3f | %.2f | %.3f | %.3f | %.3f\n",
                i + 1, e->parametros.altitud, e->parametros.masa,
                e->parametros.aceleracion_lateral, e->parametros.duracion,
                e->resultados.velocidad, e->resultados.periodo, e->resultados.desviacion);
    }
}

int generar_reporte(const Simulador *s)
{
    FILE *f;
    int correcto;
    if (!s) return 0;
    f = fopen(ARCHIVO_REPORTE, "w");
    if (!f) { perror("No se pudo abrir el reporte"); return 0; }
    imprimir_reporte(f, s);
    correcto = !ferror(f);
    if (fclose(f) != 0) correcto = 0;
    if (!correcto) fprintf(stderr, "Error de escritura: reporte posiblemente incompleto.\n");
    return correcto;
}

static int configurar(Simulador *s)
{
    Ensayo nuevo;
    int estado;
    puts("Ensayo independiente. Decimales con punto o coma, sin separadores de miles.\n"
         "Limites didacticos: h=100000..2000000 m; a=0..1 m/s^2; dt=0..30 s.\n"
         "El pulso breve permite usar una aproximacion local; no integra una orbita perturbada.");
    estado = leer_real("Altitud (m): ", ALTITUD_MIN, ALTITUD_MAX, &nuevo.parametros.altitud);
    if (estado != 1) return estado;
    estado = leer_real("Masa Orion (kg, 1..1000000): ", MASA_MIN, MASA_MAX, &nuevo.parametros.masa);
    if (estado != 1) return estado;
    estado = leer_real("Aceleracion lateral (m/s^2): ", 0, ACELERACION_MAX, &nuevo.parametros.aceleracion_lateral);
    if (estado != 1) return estado;
    estado = leer_real("Duracion (s): ", 0, DURACION_MAX, &nuevo.parametros.duracion);
    if (estado != 1) return estado;
    if (!calcular_resultados(&nuevo.parametros, &nuevo.resultados)) return -1;
    /* Actualizacion transaccional: no cambia el estado ante ingreso incompleto. */
    guardar_ensayo(s, &nuevo);
    puts("Ensayo calculado y guardado.");
    return 1;
}

int menu_principal(Simulador *s)
{
    int opcion, estado;
    char confirmar;
    const char *opciones[] = {
        "0. Salir", "1. A: fuerzas y velocidad", "2. B: periodo orbital",
        "3. C: aceleracion centripeta", "4. D: pulso lateral",
        "5. Configurar y calcular ensayo", "6. Restablecer caso nominal",
        "7. Reporte completo en pantalla", "8. Exportar reporte a texto"
    };
    size_t i;
    if (!s || !s->cantidad) return EXIT_FAILURE;
    for (;;) {
        puts("\n=== ARTEMIS / ORION / LEO ===");
        for (i = 0; i < sizeof opciones / sizeof opciones[0]; ++i) puts(opciones[i]);
        estado = leer_entero("Opcion: ", 0, 8, &opcion);
        if (estado != 1) break;
        if (opcion == 0) { puts("Fin de la simulacion."); return EXIT_SUCCESS; }
        if (opcion >= 1 && opcion <= 4) imprimir_analisis(stdout, &s->actual, opcion);
        else if (opcion == 5) {
            estado = configurar(s);
            if (estado != 1) break;
        } else if (opcion == 6) {
            Ensayo nominal;
            nominal.parametros = (Parametros){ALTITUD_LEO, MASA_ORION, ACELERACION_LATERAL, DURACION_PULSO};
            if (!calcular_resultados(&nominal.parametros, &nominal.resultados)) return EXIT_FAILURE;
            guardar_ensayo(s, &nominal);
            puts("Caso nominal restablecido; historial conservado.");
        } else if (opcion == 7) imprimir_reporte(stdout, s);
        else {
            estado = leer_caracter("Se escribira/sobrescribira reporte_artemis.txt. Continuar (s/n): ", "sn", &confirmar);
            if (estado != 1) break;
            if (confirmar == 's' && generar_reporte(s)) puts("Reporte guardado.");
        }
        if (ferror(stdout)) return EXIT_FAILURE;
    }
    if (estado < 0) { fputs("Error de entrada/salida.\n", stderr); return EXIT_FAILURE; }
    puts("\nFin de entrada: cierre limpio; se conserva el ultimo ensayo completo.");
    return EXIT_SUCCESS;
}
