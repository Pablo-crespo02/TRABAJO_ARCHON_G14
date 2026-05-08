#pragma once
#include "PiezaVoladora.h"
#include "Arena.h"
#include "Color.h"
#include <string>

class ClaseFenix : public PiezaVoladora {
public:
    ClaseFenix(Bando b, sf::Vector2i pos, std::string tipo);
    void dibujar(sf::RenderWindow& window, Estado estadoActual);
    void usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) override;
};

