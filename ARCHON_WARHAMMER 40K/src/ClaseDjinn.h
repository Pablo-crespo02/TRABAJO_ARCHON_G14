#pragma once
#include "PiezaTeletransporte.h"
#include "Arena.h"
#include "Color.h"
#include <string>

class ClaseDjinn : public PiezaTeletransporte {
public:
    ClaseDjinn(Bando b, sf::Vector2i pos, std::string tipo);
    void dibujar(sf::RenderWindow& window, Estado estadoActual) override;
};