#include "ClaseUnicornio.h"
#include <iostream>

ClaseUnicornio::ClaseUnicornio(Bando b, sf::Vector2i pos, std::string tipo)
    : PiezaTerrestre(b, pos)
{
    this->stats.nombre = tipo;
    this->patronMovimiento = PatronMovimiento::Diagonal;

    // Estadísticas
    this->stats.vida = 7.0f;
    this->stats.vidaMaxima = 7.0f; // Asegúrate de inicializar vidaMaxima
    this->stats.ataque = 8.0f;
    this->stats.defensa = 5.0f;
    this->rangoMovimiento = 4;
    this->stats.velAtaque = 0.5f;
    this->stats.esRango = true;
}
//Gestiona dt y permite que el escudo se apague tras 5 segundos
void ClaseUnicornio::procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) {
    // Actualizamos el cronómetro del escudo y parálisis
    this->gestionarEstadosAlterados(static_cast<double>(dt));

    // Si está inmovilizado, no se mueve
    if (this->getInmovilizado()) return;

    // Ejecutamos el movimiento normal heredado
    PiezaTerrestre::procesarMovimientoArena(direccion, dt, arena);
}

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

        // Dibujo del Halo (Escudo)
        if (this->getInvulnerable()) {
            double radioDbl = 35.0;
            sf::CircleShape halo(static_cast<float>(radioDbl));
            halo.setOrigin(static_cast<float>(radioDbl), static_cast<float>(radioDbl));
            halo.setPosition(posicionAbsoluta);

            halo.setFillColor(sf::Color(255, 215, 0, 80));
            halo.setOutlineColor(sf::Color(255, 255, 255, 180));
            halo.setOutlineThickness(2.0f);
            window.draw(halo);
        }

    //Barra de salud
        barraSalud.actualizar(stats.vida, stats.vidaMaxima, posicionAbsoluta);
        barraSalud.dibujar(window);
    }

    window.draw(formaVisual);
}

void ClaseUnicornio::usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) {
    if (this->bando == Bando::LUZ) {
        this->aplicarInvulnerabilidad(5.0);
        std::cout << "¡El Unicornio alza un escudo de luz impenetrable!" << std::endl;
    }
    else if (this->bando == Bando::OSCURIDAD) {
        sf::Vector2f dirAtaque = this->getultimadireccion();
        float magnitud = std::hypot(dirAtaque.x, dirAtaque.y);
        dirAtaque = (magnitud != 0) ? (dirAtaque / magnitud) : sf::Vector2f(-1, 0);

        sf::Vector2f puntoSpawn = this->posicionAbsoluta + (dirAtaque * 35.f);

        hitboxes.emplace_back(
            puntoSpawn, dirAtaque, 900.0, sf::Color(148, 0, 211), this,
            1.0, 1.5, 12.0, false, false, true, 3.0
        );
        std::cout << "¡El Basilisco dispara un proyectil paralizante!" << std::endl;
    }
}