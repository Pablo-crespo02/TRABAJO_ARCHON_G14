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
    this->rangoMovimiento = 4;
    this->stats.esRango = true;

    // Asignación del patrón de movimiento: ESTRELLA (*)
    this->patronMovimiento = PatronMovimiento::Ambos;
    this->tipoMov = TipoMovimiento::Volador;
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
    // El Fénix no se cura, sino que hace una explosión que ocupa media pantalla
    // No se mueve (velocidad 0), dura medio segundo (0.5), y tiene radio gigante (150.f)
    sf::Vector2f dirFija(0, 0);

    // Reutilizamos tu clase Hitbox para crear el área de daño
    hitboxes.emplace_back(
        this->posicionAbsoluta, // Aparece justo encima del Fénix
        dirFija,
        0,                      // Rapidez 0
        sf::Color(255, 100, 0, 150), // Naranja translúcido
        this,
        15.0f,                  // Daño masivo, aún no hace daño
        0.5,                    // Dura medio segundo
        150.0f                  // Radio enorme
    );
    std::cout << "¡El Fenix desata una Supernova!" << std::endl;
}
