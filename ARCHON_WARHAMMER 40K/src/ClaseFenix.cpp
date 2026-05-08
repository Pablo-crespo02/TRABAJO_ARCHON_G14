/*#include "ClaseFenix.h"

ClaseFenix::ClaseFenix(Bando b, sf::Vector2i pos, std::string tipo)
    : PiezaVoladora(b, pos) // Asumo que el Fénix hereda de PiezaVoladora
{
    this->stats.nombre = tipo;
    this->stats.vida = 25.0f;
    this->stats.vidaMaxima = 25.0f;
    this->stats.defensa = 5.0f;
    this->stats.ataque = 12.0f;
    this->stats.velAtaque = 1.5f;
    this->stats.cooldown = 1.0f;
    this->rangoMovimiento = 4;
    this->stats.esRango = true;
    this->stats.esVolador = true;    // Esto es solo para el HUD

    // Asignación del patrón de movimiento: ESTRELLA (*)
    this->patronMovimiento = PatronMovimiento::Ambos;
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
}*/

#include "ClaseFenix.h"
#include <cmath> 
#include <iostream>

const float PIEZA_ALTURA_TABLERO_FENIX = 90.0f;
const float PIEZA_ALTURA_ARENA_FENIX = 75.0f;

ClaseFenix::ClaseFenix(Bando b, sf::Vector2i pos, std::string tipo)
    : PiezaVoladora(b, pos)
{
    //ESTADÍSTICAS 
    this->stats.nombre = tipo;
    this->stats.vida = 25.0f;
    this->stats.vidaMaxima = 25.0f;
    this->stats.ataque = 12.0f;
    this->stats.defensa = 5.0f;
    this->stats.velAtaque = 1.5f;
    this->stats.cooldown = 1.0f;

    // --- Lógica de tipos ---
    this->stats.esRango = true;    // El Fénix ataca a distancia
    this->stats.esVolador = true;  // Vuela sobre obstáculos

    this->rangoMovimiento = 4;
    this->patronMovimiento = PatronMovimiento::Ambos; // Patrón de estrella

    //CARGA DE SPRITES (Chibi)
    if (tipo == "LIBRARIAN" || tipo == "HARPY") {

        std::string rutaTablero = (tipo == "LIBRARIAN") ? "imagenes/BASE-FENIX-Luz.png" : "imagenes/BASE-MANTICORA-Oscuridad.png";
        std::string rutaArena = (tipo == "LIBRARIAN") ? "imagenes/Chibi-FENIX-Luz-1.0.png" : "imagenes/Chibi-MANTICORA-Oscuridad-1.0.png";

        // se adapta a la tira del Fénix (1 fila, aprox 10 frames de animación y de harpy hay 10
        int columnas = (tipo == "LIBRARIAN") ? 8 : 10;
        int filas = 1;

        if (!texturaTablero.loadFromFile(rutaTablero)) {
            std::cout << "Error: No se encontro " << rutaTablero << std::endl;
        }
        spriteTablero.setTexture(texturaTablero);
        spriteTablero.setOrigin(texturaTablero.getSize().x / 2.0f, texturaTablero.getSize().y / 2.0f);

        float escalaTablero = PIEZA_ALTURA_TABLERO_FENIX / texturaTablero.getSize().y;
        spriteTablero.setScale(escalaTablero, escalaTablero);

        if (!texturaArena.loadFromFile(rutaArena)) {
            std::cout << "Error: No se encontro " << rutaArena << std::endl;
        }
        spriteArena.setTexture(texturaArena);

        anchoFrame = texturaArena.getSize().x / columnas;
        altoFrame = texturaArena.getSize().y / filas;

        spriteArena.setTextureRect(sf::IntRect(0, 0, anchoFrame, altoFrame));
        spriteArena.setOrigin(anchoFrame / 2.0f, altoFrame / 2.0f);

        float escalaArena = PIEZA_ALTURA_ARENA_FENIX / altoFrame;
        if (this->bando == Bando::OSCURIDAD) {
            spriteArena.setScale(-escalaArena, escalaArena);
        }
        else {
            spriteArena.setScale(escalaArena, escalaArena);
        }

        frameActual = 0;
        temporizadorAnimacion = 0.0f;
    }
}

