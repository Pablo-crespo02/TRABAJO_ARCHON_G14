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

    // Dibujamos el marco
    // 
    // El tablero mide 590x590. Hacemos el borde de 610x610 (10px extra por lado)
    sf::RectangleShape fondoBorde(sf::Vector2f(580.f, 580.f));

    // Lo movemos 10 píxeles hacia atrás en X e Y para que quede centrado detrás de las casillas
    fondoBorde.setPosition(-20.f, -20.f);

    // Le damos un color gris oscuro para que resalte sin molestar
    fondoBorde.setFillColor(sf::Color(45, 45, 45));

    // Un pequeño borde un poco más claro para darle relieve
    fondoBorde.setOutlineThickness(2.f);
    fondoBorde.setOutlineColor(sf::Color(80, 80, 80));

    // Lo dibujamos primero para que quede al fondo
    window.draw(fondoBorde);


    // 2. Dibujar la casillas:

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            tablero[i][j].Dibujar(window);
        }
    }
}