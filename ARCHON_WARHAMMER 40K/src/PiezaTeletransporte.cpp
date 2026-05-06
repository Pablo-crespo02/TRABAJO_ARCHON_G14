#include "PiezaTeletransporte.h"
#include <cmath>

PiezaTeletransporte::PiezaTeletransporte(Bando b, sf::Vector2i pos) : Pieza(b, pos) {}

bool PiezaTeletransporte::poderMover(sf::Vector2i destino, const std::vector<Pieza*>& otrasPiezas, bool esDestinoOcupado) {
    int distX = std::abs(destino.x - posicionTablero.x);
    int distY = std::abs(destino.y - posicionTablero.y);

    // 1. Validar Rango
    if (distX > rangoMovimiento || distY > rangoMovimiento) return false;

    // 2. Validar Patrón (Ortogonal, Diagonal o Ambos)
    if (patronMovimiento == PatronMovimiento::Ortogonal && (distX != 0 && distY != 0)) return false;
    if (patronMovimiento == PatronMovimiento::Diagonal && (distX != distY)) return false;
    if (patronMovimiento == PatronMovimiento::Ambos && (distX != 0 && distY != 0 && distX != distY)) return false;

    // 3. SALTO: No validamos el camino. Al ser teletransporte, "desaparece y aparece",
    // por lo que las piezas que haya en medio no bloquean el paso.

    // 4. Validar Destino Final
    for (const auto* otra : otrasPiezas) {
        if (otra->getPosicionTablero() == destino && otra->getBando() == this->bando) {
            return false; // No puede teletransportarse encima de un aliado
        }
    }
    return true;
}

void PiezaTeletransporte::procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) {
    if (direccion == sf::Vector2f(0.f, 0.f)) return;

    // En Archon, el teletransporte en combate suele ser un "salto" corto o un dash
    // que atraviesa obstáculos.
    float velocidadTeletransporte = 500.f;
    sf::Vector2f desplazamiento = direccion * velocidadTeletransporte * dt;
    sf::Vector2f nuevaPos = posicionAbsoluta + desplazamiento;

    // El teletransporte ignora colisiones de ambiente (rocas) igual que el volador,
    // pero podemos hacerlo incluso más rápido o con un efecto visual.
    if (arena.esPosicionValida(nuevaPos, 20.f, true)) { // true = ignora rocas
        this->moverEnArena(desplazamiento.x, desplazamiento.y);
    }
}