//ENLACE DE FÍSICAS Y ANIMACIÓN
void ClaseFenix::procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) {
    //Dejamos que la clase padre (PiezaVoladora) mueva las coordenadas físicas
    PiezaVoladora::procesarMovimientoArena(direccion, dt, arena);

    //Actualizamos la imagen visible con nuestra máquina de estados
    if (this->stats.nombre == "LIBRARIAN" || this->stats.nombre == "HARPY") {
        animar(dt, direccion);
    }
}

void ClaseFenix::animar(float dt, sf::Vector2f direccion) {
    // Para el Fénix, la fila siempre es 0 porque es una tira horizontal
    int fila = 0;
    int colInicial = 0;
    int colFinal = 0;

    // Leemos el reloj interno. Si hace menos de 0.2 segundos que disparamos, estamos atacando.
    bool estaAtacando = (this->stats.relojProyectil.getElapsedTime().asSeconds() < 0.2f);

    if (estaAtacando) {
        // FOTOGRAMA DE ATAQUE (Frame donde escupe fuego)
        colInicial = 4;
        colFinal = 4;
    }
    else if (direccion.x != 0) {
        // FOTOGRAMA DE VUELO LATERAL (Aleteo estándar)
        colInicial = 2;
        colFinal = 3;
    }
    else if (direccion.y > 0) {
        // FOTOGRAMA DE ABAJO (Aleteo descendente)
        colInicial = 5;
        colFinal = 6;
    }
    else if (direccion.y < 0) {
        // FOTOGRAMA DE ARRIBA (Aleteo ascendente)
        colInicial = 6;
        colFinal = 7;
    }
    else {
        // FOTOGRAMA QUIETO (Posición majestuosa)
        colInicial = 0;
        colFinal = 0;
    }

    int posY_Textura = fila * altoFrame;
    int posY_Actual = spriteArena.getTextureRect().top;

    if (posY_Actual != posY_Textura || frameActual < colInicial || frameActual > colFinal) {
        frameActual = colInicial;
        spriteArena.setTextureRect(sf::IntRect(frameActual * anchoFrame, posY_Textura, anchoFrame, altoFrame));
        temporizadorAnimacion = 0.0f;
    }

    if (colInicial != colFinal) {
        temporizadorAnimacion += dt;
        float velocidadAnimacion = 0.12f; // Un poco más rápido que el Golem para simular aleteo

        if (temporizadorAnimacion >= velocidadAnimacion) {
            temporizadorAnimacion = 0.0f;
            frameActual++;

            if (frameActual > colFinal) {
                frameActual = colInicial;
            }

            spriteArena.setTextureRect(sf::IntRect(frameActual * anchoFrame, posY_Textura, anchoFrame, altoFrame));
        }
    }

    // ARREGLO DEL EFECTO ESPEJO
    float escalaArena = PIEZA_ALTURA_ARENA_FENIX / altoFrame;
    if (direccion.x < 0) {
        spriteArena.setScale(-escalaArena, escalaArena); // Mira a la izquierda
    }
    else if (direccion.x > 0) {
        spriteArena.setScale(escalaArena, escalaArena);  // Mira a la derecha
    }
    else {
        // Si va hacia arriba, abajo, ataca o se queda quieto, respeta la dirección a la que miraba
        float escalaActualX = (spriteArena.getScale().x > 0) ? escalaArena : -escalaArena;
        spriteArena.setScale(escalaActualX, escalaArena);
    }
}

void ClaseFenix::dibujar(sf::RenderWindow& window, Estado estadoActual) {
    if (estadoActual == Estado::Tablero) {
        this->sincronizarPosicionTablero();

        if (this->stats.nombre == "LIBRARIAN" || this->stats.nombre == "HARPY") {

            // CÍRCULO DE SELECCIÓN AMARILLO
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
        if (this->stats.nombre == "LIBRARIAN" || this->stats.nombre == "HARPY") {
            spriteArena.setPosition(posicionAbsoluta);
            window.draw(spriteArena);
        }
        else {
            formaVisual.setPosition(posicionAbsoluta);
            window.draw(formaVisual);
        }

        // DIBUJAMOS BARRA DE VIDA SOBRE LA PIEZA
        barraSalud.actualizar(stats.vida, stats.vidaMaxima, posicionAbsoluta);
        barraSalud.dibujar(window);
    }
}
