#include "ClaseKnight.h"
#include <cmath> 
#include <iostream>

ClaseKnight::ClaseKnight(Bando b, sf::Vector2i pos, std::string tipo)
    : PiezaTerrestre(b, pos)
{
    //ESTADÍSTICAS 
    this->stats.nombre = tipo;
    this->stats.vida = 20.0f;
    this->stats.vidaMaxima = 20.0f; // Ajusta a valores en el futuro
    this->stats.ataque = 6.0f;
    this->stats.defensa = 20.0f;
    this->stats.velAtaque = 1.0f;
    // --- Lógica de tipos ---
    this->stats.esRango = false;    // El caballero es melee

    this->rangoMovimiento = 2;
    this->patronMovimiento = PatronMovimiento::Ortogonal;
    this->tipoMov = TipoMovimiento::Terrestre;  // Solo para el HUD

    //CONFIGURACIÓN DE TAMAÑO SPRITES
    this->piezaAlturaArena = 80;
    this->piezaAlturaTablero = 90;


    //CARGA DE SPRITES (Chibi)
    cargarConfigurarSprites(tipo);

    //REGISTRO DE CLIPS DE ANIMACIÓN EN EL DICCIONARIO:
    if (animador) {
        animador->agreganAnimacion("QUIETO", 0, 0, 0, 0.20f, true);
        animador->agreganAnimacion("CAMINAR_LATERAL", 0, 1, 4, 0.15f, true);
        animador->agreganAnimacion("ATAQUE", 1, 0, 1, 0.15f, true);
        animador->agreganAnimacion("ABAJO", 1, 3, 3, 0.20f, true);
        animador->agreganAnimacion("ARRIBA", 1, 4, 4, 0.20f, true);
    }
}

//ENLACE DE FÍSICAS Y ANIMACIÓN
void ClaseKnight::procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) {
    //Dejamos que la clase padre (PiezaTerrestre) mueva las coordenadas físicas
    PiezaTerrestre::procesarMovimientoArena(direccion, dt, arena);

    //Actualizamos la imagen visible con nuestra máquina de estados
    if (this->stats.nombre == "INTERCESSOR" || this->stats.nombre == "TERMAGANT") {
        animar(dt, direccion);
    }
}

void ClaseKnight::animar(float dt, sf::Vector2f direccion) {
    animador->jugar("ATAQUE");;

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

    actualizarAnimacion(dt);

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

void ClaseKnight::dibujar(sf::RenderWindow& window, Estado estadoActual) {
    if (estadoActual == Estado::Tablero) {
        this->sincronizarPosicionTablero();

        if (this->stats.nombre == "INTERCESSOR" || this->stats.nombre == "TERMAGANT") {

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
        if (this->stats.nombre == "INTERCESSOR" || this->stats.nombre == "TERMAGANT") {
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

    std::cout << this->stats.nombre << " ha lanzado una Granada de Fragmentacion!" << std::endl;
}