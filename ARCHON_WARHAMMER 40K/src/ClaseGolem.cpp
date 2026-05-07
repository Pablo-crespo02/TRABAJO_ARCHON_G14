#include "ClaseGolem.h"

ClaseGolem::ClaseGolem(Bando b, sf::Vector2i pos, std::string tipo)
    : PiezaTerrestre(b, pos)
{
    this->stats.nombre = tipo;
    this->stats.vida = 30.0f;
    this->stats.vidaMaxima = 30.0f;    // Ajusta a valores en el futuro
    this->stats.ataque = 6.0f;
    this->stats.defensa = 20.0f;
    this->stats.velAtaque = 1.0f;
    this->stats.cooldown = 2.0f;

    // --- Lógica de tipos ---
    this->stats.esRango = false;    // El Golem es mele
    this->stats.esVolador = false; // Solo para el HUD

    this->rangoMovimiento = 2;
    this->patronMovimiento = PatronMovimiento::Ortogonal;
}
// Aquí NO deben ir las funciones de movimiento. Ya están en PiezaTeletransporte.cpp.

void ClaseGolem::dibujar(sf::RenderWindow& window, Estado estadoActual) {
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

