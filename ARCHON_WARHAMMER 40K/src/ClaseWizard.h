#pragma once
#include "Pieza.h"
#include "PiezaVoladora.h"
#include "Arena.h"
#include "Color.h"
#include <string>

class ClaseWizard : public PiezaVoladora {
public:
    ClaseWizard(Bando b, sf::Vector2i pos, std::string tipo);
    void dibujar(sf::RenderWindow& window, Estado estadoActual);
};