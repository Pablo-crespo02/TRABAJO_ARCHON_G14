#pragma once
#include "PiezaTerrestre.h"
#include "Arena.h"
#include "Color.h"
#include <string>
#include <SFML/Graphics.hpp>

class ClaseKnight : public PiezaTerrestre {
public:
    ClaseKnight(Bando b, sf::Vector2i pos, std::string tipo);

    
    void procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) override;

  
    void dibujar(sf::RenderWindow& window, Estado estadoActual) override;


    void usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) override;
};