#include "ClaseDjinn.h"

ClaseDjinn::ClaseDjinn(Bando b, sf::Vector2i pos, std::string tipo)
    : PiezaTeletransporte(b, pos) // Llama al constructor de la clase intermedia
{
    this->stats.nombre = tipo;
    this->stats.vida = 18.0f;
    this->stats.vidaMaxima = 18.0f;
    this->stats.defensa = 3.0f;
    this->stats.ataque = 15.0f;
    this->stats.velAtaque = 1.2f;
    this->rangoMovimiento = 4;
    this->stats.esRango = true;

    // Asignación del patrón de movimiento
    this->patronMovimiento = PatronMovimiento::Ambos;
    this->tipoMov = TipoMovimiento::Teletransporte;
}

// Aquí NO deben ir las funciones de movimiento. Ya están en PiezaTeletransporte.cpp.

void ClaseDjinn::dibujar(sf::RenderWindow& window, Estado estadoActual) {
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