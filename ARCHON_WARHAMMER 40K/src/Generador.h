#pragma once
#include "Tablero.h"
#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// --- DECLARACIONES ADELANTADAS ---
// No hacemos include, solo decimos que estas cosas existen
class Tablero;
class Motor;
class Pieza;
enum class Bando;

class Generador {
public:
    static void GenerarTablero(Tablero& tablero);
    static void AnadirUnidad(Motor& motor, Bando bando, std::string tipo, sf::Vector2i pos);
    static void GenerarDespliegueUnidades(Motor& motor);
};