#include "Pieza.h"
#include <iostream>
#include <cmath>

// CONSTRUCTOR BASE
Pieza::Pieza(Bando b, sf::Vector2i pos) {
    bando = b;
    posicionTablero = pos;
    seleccionado = false;

    // Valores de seguridad por defecto (las clases de cada unidad los sobreescribirán)
    rangoMovimiento = 0;
    patronMovimiento = PatronMovimiento::Ambos;

    // Inicialización visual básica
    formaVisual.setRadius(20.f);
    formaVisual.setOrigin(20.f, 20.f);
}

// SINCRONIZACIÓN VISUAL
void Pieza::sincronizarPosicionTablero() {
    float px = static_cast<float>(posicionTablero.x) * 60.0f + 80.0f;
    float py = static_cast<float>(posicionTablero.y) * 60.0f + 60.0f;

    posicionAbsoluta = sf::Vector2f(px, py);
    formaVisual.setPosition(posicionAbsoluta);
}

// MOVIMIENTO EN EL TABLERO
void Pieza::mover(sf::Vector2i destino) {
    posicionTablero = destino;
    sincronizarPosicionTablero();
}

// MOVIMIENTO EN LA ARENA
void Pieza::moverEnArena(float dx, float dy) {
    posicionAbsoluta.x += dx;
    posicionAbsoluta.y += dy;
    formaVisual.setPosition(posicionAbsoluta);
}

// DETECTAR CONFLICTO (Combate)
bool Pieza::detectarConflicto(const std::vector<Pieza*>& otrasPiezas) {
    for (const auto otra : otrasPiezas) {
        if (otra != this && otra->getPosicionTablero() == this->posicionTablero) {
            if (otra->getBando() != this->bando) {
                return true;
            }
        }
    }
    return false;
}