#include "PiezaTeletransporte.h"
#include <cmath>

PiezaTeletransporte::PiezaTeletransporte(Bando b, sf::Vector2i pos) : Pieza(b, pos) {}

bool PiezaTeletransporte::poderMover(sf::Vector2i destino, const std::vector<Pieza*>& otrasPiezas, bool esDestinoOcupado) {
    int distX = std::abs(destino.x - posicionTablero.x);
    int distY = std::abs(destino.y - posicionTablero.y);

    // Validamos rango (tablero)
    if (distX > rangoMovimiento || distY > rangoMovimiento) return false;

    // Validamos patrón (Ortogonal, Diagonal o Ambos)
    if (patronMovimiento == PatronMovimiento::Ortogonal && (distX != 0 && distY != 0)) return false;
    if (patronMovimiento == PatronMovimiento::Diagonal && (distX != distY)) return false;
    if (patronMovimiento == PatronMovimiento::Ambos && (distX != 0 && distY != 0 && distX != distY)) return false;



    // No puede acabar encima de una pieza aliada 
    for (const auto* otra : otrasPiezas) {
        if (otra->getPosicionTablero() == destino && otra->getBando() == this->bando) {
            return false;
        }
    }
    return true;
}

void PiezaTeletransporte::procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) {

    // Parálisis del basilisco y gestión de ralentizaciones:
    this->gestionarEstadosAlterados(static_cast<double>(dt));
    if (this->getInmovilizado()) return; // Si está paralizado, termina

    // Leemos el color y opacidad actuales del sprite en la arena
    sf::Color colorSprite = spriteArena.getColor();

    // Si NO hay teclas de movimiento pulsadas (Dirección 0,0)
    if (direccion.x == 0.f && direccion.y == 0.f) {
        // La pieza reaparece (se vuelve totalmente opaca)
        if (colorSprite.a != 255) {
            colorSprite.a = 255;
            spriteArena.setColor(colorSprite);
        }
        return;
    }

    // Si se está moviendo, se vuelve invisible
    if (colorSprite.a != 0) {
        colorSprite.a = 0; // Si se pone a 0 desaparece completamente
        spriteArena.setColor(colorSprite);
    }

    // Normalización de vector
    float magnitud = std::hypot(direccion.x, direccion.y);
    if (magnitud != 0.f) direccion /= magnitud;

    float velocidadBase = 500.f;
    // Aplicamos el factor de ralentización
    float velocidadFinal = velocidadBase * this->multiplicadorVelocidadActual;

    sf::Vector2f desplazamiento = direccion * velocidadFinal * dt;
    sf::Vector2f nuevaPos = posicionAbsoluta + desplazamiento;

    if (arena.esPosicionValida(nuevaPos, 20.f, true)) {
        this->moverEnArena(desplazamiento.x, desplazamiento.y);
    }
}