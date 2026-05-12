#include "Coordinador.h"

int main() {
    // 1. Instanciamos el Coordinador.
    // Al crearse, él se encargará de crear la ventana y el Motor.
    Coordinador miJuego;

    // 2. Simplemente lanzamos el bucle principal.
    // Ya no necesitamos el 'while' aquí, porque el Coordinador 
    // tiene su propio bucle interno (ejecutar).
    miJuego.ejecutar();

    return 0;
}