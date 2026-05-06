#include "Generador.h"
#include "Color.h"
#include "Tablero.h"
#include "Motor.h"
#include "ClaseGolem.h"

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


void Generador::AnadirUnidad(Motor& motor, Bando bando, std::string tipo, sf::Vector2i pos) {
    Pieza* nuevaPieza = nullptr;

    // Instanciación basada en el rol de Archon/Warhammer

    if (tipo == "DREADNOUGHT") {
        nuevaPieza = new ClaseGolem(bando, pos, "Dreadnought");
        /*

    if (tipo == "CAPTAIN") {
        nuevaPieza = new ClaseWizard(bando, pos, "Captain Terminator");
    }
    else if (tipo == "LIBRARIAN") {
        nuevaPieza = new ClaseFenix(bando, pos, "Librarian");
    }
    else if (tipo == "ASSAULT_MARINE") {
        nuevaPieza = new ClaseValkyrie(bando, pos, "Assault Marine");
    }
    else if (tipo == "DREADNOUGHT") {
        nuevaPieza = new ClaseGolem(bando, pos, "Dreadnought");
    }
    else if (tipo == "INTERCESSOR") {
        nuevaPieza = new ClaseKnight(bando, pos, "Intercessor");
    }
    else if (tipo == "VINDICARE") {
        nuevaPieza = new ClaseArcher(bando, pos, "Vindicare");
    }
    else if (tipo == "THUNDERHAWK") {
        nuevaPieza = new ClaseDjinn(bando, pos, "Thunderhawk");
    }
    else if (tipo == "PRIMARIS") {
        nuevaPieza = new ClaseUnicornio(bando, pos, "Primaris");
    }

    if (nuevaPieza) {
        nuevaPieza->sincronizarPosicionTablero(); // Actualiza visual en tablero
        motor.listaPiezas.push_back(nuevaPieza);   // Agrega a la gestión del motor
    } */
    }
}

void Generador::GenerarDespliegueUnidades(Motor& motor) {
    // --- DESPLIEGUE IMPERIUM (Luz) - Columna 0 y 1 ---

    // Líder en el centro
    AnadirUnidad(motor, Bando::LUZ, "CAPTAIN", sf::Vector2i(0, 4));

    // Infantería Pesada y Especialistas
    AnadirUnidad(motor, Bando::LUZ, "DREADNOUGHT", sf::Vector2i(0, 1));
    AnadirUnidad(motor, Bando::LUZ, "DREADNOUGHT", sf::Vector2i(0, 7));

    AnadirUnidad(motor, Bando::LUZ, "LIBRARIAN", sf::Vector2i(0, 3));
    AnadirUnidad(motor, Bando::LUZ, "LIBRARIAN", sf::Vector2i(0, 5));

    AnadirUnidad(motor, Bando::LUZ, "ASSAULT_MARINE", sf::Vector2i(0, 2));
    AnadirUnidad(motor, Bando::LUZ, "ASSAULT_MARINE", sf::Vector2i(0, 6));

    // Vanguardia
    AnadirUnidad(motor, Bando::LUZ, "INTERCESSOR", sf::Vector2i(1, 0));
    AnadirUnidad(motor, Bando::LUZ, "INTERCESSOR", sf::Vector2i(1, 8));

    AnadirUnidad(motor, Bando::LUZ, "VINDICARE", sf::Vector2i(1, 2));
    AnadirUnidad(motor, Bando::LUZ, "THUNDERHAWK", sf::Vector2i(1, 4));
    AnadirUnidad(motor, Bando::LUZ, "PRIMARIS", sf::Vector2i(1, 6));
}