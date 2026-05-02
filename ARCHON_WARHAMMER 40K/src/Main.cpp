#include <SFML/Graphics.hpp>
#include "Motor.h"
int main() {
    // 1. Creamos el motor. 
    // NOTA: La ventana DEBE estar dentro del Motor para que manejarEventos funcione.
    Motor mimotor;

    // 2. Bucle principal del juego
    // Usamos una función del motor para saber si la ventana sigue abierta
    while (mimotor.windowAbierta()) {
        mimotor.manejarEventos();
        mimotor.actualizar();
        mimotor.renderizar();
    }

    return 0;
}
