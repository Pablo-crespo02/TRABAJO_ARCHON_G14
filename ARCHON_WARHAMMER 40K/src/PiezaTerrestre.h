#pragma once
#include "Pieza.h"

class PiezaTerrestre : public Pieza {
public:
    PiezaTerrestre(Bando b, sf::Vector2i pos);

    bool poderMover(sf::Vector2i destino, const std::vector<Pieza*>& otrasPiezas, bool esDestinoOcupado) override;
    void procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) override;
};