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

    //Parálisis del basilisco:
    this->gestionarEstadosAlterados(static_cast<double>(dt));//estado alterado de parálisis
    if (this->getInmovilizado()) return;//Si está paralizado, termina

    float velocidad = 200.f; // Velocidad base terrestre
    sf::Vector2f desplazamiento = direccion * velocidad * dt;
    sf::Vector2f nuevaPos = posicionAbsoluta + desplazamiento;

    // 'false' indica que colisiona con obstáculos ambientales (rocas)
    if (arena.esPosicionValida(nuevaPos, 20.f, false)) {
        this->moverEnArena(desplazamiento.x, desplazamiento.y);
    }
}