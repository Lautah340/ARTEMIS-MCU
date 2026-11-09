#include "simulador.h"

int main(void)
{
    Simulador simulador;
    inicializar_simulador(&simulador);
    return menu_principal(&simulador);
}
