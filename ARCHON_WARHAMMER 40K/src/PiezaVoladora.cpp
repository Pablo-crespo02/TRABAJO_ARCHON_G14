#include "PiezaVoladora.h"
#include <cmath>

PiezaVoladora::PiezaVoladora(Bando b, sf::Vector2i pos) : Pieza(b, pos) {}

bool PiezaVoladora::poderMover(sf::Vector2i destino, const std::vector<Pieza*>& otrasPiezas, bool esDestinoOcupado) {
    int distX = std::abs(destino.x - posicionTablero.x);
    int distY = std::abs(destino.y - posicionTablero.y);

    if (distX > rangoMovimiento || distY > rangoMovimiento) return false;

    // 2. Validar Patrón
    if (patronMovimiento == PatronMovimiento::Ortogonal && (distX != 0 && distY != 0)) return false;
    if (patronMovimiento == PatronMovimiento::Diagonal && (distX != distY)) return false;
    if (patronMovimiento == PatronMovimiento::Ambos && (distX != 0 && distY != 0 && distX != distY)) return false;

    // 3. NO se valida el camino porque vuela por encima de las piezas.

    for (const auto* otra : otrasPiezas) {
        if (otra->getPosicionTablero() == destino && otra->getBando() == this->bando) {
            return false;
        }
    }
    return true;
}
void PiezaVoladora::procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) {
    if (direccion == sf::Vector2f(0.f, 0.f)) return;

    this->gestionarEstadosAlterados(static_cast<double>(dt));
    if (this->getInmovilizado()) return;

    float magnitud = std::hypot(direccion.x, direccion.y);
    if (magnitud != 0.f) direccion /= magnitud;

    float velocidadBase = 250.f;
    float velocidadFinal = velocidadBase * this->multiplicadorVelocidadActual;

    sf::Vector2f desplazamiento = direccion * velocidadFinal * dt;
    sf::Vector2f nuevaPos = posicionAbsoluta + desplazamiento;

    if (arena.esPosicionValida(nuevaPos, 20.f, true)) {
        this->moverEnArena(desplazamiento.x, desplazamiento.y);
    }
}