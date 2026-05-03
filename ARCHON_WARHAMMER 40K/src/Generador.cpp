#include "Generador.h"
#include "Color.h"
#include "Tablero.h"
#include "Motor.h"

//FUNCIÓN QUE INICIALIZA EL TABLERO:
void Generador::GenerarTablero(Tablero& tablero) {

    //Se genera una matriz "monigote" para definir qué tipo de casilla es cada posición
    TipoCasilla esquemacasillas[9][9] = {
       {TipoCasilla::FijaNegra,TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra,TipoCasilla::ColorCambiante,TipoCasilla::ColorCambiante,TipoCasilla::ColorCambiante,TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra,TipoCasilla::FijaBlanca},
       {TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra,TipoCasilla::ColorCambiante,TipoCasilla::FijaBlanca,TipoCasilla::ColorCambiante,TipoCasilla::FijaNegra, TipoCasilla::ColorCambiante,TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra},
       {TipoCasilla::FijaNegra,TipoCasilla::ColorCambiante,TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra,TipoCasilla::ColorCambiante,TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra,TipoCasilla::ColorCambiante,TipoCasilla::FijaBlanca},
       {TipoCasilla::ColorCambiante,TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra,TipoCasilla::FijaBlanca,TipoCasilla::ColorCambiante,TipoCasilla::FijaNegra,TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra,TipoCasilla::ColorCambiante},
       {TipoCasilla::FijaBlanca,TipoCasilla::ColorCambiante,TipoCasilla::ColorCambiante,TipoCasilla::ColorCambiante,TipoCasilla::ColorCambiante,TipoCasilla::ColorCambiante,TipoCasilla::ColorCambiante,TipoCasilla::ColorCambiante,TipoCasilla::FijaNegra},
       {TipoCasilla::ColorCambiante,TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra,TipoCasilla::FijaBlanca,TipoCasilla::ColorCambiante,TipoCasilla::FijaNegra,TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra,TipoCasilla::ColorCambiante},
       {TipoCasilla::FijaNegra,TipoCasilla::ColorCambiante,TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra,TipoCasilla::ColorCambiante,TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra,TipoCasilla::ColorCambiante,TipoCasilla::FijaBlanca},
       {TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra,TipoCasilla::ColorCambiante,TipoCasilla::FijaBlanca,TipoCasilla::ColorCambiante,TipoCasilla::FijaNegra, TipoCasilla::ColorCambiante,TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra},
       {TipoCasilla::FijaNegra,TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra,TipoCasilla::ColorCambiante,TipoCasilla::ColorCambiante,TipoCasilla::ColorCambiante,TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra,TipoCasilla::FijaBlanca},
    };

    //Recorremos la matriz "esquemacasillas" para inicializar nuestros valores:
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {

            //Definimos una variables locales para pasarle al consructor de casilla:
            ColorActual colorinicial;
            bool esPowerPoint = false;

            //Definimos qué posiciones son Power Points:
            if ((i == 0 && j == 4) || (i == 4 && j == 0) || (i == 4 && j == 4) || (i == 4 && j == 8) || (i == 8 && j == 4)) {
                esPowerPoint = true;
            }

            //Definimos colores iniciales (cambiantes empiezan en blanco)
            if (esquemacasillas[i][j] == TipoCasilla::FijaNegra) {
                colorinicial = ColorActual::Negro_pico;
            }
            else {
                colorinicial = ColorActual::Blanco_pico;
            }

            //Llamamos al constructor de casilla para cada posición del tablero:
            tablero.SetValoresCasillasDesdeTablero(i, j, esquemacasillas[i][j], colorinicial, esPowerPoint);

        }
    }
}

//FUNCIÓN QUE PERMITE CREAR UNA UNIDAD CON SUS ATRIBUTOS:
void Generador::AnadirUnidad(Motor& motor, Bando bando, std::string tipo, sf::Vector2i pos) {
    Pieza* nuevaPieza = nullptr;

    // El Generador solo sabe qué CLASE crear, los stats los sabe la propia clase interna
    if (tipo == "DREADNOUGHT" || tipo == "CARNIFEX") {
        nuevaPieza = new ClaseGolem(bando, pos, tipo);
    }
    else if (tipo == "LIBRARIAN" || tipo == "HARPY") {
        nuevaPieza = new ClaseFenix(bando, pos, tipo);
    }
    // añadir más tipos más adelante

    if (nuevaPieza) {
        nuevaPieza->sincronizarPosicionTablero();
        motor.listaPiezas.push_back(nuevaPieza);
        // añadir a listas de bando ...
    }
}
void Generador::GenerarDespliegueUnidades(Motor& motor) {
    // --- BANDO LUZ (Columna 0) ---
    // Dreadnoughts en filas 1 y 7
    AnadirUnidad(motor, Bando::LUZ, "DREADNOUGHT", sf::Vector2i(0, 1));
    AnadirUnidad(motor, Bando::LUZ, "DREADNOUGHT", sf::Vector2i(0, 7));

    // Fénix (Librarian) en fila 5
    AnadirUnidad(motor, Bando::LUZ, "LIBRARIAN", sf::Vector2i(0, 5));


    // --- BANDO OSCURIDAD (Columna 8) ---
    // Dreadnoughts (Carnifex) en filas 1 y 7
    AnadirUnidad(motor, Bando::OSCURIDAD, "CARNIFEX", sf::Vector2i(8, 1));
    AnadirUnidad(motor, Bando::OSCURIDAD, "CARNIFEX", sf::Vector2i(8, 7));

    // Fénix (Harpy) en fila 5
    AnadirUnidad(motor, Bando::OSCURIDAD, "HARPY", sf::Vector2i(8, 5));
}