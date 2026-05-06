#pragma once
#include "Pieza.h"
#include "PiezaTerrestre.h"
#include "Arena.h"
#include "Color.h"
#include <string>

class ClaseUnicornio : public PiezaTerrestre {
public:
    ClaseUnicornio(Bando b, sf::Vector2i pos, std::string tipo);
    void dibujar(sf::RenderWindow& window, Estado estadoActual);
};