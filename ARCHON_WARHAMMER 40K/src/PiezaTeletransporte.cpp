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
    // 1. Obtenemos el color actual de la pieza visual
    sf::Color colorActual = formaVisual.getFillColor();

    // 2. Si NO hay teclas de movimiento pulsadas (Dirección 0,0)
    if (direccion.x == 0.f && direccion.y == 0.f) {
        // La pieza reaparece (se vuelve totalmente opaca)
        if (colorActual.a != 255) {
            colorActual.a = 255;
            formaVisual.setFillColor(colorActual);
        }
        return; // Terminamos la función aquí porque no hay desplazamiento físico
    }

    //  Si se está moviendo, se vuelve invisible (translucido)->colorActual = 40    invisible ->colorActual = 0

    if (colorActual.a != 0) {
        colorActual.a = 40;//Si se pone a 0 desaparece
        formaVisual.setFillColor(colorActual);
    }

    // Movimiento a alta velocidad 
    float velocidadTeletransporte = 500.f;
    sf::Vector2f desplazamiento = direccion * velocidadTeletransporte * dt;
    sf::Vector2f nuevaPos = posicionAbsoluta + desplazamiento;

    // Ignora colisiones (como voladora)
    if (arena.esPosicionValida(nuevaPos, 20.f, true)) {
        this->moverEnArena(desplazamiento.x, desplazamiento.y);
    }
}