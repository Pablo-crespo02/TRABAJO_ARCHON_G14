#include "ClaseFenix.h"

ClaseFenix::ClaseFenix(Bando b, sf::Vector2i pos, std::string tipo)
    : PiezaVoladora(b, pos) // Asumo que el Fénix hereda de PiezaVoladora
{
    this->stats.nombre = tipo;
    this->stats.vida = 25.0f;
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
    }
    window.draw(formaVisual);
}

