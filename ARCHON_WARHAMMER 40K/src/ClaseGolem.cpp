#include "ClaseGolem.h"
#include <cmath> // Necesario para std::abs

// Fíjate en cómo llamamos al constructor de la clase Padre (Pieza)
ClaseGolem::ClaseGolem(Bando b, sf::Vector2i pos, std::string tipo)
    : Pieza(b, pos) // Llama al constructor base de Pieza (ajusta Pieza.h si es necesario)
{
    this->bando = b;
    this->posicionTablero = pos;
    this->stats.nombre = tipo;

    // Aquí defines las estadísticas fijas del "Arquetipo Golem"
    this->stats.vida = 10.0f;
    this->stats.ataque = 4.0f;
    this->stats.defensa = 9.0f;
    this->rangoMovimiento = 2;
    this->stats.esRango = false;

    // Personalización extra si quieres diferenciar Dreadnought de Carnifex
    if (tipo == "DREADNOUGHT") {
        // Podrías ajustar algo específico aquí si quisieras
    }
}

// Fíjate en el prefijo ClaseGolem::
bool ClaseGolem::poderMover(sf::Vector2i destino, const std::vector<Pieza*>& otrasPiezas, bool esDestinoOcupado) {
    // 1. Cálculo de distancias y patrón (ya lo tendrás arriba)
    int distX = std::abs(destino.x - posicionTablero.x);
    int distY = std::abs(destino.y - posicionTablero.y);

    if (distX > rangoMovimiento || distY > rangoMovimiento) return false;
    if (distX != 0 && distY != 0) return false; // Patrón Ortogonal

    // 2. VALIDACIÓN DE COLISIONES (Aquí es donde faltaban las definiciones)
    // Calculamos la dirección del paso: 1, -1 o 0
    int stepX = (destino.x > posicionTablero.x) ? 1 : (destino.x < posicionTablero.x ? -1 : 0);
    int stepY = (destino.y > posicionTablero.y) ? 1 : (destino.y < posicionTablero.y ? -1 : 0);

    // 'rev' es nuestra casilla de revisión, empezamos en nuestra posición actual
    sf::Vector2i rev = posicionTablero;

    // Bucle para revisar el camino (sin incluir la casilla de destino final)
    while (rev.x + stepX != destino.x || rev.y + stepY != destino.y) {
        rev.x += stepX;
        rev.y += stepY;

        for (const auto* otra : otrasPiezas) {
            // Usamos el Getter que creamos antes
            if (otra->getPosicionTablero() == rev) {
                return false; // CAMINO BLOQUEADO
            }
        }

        // Seguridad para movimientos de 1 sola casilla (evita bucles infinitos)
        if (distX <= 1 && distY <= 1) break;
    }

    // 3. VALIDACIÓN DEL DESTINO FINAL
    for (const auto* otra : otrasPiezas) {
        if (otra->getPosicionTablero() == destino) {
            if (otra->getBando() == this->bando) {
                return false; // No puedes pisar a un aliado
            }
        }
    }

    return true;
}
void ClaseGolem::procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) {
    if (direccion == sf::Vector2f(0.f, 0.f)) return;

    float velocidad = 250.f;
    sf::Vector2f desplazamiento = direccion * velocidad * dt;
    sf::Vector2f nuevaPos = posicionAbsoluta + desplazamiento;

    // El Golem sabe que NO vuela (false)
    if (arena.esPosicionValida(nuevaPos, 20.f, false)) {
        this->moverEnArena(desplazamiento.x, desplazamiento.y);
    }
}