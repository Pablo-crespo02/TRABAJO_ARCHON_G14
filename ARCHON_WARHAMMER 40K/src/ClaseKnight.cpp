#include "ClaseKnight.h"

ClaseKnight::ClaseKnight(Bando b, sf::Vector2i pos, std::string tipo)
    : PiezaTerrestre(b, pos) // Llama al constructor de la clase intermedia
{
    this->stats.nombre = tipo;
    this->stats.vida = 2.0f;
    this->stats.ataque = 5.0f;
    this->stats.defensa = 8.0f;
    this->rangoMovimiento = 3;
    this->stats.esRango = true;

    // Asignación del patrón de movimiento
    this->patronMovimiento = PatronMovimiento::Ambos;
}

// Aquí NO deben ir las funciones de movimiento. Ya están en PiezaTeletransporte.cpp.

void ClaseKnight::dibujar(sf::RenderWindow& window, Estado estadoActual) {
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
