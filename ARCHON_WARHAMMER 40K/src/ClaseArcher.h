#pragma once
#include "Pieza.h"
#include "Arena.h"
#include "Color.h"
#include "PiezaTerrestre.h"
#include <string>

class ClaseArcher : public PiezaTerrestre {
public:
    ClaseArcher(Bando b, sf::Vector2i pos, std::string tipo);
    void dibujar(sf::RenderWindow& window, Estado estadoActual);
};