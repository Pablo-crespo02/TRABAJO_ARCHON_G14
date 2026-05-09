#pragma once
#include "Pieza.h"
#include "Arena.h"
#include "Color.h"
#include "PiezaTerrestre.h"
#include <string>
#include <vector>

class ClaseUnicornio : public PiezaTerrestre {
private:

public:
    ClaseUnicornio(Bando b, sf::Vector2i pos, std::string tipo);
    void dibujar(sf::RenderWindow& window, Estado estadoActual);
    void usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) override;
    void procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) override;
};
