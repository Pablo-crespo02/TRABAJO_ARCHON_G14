#include "Pieza.h"
#include <iostream>
#include <cmath>

Pieza::Pieza() {
    //Inicialización por defecto
    bando = Bando::LUZ;
    tipoMov = TipoMovimiento::TERRESTRE;
    patron = PatronMovimiento::AMBOS;
    rangoMovimiento = 2;
    posicionTablero = { 0, 0 };
    seleccionado = false;

    formaVisual.setRadius(20.f);
    formaVisual.setOrigin(20.f, 20.f);
}

bool Pieza::poderMover(sf::Vector2i destino, const std::vector<Pieza*>& otrasPiezas, bool esDestinoOcupado) {
    int distX = std::abs(destino.x - posicionTablero.x);
    int distY = std::abs(destino.y - posicionTablero.y);

    if (distX > rangoMovimiento || distY > rangoMovimiento) return false;

    bool patronValido = false;
    if (patron == PatronMovimiento::ORTOGONAL) {
        if (distX == 0 || distY == 0) patronValido = true;
    }
    else if (patron == PatronMovimiento::DIAGONAL) {
        if (distX == distY) patronValido = true;
    }
    else if (patron == PatronMovimiento::AMBOS) {
        if (distX == 0 || distY == 0 || distX == distY) patronValido = true;
    }
    if (!patronValido) return false;

    // Colisiones en el camino para terrestres
    if (tipoMov == TipoMovimiento::TERRESTRE) {
        int stepX = (destino.x > posicionTablero.x) ? 1 : (destino.x < posicionTablero.x ? -1 : 0);
        int stepY = (destino.y > posicionTablero.y) ? 1 : (destino.y < posicionTablero.y ? -1 : 0);

        sf::Vector2i revision = posicionTablero;
        while (revision.x + stepX != destino.x || revision.y + stepY != destino.y) {
            if (revision.x + stepX != destino.x) revision.x += stepX;
            if (revision.y + stepY != destino.y) revision.y += stepY;

            for (const auto* otra : otrasPiezas) {
                if (otra->posicionTablero == revision) {
                    return false; // Camino bloqueado
                }
            }
            if (distX <= 1 && distY <= 1) break;
        }
    }

    // Validación de la casilla final
    for (const auto* otra : otrasPiezas) {
        if (otra->posicionTablero == destino) {
            // No puedes superponer una pieza de tu mismo bando
            if (otra->bando == this->bando) return false;
        }
    }

    // Retorno de seguridad (soluciona el C4715)
    return true;
}
void Pieza::sincronizarPosicionTablero() {
    // Usamos las constantes de diseño (puedes ajustarlas según tu Renderizador)
    float px = static_cast<float>(posicionTablero.x) * 60.0f + 50.0f + 30.0f;
    float py = static_cast<float>(posicionTablero.y) * 60.0f + 30.0f + 30.0f;

    posicionAbsoluta = sf::Vector2f(px, py);
    formaVisual.setPosition(posicionAbsoluta);
}
void Pieza::moverEnArena(float dx, float dy) {
    posicionAbsoluta.x += dx;
    posicionAbsoluta.y += dy;
}
bool Pieza::detectarConflicto(const std::vector<Pieza*>& otrasPiezas) {
    for (const auto* otra : otrasPiezas) {
        // Acceso directo gracias a la amistad entre clases
        if (otra != this && otra->posicionTablero == this->posicionTablero) {
            if (otra->bando != this->bando) {
                return true;
            }
        }
    }
    return false;
}

void Pieza::mover(sf::Vector2i destino) {
    posicionTablero = destino;
    sincronizarPosicionTablero();
}
