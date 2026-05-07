#include "ClaseGolem.h"

ClaseGolem::ClaseGolem(Bando b, sf::Vector2i pos, std::string tipo)
    : PiezaTerrestre(b, pos) // Llama al constructor de la clase intermedia
{
    this->stats.nombre = tipo;
    this->stats.vida = 2.0f;
    this->stats.ataque = 5.0f;
    this->stats.defensa = 8.0f;
    this->rangoMovimiento = 2;
    this->stats.esRango = false;
    this->stats.tiempoRecarga = 1;  //Valor provisional

    // Asignación del patrón de movimiento
    this->patronMovimiento = PatronMovimiento::Ambos;

    if (b == Bando::LUZ) { ultimadireccion = sf::Vector2f(1, 0); }   //Inicializa la pieza mirando hacia la derecha en la arena, a efectos de los proyectiles
    else { ultimadireccion = sf::Vector2f(-1, 0); } //Inicializa la pieza mirando hacia la izquieda en la arena, a efectos de los proyectiles
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
    }
    window.draw(formaVisual);
}

