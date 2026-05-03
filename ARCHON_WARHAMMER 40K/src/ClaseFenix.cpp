#include "ClaseFenix.h"
#include <cmath>

ClaseFenix::ClaseFenix(Bando b, sf::Vector2i pos, std::string tipo)
    : Pieza(b, pos)
{
    this->bando = b;
    this->posicionTablero = pos;
    this->stats.nombre = tipo;

    // Estadísticas del Arquetipo Fénix (Volador)
    this->stats.vida = 7.0f;
    this->stats.ataque = 8.0f;
    this->stats.defensa = 5.0f;
    this->rangoMovimiento = 4;
    this->stats.esRango = false;

    // Aquí podrías diferenciar visualmente si es Librarian o Harpy si fuera necesario
}

bool ClaseFenix::poderMover(sf::Vector2i destino, const std::vector<Pieza*>& otrasPiezas, bool esDestinoOcupado) {
    // 1. Cálculo de distancias
    int distX = std::abs(destino.x - posicionTablero.x);
    int distY = std::abs(destino.y - posicionTablero.y);

    // 2. Comprobar Rango
    if (distX > rangoMovimiento || distY > rangoMovimiento) return false;

    // 3. Comprobar Patrón (AMBOS para el Fénix)
    if (distX != 0 && distY != 0 && distX != distY) return false;

    // 4. VALIDACIÓN DE COLISIONES: Al ser VOLADOR, ignoramos el camino.

    // 5. VALIDACIÓN DEL DESTINO FINAL
    // Solo comprobamos que no aterrice sobre un aliado
    for (const auto* otra : otrasPiezas) {
        if (otra->getPosicionTablero() == destino) {
            if (otra->getBando() == this->bando) {
                return false; // No puedes pisar a un aliado
            }
        }
    }

    return true; // Si el patrón es correcto y no hay aliados, el fénix vuela hasta allí
}
void ClaseFenix::procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) {
    if (direccion == sf::Vector2f(0.f, 0.f)) return;

    float velocidad = 250.f;
    sf::Vector2f desplazamiento = direccion * velocidad * dt;
    sf::Vector2f nuevaPos = posicionAbsoluta + desplazamiento;

    // El Fénix sabe que SÍ vuela (true)
    if (arena.esPosicionValida(nuevaPos, 20.f, true)) {
        this->moverEnArena(desplazamiento.x, desplazamiento.y);
    }
}