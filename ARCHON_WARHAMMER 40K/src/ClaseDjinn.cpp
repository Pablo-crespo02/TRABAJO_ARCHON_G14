#include "ClaseDjinn.h"
#include <iostream>

ClaseDjinn::ClaseDjinn(Bando b, sf::Vector2i pos, std::string tipo)
    : PiezaTeletransporte(b, pos) // Llama al constructor de la clase intermedia
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
void ClaseDjinn::usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) {
    // Dirección inicial aleatoria para el torbellino
    float anguloInicial = static_cast<float>(std::rand() % 360) * 3.14159f / 180.f;
    sf::Vector2f dirInicial(std::cos(anguloInicial), std::sin(anguloInicial));

    hitboxes.emplace_back(
        this->posicionAbsoluta,      // Origen: la posición del Djinn
        dirInicial,                  // Dirección inicial
        150.0f,                      // Rapidez (se moverá por la arena)
        sf::Color(0, 255, 255, 120), // Color Cian (místico) con transparencia
        this,                        // Propietario
        8.0f,                        // Daño por segundo (DoT)
        10.0f,                       // Dura 10 segundos
        60.0f,                       // Radio del torbellino
        true,                        // Es Daño Continuo (DoT)
        true                         // ¡ES ERRÁTICO!
    );

    std::cout << "¡El Djinn invoca un Torbellino Erratico!" << std::endl;
}