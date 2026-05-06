#pragma once
#include "Pieza.h"

class PiezaVoladora : public Pieza {
public:
    PiezaVoladora(Bando b, sf::Vector2i pos);

    bool poderMover(sf::Vector2i destino, const std::vector<Pieza*>& otrasPiezas, bool esDestinoOcupado) override;
    void procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) override;
};