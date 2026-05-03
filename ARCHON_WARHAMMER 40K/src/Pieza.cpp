#include "Pieza.h"
#include <iostream>
#include <cmath>
#include "Pieza.h"
#include <iostream>

// 1. CONSTRUCTOR BASE
// Ahora es mucho más sencillo y recibe los datos mínimos obligatorios
Pieza::Pieza(Bando b, sf::Vector2i pos) {
    bando = b;
    posicionTablero = pos;
    seleccionado = false;

    // Inicialización visual básica
    formaVisual.setRadius(20.f);
    formaVisual.setOrigin(20.f, 20.f);

    // Las stats y el rango se llenarán en el constructor de la clase hija
}

// 2. SINCRONIZACIÓN VISUAL
// Esto es igual para todas las piezas: todas se dibujan en el tablero
void Pieza::sincronizarPosicionTablero() {
    float px = static_cast<float>(posicionTablero.x) * 60.0f + 80.0f;
    float py = static_cast<float>(posicionTablero.y) * 60.0f + 60.0f;

    posicionAbsoluta = sf::Vector2f(px, py);
    formaVisual.setPosition(posicionAbsoluta);
}

// 3. MOVIMIENTO EN EL TABLERO
void Pieza::mover(sf::Vector2i destino) {
    posicionTablero = destino;
    sincronizarPosicionTablero();
}

// 4. MOVIMIENTO EN LA ARENA
void Pieza::moverEnArena(float dx, float dy) {
    posicionAbsoluta.x += dx;
    posicionAbsoluta.y += dy;
    formaVisual.setPosition(posicionAbsoluta);
}

// 5. DETECTAR CONFLICTO (Combate)
// Esta lógica es universal: si estoy en la misma celda que un enemigo, hay conflicto
bool Pieza::detectarConflicto(const std::vector<Pieza*>& otrasPiezas) {
    for (const auto* otra : otrasPiezas) {
        if (otra != this && otra->getPosicionTablero() == this->posicionTablero) {
            if (otra->getBando() != this->bando) {
                return true;
            }
        }
    }
    return false;
}