#pragma once
#include "Pieza.h"
#include "Arena.h"
#include "Color.h"
#include <string>

class ClaseValkyrie : public Pieza {
public:
    ClaseValkyrie(Bando b, sf::Vector2i pos, std::string tipo);


    bool poderMover(sf::Vector2i destino, const std::vector<Pieza*>& otrasPiezas, bool esDestinoOcupado) override;
    void procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena);
    void dibujar(sf::RenderWindow& window, Estado estadoActual);
};