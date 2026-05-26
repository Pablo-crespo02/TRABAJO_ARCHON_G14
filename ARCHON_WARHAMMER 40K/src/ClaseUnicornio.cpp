#include "ClaseUnicornio.h"
#include <iostream>
#include <cmath> 

ClaseUnicornio::ClaseUnicornio(Bando b, sf::Vector2i pos, std::string tipo)
    : PiezaTerrestre(b, pos)
{
    //ESTADÍSTICAS 
    this->stats.nombre = tipo;
    this->stats.vida = 25.0f;
    this->stats.vidaMaxima = 25.0f; // Ajusta a valores en el futuro
    this->stats.ataque = 10.0f;
    this->stats.defensa = 15.0f;
    this->stats.velAtaque = 1.0f;
    // --- Lógica de tipos ---
    this->stats.esRango = false;    // El Golem es melee

    this->rangoMovimiento = 3;
    this->patronMovimiento = PatronMovimiento::Ambos;
    this->tipoMov = TipoMovimiento::Terrestre;  // Solo para el HUD

    // CONFIGURACIÓN DE TAMAÑOS SPRITES:
    this->piezaAlturaTablero = 90;
    this->piezaAlturaArena = 120;

    //CARGA DE SPRITES (Chibi)
    cargarConfigurarSprites(tipo);
}

//ENLACE DE FÍSICAS Y ANIMACIÓN
void ClaseUnicornio::procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) {
    //Dejamos que la clase padre (PiezaTerrestre) mueva las coordenadas físicas
    PiezaTerrestre::procesarMovimientoArena(direccion, dt, arena);

    //Actualizamos la imagen visible con nuestra máquina de estados
    if (this->stats.nombre == "PRIMARIS" || this->stats.nombre == "TOXICRENO") {
        Pieza::Animar(dt, direccion);
    }
}

void ClaseUnicornio::dibujar(sf::RenderWindow& window, Estado estadoActual) {
    if (estadoActual == Estado::Tablero) {
        this->sincronizarPosicionTablero();

        if (this->stats.nombre == "PRIMARIS" || this->stats.nombre == "TOXICRENO") {

            //CÍRCULO DE SELECCIÓN AMARILLO
            if (seleccionado) {
                sf::CircleShape anilloSeleccion(25.f);
                anilloSeleccion.setOrigin(25.f, 25.f);
                anilloSeleccion.setPosition(posicionAbsoluta);
                anilloSeleccion.setFillColor(sf::Color::Transparent); // Fondo vacío
                anilloSeleccion.setOutlineThickness(4.f);             // Borde grueso
                anilloSeleccion.setOutlineColor(Colores::ColorOutlineSeleccion); // Amarillo
                window.draw(anilloSeleccion);
            }

            spriteTablero.setPosition(posicionAbsoluta);
            window.draw(spriteTablero);
        }
    
    }
    else if (estadoActual == Estado::Arena) {
        if (this->stats.nombre == "PRIMARIS" || this->stats.nombre == "TOXICRENO") {
            spriteArena.setPosition(posicionAbsoluta);
            window.draw(spriteArena);
        }
       

        //DIBUJAMOS BARRA DE VIDA SOBRE LA PIEZA
        barrasArena.actualizar(stats.vida, stats.vidaMaxima, stats.velAtaque, posicionAbsoluta);
        barrasArena.dibujar(window);

    }
}

void ClaseUnicornio::usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) {
    if (this->bando == Bando::LUZ) {
        this->aplicarInvulnerabilidad(5.0);
        std::cout << "El Unicornio alza un escudo de luz impenetrable" << std::endl;
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