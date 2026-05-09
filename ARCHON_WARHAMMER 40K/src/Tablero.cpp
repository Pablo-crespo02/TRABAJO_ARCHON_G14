#include "Tablero.h"
#include "Casilla.h"

//Constructor:
Tablero::Tablero() {};

//Función "puente" para que 'Generador.h' pueda modificar atributos de la clase 'casilla'
void Tablero::SetValoresCasillasDesdeTablero(int fila, int columna, TipoCasilla tipo, ColorActual color, bool esPowerPoint) {
    tablero[fila][columna].SetValoresCasillas(fila, columna, tipo, color, esPowerPoint);
}

//

void Tablero::actualizarColores(int numCiclo) {
    // Definimos la secuencia de colores (los 12 estados)
    ColorActual secuencia[] = {
        ColorActual::Blanco_pico, ColorActual::Blanco, ColorActual::Gris_claro,
        ColorActual::Gris_medio,  ColorActual::Gris_oscuro, ColorActual::Negro,
        ColorActual::Negro_pico,  ColorActual::Negro, ColorActual::Gris_oscuro,
        ColorActual::Gris_medio,  ColorActual::Gris_claro, ColorActual::Blanco
    };

    // Obtenemos el color correspondiente (ajustando de 1-12 a índice 0-11)
    ColorActual colorDelCiclo = secuencia[numCiclo - 1];

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            // Actualizamos solo las casillas que NO son permanentes
            if (tablero[i][j].gettipocasilla() == TipoCasilla::ColorCambiante) {
                tablero[i][j].setcoloractualcasilla(colorDelCiclo);
            }
        }
    }
}
void Tablero::dibujar(sf::RenderWindow& window) {
    // Recorremos la matriz 9x9 y mandamos a cada Casilla a dibujarse
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            // 'tablero' es el nombre de tu matriz interna (Casilla tablero[9][9])
            tablero[i][j].Dibujar(window);
        }
    }
}
