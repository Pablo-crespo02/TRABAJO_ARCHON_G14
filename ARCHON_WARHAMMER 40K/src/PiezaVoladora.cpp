#include "PiezaVoladora.h"
#include <cmath>

PiezaVoladora::PiezaVoladora(Bando b, sf::Vector2i pos) : Pieza(b, pos) {}

bool PiezaVoladora::poderMover(sf::Vector2i destino, const std::vector<Pieza*>& otrasPiezas, bool esDestinoOcupado) {
    int distX = std::abs(destino.x - posicionTablero.x);
    int distY = std::abs(destino.y - posicionTablero.y);

    if (distX > rangoMovimiento || distY > rangoMovimiento) return false;

<<<<<<< HEAD
    // 2. Validar Patrón
=======
>>>>>>> ae9d14edd8fb2b3f35330778e2828e95dd315d36
    if (patronMovimiento == PatronMovimiento::Ortogonal && (distX != 0 && distY != 0)) return false;
    if (patronMovimiento == PatronMovimiento::Diagonal && (distX != distY)) return false;
    if (patronMovimiento == PatronMovimiento::Ambos && (distX != 0 && distY != 0 && distX != distY)) return false;

    for (const auto* otra : otrasPiezas) {
        if (otra->getPosicionTablero() == destino && otra->getBando() == this->bando) {
            return false;
        }
    }
    return true;
}
void PiezaVoladora::procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) {
    if (direccion == sf::Vector2f(0.f, 0.f)) return;
    float velocidad = 250.f; // Suelen ser más rápidas
    sf::Vector2f desplazamiento = direccion * velocidad * dt;
    sf::Vector2f nuevaPos = posicionAbsoluta + desplazamiento;

    // Se pasa 'true' porque es voladora (ignora obstáculos del suelo)
    if (arena.esPosicionValida(nuevaPos, 20.f, true)) {
        this->moverEnArena(desplazamiento.x, desplazamiento.y);
    }
}