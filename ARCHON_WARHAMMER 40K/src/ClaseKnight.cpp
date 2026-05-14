#include "ClaseKnight.h"
#include <cmath>
#include <iostream>

ClaseKnight::ClaseKnight(Bando b, sf::Vector2i pos, std::string tipo)
    : PiezaTerrestre(b, pos)
{
    // ESTADÍSTICAS 
    this->stats.nombre = tipo;
    this->stats.vida = 20.0f;
    this->stats.vidaMaxima = 20.0f;
    this->stats.ataque = 8.0f;
    this->stats.defensa = 12.0f;
    this->stats.velAtaque = 0.8f;

    // --- Lógica de tipos ---
    this->stats.esRango = true;

    this->rangoMovimiento = 3;
    this->patronMovimiento = PatronMovimiento::Ambos;
    this->tipoMov = TipoMovimiento::Terrestre;

    // Aseguramos dimensiones iniciales de la forma por si la clase base falla
    formaVisual.setRadius(20.f);
    formaVisual.setOrigin(20.f, 20.f);
}

void ClaseKnight::procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) {
    PiezaTerrestre::procesarMovimientoArena(direccion, dt, arena);
}

void ClaseKnight::dibujar(sf::RenderWindow& window, Estado estadoActual) {
    if (estadoActual == Estado::Tablero) {
        this->sincronizarPosicionTablero();

        formaVisual.setPosition(posicionAbsoluta);
        formaVisual.setFillColor(bando == Bando::LUZ ? Colores::ColorFichaLuz : Colores::ColorFichaOscuridad);

        if (seleccionado) {
            formaVisual.setOutlineThickness(4.0f);
            formaVisual.setOutlineColor(Colores::ColorOutlineSeleccion);
        }
        else {
            formaVisual.setOutlineThickness(0.0f);
        }

        window.draw(formaVisual);
    }
    else if (estadoActual == Estado::Arena) {
        formaVisual.setPosition(posicionAbsoluta);
        formaVisual.setOutlineThickness(0.0f);
        formaVisual.setOrigin(20.f, 20.f);

        window.draw(formaVisual);

        barrasArena.actualizar(stats.vida, stats.vidaMaxima, stats.velAtaque, posicionAbsoluta);
        barrasArena.dibujar(window);
    }
}

void ClaseKnight::usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) {
    sf::Vector2f dirAtaque = this->getultimadireccion();
    float magnitud = std::hypot(dirAtaque.x, dirAtaque.y);
    dirAtaque = (magnitud != 0) ? (dirAtaque / magnitud) : sf::Vector2f(1.f, 0.f);

    sf::Vector2f puntoSpawn = this->posicionAbsoluta + (dirAtaque * 35.f);

    double rapidez = 400.0;
    double danoBase = 10.0;
    double tiempoDeVidaTotal = 2.5;
    double radioProyectil = 10.0;
    double radioDeLaExplosion = 120.0;
    hitboxes.emplace_back(
        puntoSpawn, dirAtaque, rapidez, sf::Color(100, 100, 100), this,
        (danoBase * this->multiplicadorArena), tiempoDeVidaTotal, radioProyectil,
        false, false, false, 0.0,
        true, radioDeLaExplosion
    );

    std::cout << "¡" << this->stats.nombre << " ha lanzado una Granada de Fragmentacion!" << std::endl;
}