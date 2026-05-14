#include "Pieza.h"
#include <iostream>
#include <cmath>

// CONSTRUCTOR BASE
// Ahora es mucho más sencillo y recibe los datos mínimos obligatorios
Pieza::Pieza(Bando b, sf::Vector2i pos) {
    bando = b;
    posicionTablero = pos;
    seleccionado = false;
    hechizoDisponible = true;

    // Inicialización visual básica
    formaVisual.setRadius(20.f);
    formaVisual.setOrigin(20.f, 20.f);

    // Las stats y el rango se llenarán en el constructor de la clase hija
    formaVisual.setFillColor(sf::Color::Magenta);
}

// SINCRONIZACIÓN VISUAL
// Esto es igual para todas las piezas: todas se dibujan en el tablero
void Pieza::sincronizarPosicionTablero() {
    // 60.f es el valor de tu TAMANOCASILLA. 
    // Asegúrate de que este número sea el mismo que usas en el Renderizador.
    float px = (posicionTablero.x * 60.f) + (60.f / 2.f);
    float py = (posicionTablero.y * 60.f) + (60.f / 2.f);

    // Guardamos la posición y movemos el sprite o forma
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

//GESTIÓN DE PROYECTILES:
bool Pieza::puedeAtacar()const {
    // Por ejemplo, un cooldown de 0.5 segundos entre ataques
    float tiempoEspera = 0.5f;

    if (relojAtaque.getElapsedTime().asSeconds() >= tiempoEspera) {
        return true;
    }
    return false;
}
void Pieza::reiniciarRelojHitbox() {
   relojAtaque.restart(); // <--- Este es el nombre real de tu sf::Clock
}
//Gestión de la inmovilización del basilisco (activación y tiempo)
void Pieza::aplicarInmovilizacion(double duracion) {
    inmovilizado = true;
    temporizadorInmovilizacion = duracion;
}
//Gestiona inmovilización e invulnerabilidad 
void Pieza::gestionarEstadosAlterados(double dt) {
    if (inmovilizado) {
        temporizadorInmovilizacion -= dt;
        if (temporizadorInmovilizacion <= 0.0) {
            inmovilizado = false;
            temporizadorInmovilizacion = 0.0;
        }
    }
    if (invulnerable) {
        temporizadorInvulnerabilidad -= dt; // Restamos el tiempo por frame
        if (temporizadorInvulnerabilidad <= 0.0) {
            invulnerable = false;
        }
    }
}
//Invulnerabilidad del unicornio: (la activa y controla el tiempo)
void Pieza::aplicarInvulnerabilidad(double duracion) {
    invulnerable = true;
    temporizadorInvulnerabilidad = duracion;
}