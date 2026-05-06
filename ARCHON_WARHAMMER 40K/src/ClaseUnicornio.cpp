#include "ClaseUnicornio.h"

ClaseUnicornio::ClaseUnicornio(Bando b, sf::Vector2i pos, std::string tipo)
<<<<<<< HEAD
    : PiezaTerrestre(b, pos)
=======
    : PiezaTerrestre(b, pos) // Llama al constructor de la clase intermedia
>>>>>>> ae9d14edd8fb2b3f35330778e2828e95dd315d36
{
    this->stats.nombre = tipo;
<<<<<<< HEAD
    this->patronMovimiento = PatronMovimiento::Diagonal;
    // Estadísticas Arquetipo Fénix 
    this->stats.vida = 7.0f;
    this->stats.ataque = 8.0f;
    this->stats.defensa = 5.0f;
    this->rangoMovimiento = 4;
    this->stats.esRango = false;

    // Personalización para diferencias Librarian o Harpy 
    if (tipo == "Primaris") {

    }
=======
    this->stats.vida = 2.0f;
    this->stats.ataque = 5.0f;
    this->stats.defensa = 8.0f;
    this->rangoMovimiento = 3;
    this->stats.esRango = true;

    // Asignación del patrón de movimiento
    this->patronMovimiento = PatronMovimiento::Ambos;
>>>>>>> ae9d14edd8fb2b3f35330778e2828e95dd315d36
}

// Aquí NO deben ir las funciones de movimiento. Ya están en PiezaTeletransporte.cpp.

void ClaseUnicornio::dibujar(sf::RenderWindow& window, Estado estadoActual) {
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