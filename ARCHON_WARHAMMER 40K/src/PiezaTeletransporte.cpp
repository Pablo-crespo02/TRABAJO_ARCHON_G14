#include "PiezaTeletransporte.h"

<<<<<<< HEAD
#include <cmath>
=======
>>>>>>> ae9d14edd8fb2b3f35330778e2828e95dd315d36

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

    // 3. SALTO: Al ser teletransporte, no validamos el camino (ignora piezas intermedias)[cite: 1]

    // 4. Validar Destino Final (No pisar aliados)
    for (const auto* otra : otrasPiezas) {
        if (otra->getPosicionTablero() == destino && otra->getBando() == this->bando) {
            return false;
        }
    }
    return true;
}

void PiezaTeletransporte::procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) {
    if (direccion == sf::Vector2f(0.f, 0.f)) return;

    // Velocidad de desplazamiento para el teletransporte/dash
    float velocidadTeletransporte = 500.f;
    sf::Vector2f desplazamiento = direccion * velocidadTeletransporte * dt;
    sf::Vector2f nuevaPos = posicionAbsoluta + desplazamiento;

    // El teletransporte ignora colisiones de ambiente (pasamos true a esVoladora)
    if (arena.esPosicionValida(nuevaPos, 20.f, true)) {
        // CORRECCIÓN: Llamamos a moverEnArena para aplicar el desplazamiento
        this->moverEnArena(desplazamiento.x, desplazamiento.y);
    }
}