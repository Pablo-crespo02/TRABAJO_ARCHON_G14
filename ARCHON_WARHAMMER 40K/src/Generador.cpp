#include "Generador.h"
#include "Color.h"
#include "Tablero.h"
#include "Motor.h"
#include "ClaseGolem.h"
#include "ClaseFenix.h"
#include "ClaseDjinn.h"
#include "ClaseArcher.h"
#include "ClaseUnicornio.h"
#include "ClaseKnight.h"

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
    if (tipo == "DREADNOUGHT" || tipo == "CARNIFEX") {
        nuevaPieza = new ClaseGolem(bando, pos, tipo);
    }
    else if (tipo == "LIBRARIAN" || tipo == "HARPY") {

        nuevaPieza = new ClaseFenix(bando, pos, tipo);
    }
    else if (tipo == "CULEXUS" || tipo == "GENESTEALER") {
        nuevaPieza = new ClaseDjinn(bando, pos, tipo);
    }
    else if (tipo == "VINDICARE"|| tipo == "GOBLIN") {
        nuevaPieza = new ClaseArcher(bando, pos, tipo);
    }
    else if (tipo == "PRIMARIS"||tipo=="TOXICRENO") {
        nuevaPieza = new ClaseUnicornio(bando, pos, tipo);
    }
    else if (tipo == "INTERCESSOR"||tipo=="TERMAGANT") {
        nuevaPieza = new ClaseKnight(bando, pos, tipo);
    }

    /*

if (tipo == "CAPTAIN") {
    nuevaPieza = new ClaseWizard(bando, pos, "Captain Terminator");
}

else if (tipo == "ASSAULT_MARINE") {
    nuevaPieza = new ClaseValkyrie(bando, pos, "Assault Marine");
}
else if (tipo == "INTERCESSOR") {
    nuevaPieza = new ClaseKnight(bando, pos, "Intercessor");
}

}
else if (tipo == "PRIMARIS") {
    nuevaPieza = new ClaseUnicornio(bando, pos, "Primaris");
}


else if (tipo == "HIVE_TYRANT") {
    nuevaPieza = new ClaseWizard(bando, pos, "Hive Tyrant");
}

else if (tipo == "HARPY") {
    nuevaPieza = new ClaseValkyrie(bando, pos, "Harpy");
}
else if (tipo == "TERMAGANT") {
    nuevaPieza = new ClaseKnight(bando, pos, "Termagant");
}

}
else if (tipo == "TOXICRENO") {
    nuevaPieza = new ClaseUnicornio(bando, pos, "Toxicreno");
}
*/
    if (nuevaPieza != nullptr) {
        nuevaPieza->sincronizarPosicionTablero(); // Calcula la posición real en píxeles
        motor.listaPiezas.push_back(nuevaPieza);  // La mete en la lista para que el Motor la dibuje
    }
}

void Generador::GenerarDespliegueUnidades(Motor& motor) {
    // --- DESPLIEGUE IMPERIUM (Luz) - Columna 0 y 1 ---

    AnadirUnidad(motor, Bando::LUZ, "ASSAULT_MARINE", sf::Vector2i(0, 0));
    AnadirUnidad(motor, Bando::LUZ, "DREADNOUGHT", sf::Vector2i(0, 1));
    AnadirUnidad(motor, Bando::LUZ, "PRIMARIS", sf::Vector2i(0, 2));
    AnadirUnidad(motor, Bando::LUZ, "CULEXUS", sf::Vector2i(0, 3));
    AnadirUnidad(motor, Bando::LUZ, "CAPTAIN", sf::Vector2i(0, 4)); //LIDER
    AnadirUnidad(motor, Bando::LUZ, "LIBRARIAN", sf::Vector2i(0, 5));
    AnadirUnidad(motor, Bando::LUZ, "PRIMARIS", sf::Vector2i(0, 6));
    AnadirUnidad(motor, Bando::LUZ, "DREADNOUGHT", sf::Vector2i(0, 7));
    AnadirUnidad(motor, Bando::LUZ, "ASSAULT_MARINE", sf::Vector2i(0, 8));
    AnadirUnidad(motor, Bando::LUZ, "VINDICARE", sf::Vector2i(1, 0));
    AnadirUnidad(motor, Bando::LUZ, "VINDICARE", sf::Vector2i(1, 8));

    for (int i = 1; i < 8; i++) {
        AnadirUnidad(motor, Bando::LUZ, "INTERCESSOR", sf::Vector2i(1, i));
    }
    AnadirUnidad(motor, Bando::LUZ, "FENIX", sf::Vector2i(2, 4));

    //BANDO OSCURIDAD DESPLIEGUE
    AnadirUnidad(motor, Bando::OSCURIDAD, "LICTOR", sf::Vector2i(8, 0));
    AnadirUnidad(motor, Bando::OSCURIDAD, "CARNIFEX", sf::Vector2i(8, 1));
    AnadirUnidad(motor, Bando::OSCURIDAD, "TOXICRENO", sf::Vector2i(8, 2));
    AnadirUnidad(motor, Bando::OSCURIDAD, "GENESTEALER", sf::Vector2i(8, 3));
    AnadirUnidad(motor, Bando::OSCURIDAD, "HIVE_TYRANT", sf::Vector2i(8, 4));//LÍDER 
    AnadirUnidad(motor, Bando::OSCURIDAD, "HARPY", sf::Vector2i(8, 5));
    AnadirUnidad(motor, Bando::OSCURIDAD, "TOXICRENO", sf::Vector2i(8, 6));
    AnadirUnidad(motor, Bando::OSCURIDAD, "CARNIFEX", sf::Vector2i(8, 7));
    AnadirUnidad(motor, Bando::OSCURIDAD, "LICTOR", sf::Vector2i(8, 8));

    AnadirUnidad(motor, Bando::OSCURIDAD, "GARGOLA", sf::Vector2i(7, 0));
    AnadirUnidad(motor, Bando::OSCURIDAD, "GARGOLA", sf::Vector2i(7, 8));

    for (int i = 1; i < 8; i++) {
        AnadirUnidad(motor, Bando::OSCURIDAD, "TERMAGANT", sf::Vector2i(7, i));
    }
}