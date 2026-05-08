#include "ClaseFenix.h"
#include <iostream>

ClaseFenix::ClaseFenix(Bando b, sf::Vector2i pos, std::string tipo)
    : PiezaVoladora(b, pos) // Asumo que el Fénix hereda de PiezaVoladora
{
    this->stats.nombre = tipo;
    this->stats.vida = 25.0f;  
    this->stats.vidaMaxima = 25.0f;
    this->stats.defensa = 5.0f;
    this->stats.ataque = 12.0f;
    this->stats.velAtaque = 1.5f;
    this->stats.cooldown = 1.0f;
    this->rangoMovimiento = 4;
    this->stats.esRango = true;
    this->stats.esVolador = true;    // Esto es solo para el HUD

    // Asignación del patrón de movimiento: ESTRELLA (*)
    this->patronMovimiento = PatronMovimiento::Ambos;
}

// Aquí NO deben ir las funciones de movimiento. Ya están en PiezaTeletransporte.cpp.

void ClaseFenix::dibujar(sf::RenderWindow& window, Estado estadoActual) {
    if (estadoActual == Estado::Tablero) {
        this->sincronizarPosicionTablero();
        formaVisual.setFillColor(bando == Bando::LUZ ? Colores::ColorFichaLuz : Colores::ColorFichaOscuridad);

        if (seleccionado) {
            formaVisual.setOutlineThickness(4.0f);
            formaVisual.setOutlineColor(Colores::ColorOutlineSeleccion);
        }
        else {
            formaVisual.setOutlineThickness(0.0f);
        }
    }
    else if (estadoActual == Estado::Arena) {
        formaVisual.setPosition(posicionAbsoluta);
        formaVisual.setOutlineThickness(0.0f);
        formaVisual.setOrigin(20.f, 20.f);
        barraSalud.actualizar(stats.vida, stats.vidaMaxima, posicionAbsoluta);
        barraSalud.dibujar(window);
    }
    window.draw(formaVisual);
}
void ClaseFenix::usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) {
    sf::Vector2f dirFija(0.f, 0.f);

    hitboxes.emplace_back(
        this->posicionAbsoluta,      // Se crea en la posición actual de la pieza
        dirFija,                     // No se mueve
        0,                           // Rapidez 0
        sf::Color(255, 69, 0, 150),  // Naranja Fuego semitransparente
        this,
        10.0f,                       // Hace 10 de daño POR SEGUNDO
        5.0f,                        // Dura 5 segundos exactos en la arena
        100.0f,                      // Radio de la explosión
        true                         // <--- ACTIVAMOS EL MODO ZONA DE FUEGO (DoT)
    );
}
