#include "PiezaTerrestre.h"
#include <cmath>

PiezaTerrestre::PiezaTerrestre(Bando b, sf::Vector2i pos) : Pieza(b, pos) {}

bool PiezaTerrestre::poderMover(sf::Vector2i destino, const std::vector<Pieza*>& otrasPiezas, bool esDestinoOcupado) {
    int distX = std::abs(destino.x - posicionTablero.x);
    int distY = std::abs(destino.y - posicionTablero.y);

    // 1. Validar Rango
    if (distX > rangoMovimiento || distY > rangoMovimiento) return false;

    // 2. Validar Patrón (usando el enum PatronMovimiento)
    if (patronMovimiento == PatronMovimiento::Ortogonal && (distX != 0 && distY != 0)) return false;
    if (patronMovimiento == PatronMovimiento::Diagonal && (distX != distY)) return false;
    if (patronMovimiento == PatronMovimiento::Ambos && (distX != 0 && distY != 0 && distX != distY)) return false;

    // 3. Validar Camino (Paso a paso para colisiones terrestres)
    int stepX = (destino.x > posicionTablero.x) ? 1 : (destino.x < posicionTablero.x ? -1 : 0);
    int stepY = (destino.y > posicionTablero.y) ? 1 : (destino.y < posicionTablero.y ? -1 : 0);
    sf::Vector2i rev = posicionTablero;

    // Comprobamos casillas intermedias
    while (rev.x + stepX != destino.x || rev.y + stepY != destino.y) {
        rev.x += stepX;
        rev.y += stepY;
        for (const auto* otra : otrasPiezas) {
            if (otra->getPosicionTablero() == rev) return false; // Camino bloqueado
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

    // 1. Actualiza todos los estados (incluida la ralentización)
    this->gestionarEstadosAlterados(static_cast<double>(dt));
    if (this->getInmovilizado()) return;

    // 2. Normalizamos la dirección para evitar que el movimiento en diagonal sea más rápido
    float magnitud = std::hypot(direccion.x, direccion.y);
    if (magnitud != 0.f) direccion /= magnitud;

    // 3. Calculamos la velocidad final aplicando el multiplicador
    float velocidadBase = 250.f;
    float velocidadFinal = velocidadBase * this->multiplicadorVelocidadActual;

    // 4. Resolvemos físicas
    sf::Vector2f desplazamiento = direccion * velocidadFinal * dt;
    sf::Vector2f nuevaPos = posicionAbsoluta + desplazamiento;

    if (arena.esPosicionValida(nuevaPos, 20.f, false)) {
        this->moverEnArena(desplazamiento.x, desplazamiento.y);
    }
}