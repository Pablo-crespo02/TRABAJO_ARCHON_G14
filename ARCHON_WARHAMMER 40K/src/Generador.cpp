#include "Generador.h"
#include "Color.h"
#include "Tablero.h"
#include "Motor.h"
#include "ClaseGolem.h"
#include "ClaseUnicornio.h"
#include "ClaseValkyrie.h"
#include "ClaseDjinn.h"
#include "ClaseWizard.h"
#include "ClaseKnight.h"
#include "ClaseArcher.h"
#include "ClaseFenix.h"

void Generador::AnadirUnidad(Motor& motor, Bando bando, std::string tipo, sf::Vector2i pos) {
    Pieza* nuevaPieza = nullptr;

    // Instanciación basada en el rol de Archon/Warhammer
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