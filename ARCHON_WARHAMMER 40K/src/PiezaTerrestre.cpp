#include "PiezaTerrestre.h"
#include <cmath>

PiezaTerrestre::PiezaTerrestre(Bando b, sf::Vector2i pos) : Pieza(b, pos) {}

bool PiezaTerrestre::poderMover(sf::Vector2i destino, const std::vector<Pieza*>& otrasPiezas, bool esDestinoOcupado) {
    int distX = std::abs(destino.x - posicionTablero.x);
    int distY = std::abs(destino.y - posicionTablero.y);

    // 1. Validar Rango
    if (distX > rangoMovimiento || distY > rangoMovimiento) return false;

    // 2. Validar Patrón (0: ORTOGONAL, 1: DIAGONAL, 2: AMBOS)
    if (patronMovimiento == 0 && (distX != 0 && distY != 0)) return false;
    if (patronMovimiento == 1 && (distX != distY)) return false;
    if (patronMovimiento == 2 && (distX != 0 && distY != 0 && distX != distY)) return false;

    // 3. Validar Camino (Paso a paso)
    int stepX = (destino.x > posicionTablero.x) ? 1 : (destino.x < posicionTablero.x ? -1 : 0);
    int stepY = (destino.y > posicionTablero.y) ? 1 : (destino.y < posicionTablero.y ? -1 : 0);
    sf::Vector2i rev = posicionTablero;

    while (rev.x + stepX != destino.x || rev.y + stepY != destino.y) {
        rev.x += stepX;
        rev.y += stepY;
        for (const auto* otra : otrasPiezas) {
            if (otra->getPosicionTablero() == rev) return false; // Hay una pieza bloqueando el paso
        }
    }

    // 4. Validar Destino Final (No pisar aliados)
    for (const auto* otra : otrasPiezas) {
        if (otra->getPosicionTablero() == destino && otra->getBando() == this->bando) {
            return false;
        }
    }
    return true;
}

void PiezaTerrestre::procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) {
    if (direccion == sf::Vector2f(0.f, 0.f)) return;
    float velocidad = 200.f; // Velocidad base terrestre
    sf::Vector2f desplazamiento = direccion * velocidad * dt;
    sf::Vector2f nuevaPos = posicionAbsoluta + desplazamiento;

    // Se pasa 'false' porque NO es voladora (colisiona con rocas/obstáculos)
    if (arena.esPosicionValida(nuevaPos, 20.f, false)) {
        this->moverEnArena(desplazamiento.x, desplazamiento.y);
    }
}