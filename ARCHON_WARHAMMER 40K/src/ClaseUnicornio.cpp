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

    // 4. REGISTRO DE CLIPS DE ANIMACIÓN EN EL DICCIONARIO
    if (animador) {
        animador->agreganAnimacion("QUIETO", 0, 0, 0, 0.20f, true);
        animador->agreganAnimacion("CAMINAR_LATERAL", 0, 1, 4, 0.15f, true);
        animador->agreganAnimacion("ATAQUE", 1, 1, 1, 0.20f, true);
        animador->agreganAnimacion("ABAJO", 1, 3, 3, 0.20f, true);
        animador->agreganAnimacion("ARRIBA", 1, 4, 4, 0.20f, true);
        }
}

//ENLACE DE FÍSICAS Y ANIMACIÓN
void ClaseUnicornio::procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) {
    //Dejamos que la clase padre (PiezaTerrestre) mueva las coordenadas físicas
    PiezaTerrestre::procesarMovimientoArena(direccion, dt, arena);

    //Actualizamos la imagen visible con nuestra máquina de estados
    if (this->stats.nombre == "PRIMARIS" || this->stats.nombre == "TOXICRENO") {
        animar(dt, direccion);
    }
}

void ClaseUnicornio::animar(float dt, sf::Vector2f direccion) {
    if (!animador) return;
   
    // Leemos el reloj interno. Si hace menos de 0.2 segundos que disparamos, estamos atacando.
    bool estaAtacando = (this->stats.relojHitbox.getElapsedTime().asSeconds() < 0.2f);

    if (estaAtacando) {
        animador->jugar("ATAQUE");
    }
    else if (direccion.x != 0) {
        animador->jugar("CAMINAR_LATERAL");
    }
    else if (direccion.y > 0) {
        animador->jugar("ABAJO");
    }
    else if (direccion.y < 0) {
        animador->jugar("ARRIBA");
    }
    else {
        animador->jugar("QUIETO");
    }


    //ARREGLO DEL EFECTO ESPEJO
    float escalaArena = piezaAlturaArena / altoFrame;
    if (direccion.x < 0) {
        spriteArena.setScale(-escalaArena, escalaArena); // Mira a la izquierda
    }
    else if (direccion.x > 0) {
        spriteArena.setScale(escalaArena, escalaArena);  // Mira a la derecha
    }
    else {
        //Si va hacia arriba, abajo, ataca o se queda quieto, respeta la dirección a la que miraba
        float escalaActualX = (spriteArena.getScale().x > 0) ? escalaArena : -escalaArena;
        spriteArena.setScale(escalaActualX, escalaArena);
    }
}

void ClaseUnicornio::dibujar(sf::RenderWindow& window, Estado estadoActual) {
    if (estadoActual == Estado::Tablero) {
        this->sincronizarPosicionTablero();

        if (this->stats.nombre == "FALTA" || this->stats.nombre == "TOXICRENO") {

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
        else {
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
    }
    else if (estadoActual == Estado::Arena) {
        if (this->stats.nombre == "FALTA" || this->stats.nombre == "TOXICRENO") {
            spriteArena.setPosition(posicionAbsoluta);
            window.draw(spriteArena);
        }
        else {
            formaVisual.setPosition(posicionAbsoluta);
            window.draw(formaVisual);
        }

        //DIBUJAMOS BARRA DE VIDA SOBRE LA PIEZA
        barrasArena.actualizar(stats.vida, stats.vidaMaxima, stats.velAtaque, posicionAbsoluta);
        barrasArena.dibujar(window);

    }
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