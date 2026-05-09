#include "ClaseFenix.h"
#include <cmath> 
#include <iostream>

const float PIEZA_ALTURA_TABLERO_FENIX = 90.0f;
const float PIEZA_ALTURA_ARENA_FENIX = 75.0f;

ClaseFenix::ClaseFenix(Bando b, sf::Vector2i pos, std::string tipo)
    : PiezaVoladora(b, pos)
{
    // ESTADÍSTICAS 
    this->stats.nombre = tipo;
    this->stats.vida = 25.0f;
    this->stats.vidaMaxima = 25.0f;
    this->stats.ataque = 12.0f;
    this->stats.defensa = 5.0f;
    this->stats.velAtaque = 1.5f;


    this->stats.esRango = true;
    this->tipoMov = TipoMovimiento::Volador;
    this->rangoMovimiento = 4;
    this->patronMovimiento = PatronMovimiento::Ambos;

    // CARGA DE SPRITES
    if (tipo == "LIBRARIAN" || tipo == "HARPY") {
        std::string rutaTablero = (tipo == "LIBRARIAN") ? "imagenes/BASE-FENIX-Luz.png" : "imagenes/BASE-MANTICORA-Oscuridad.png";
        std::string rutaArena = (tipo == "LIBRARIAN") ? "imagenes/Chibi-FENIX-Luz-1.0.png" : "imagenes/Chibi-MANTICORA-Oscuridad-1.0.png";

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

void ClaseFenix::usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) {
    sf::Vector2f dirFija(0, 0);

    // Reutilizamos la clase Hitbox para crear el área de daño continuo
    hitboxes.emplace_back(
        this->posicionAbsoluta,
        dirFija,
        0,
        sf::Color(255, 69, 0, 150), // Color Naranja Fuego
        this,
        10.0f,                  // Daño por segundo
        5.0f,                   // Duración en la arena
        150.0f,                 // Radio enorme
        true                    // Daño continuo
    );
    std::cout << "¡El Fenix desata una Supernova!" << std::endl;
}

void ClaseFenix::procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) {
    PiezaVoladora::procesarMovimientoArena(direccion, dt, arena);

    if (this->stats.nombre == "LIBRARIAN" || this->stats.nombre == "HARPY") {
        animar(dt, direccion);
    }
}

void ClaseFenix::animar(float dt, sf::Vector2f direccion) {
    int fila = 0;
    int colInicial = 0;
    int colFinal = 0;

    // Si da error en relojProyectil, asegúrate de haberlo añadido a Pieza.h
    bool estaAtacando = (this->stats.relojProyectil.getElapsedTime().asSeconds() < 0.2f);

    if (estaAtacando) {
        colInicial = 4; colFinal = 4;
    }
    else if (direccion.x != 0) {
        colInicial = 2; colFinal = 3;
    }
    else if (direccion.y > 0) {
        colInicial = 5; colFinal = 6;
    }
    else if (direccion.y < 0) {
        colInicial = 6; colFinal = 7;
    }
    else {
        colInicial = 0; colFinal = 0;
    }

    int posY_Textura = fila * altoFrame;
    int posY_Actual = spriteArena.getTextureRect().top;

    if (posY_Actual != posY_Textura || frameActual < colInicial || frameActual > colFinal) {
        frameActual = colInicial;
        spriteArena.setTextureRect(sf::IntRect((frameActual * anchoFrame) + 1, posY_Textura, anchoFrame - 2, altoFrame));
        temporizadorAnimacion = 0.0f;
    }

    if (colInicial != colFinal) {
        temporizadorAnimacion += dt;
        if (temporizadorAnimacion >= 0.12f) {
            temporizadorAnimacion = 0.0f;
            frameActual++;
            if (frameActual > colFinal) frameActual = colInicial;
            spriteArena.setTextureRect(sf::IntRect((frameActual * anchoFrame) + 1, posY_Textura, anchoFrame - 2, altoFrame));
        }
    }

    float escalaArena = PIEZA_ALTURA_ARENA_FENIX / altoFrame;
    if (direccion.x < 0) {
        spriteArena.setScale(-escalaArena, escalaArena);
    }
    else if (direccion.x > 0) {
        spriteArena.setScale(escalaArena, escalaArena);
    }
    else {
        float escalaActualX = (spriteArena.getScale().x > 0) ? escalaArena : -escalaArena;
        spriteArena.setScale(escalaActualX, escalaArena);
    }
}

void ClaseFenix::dibujar(sf::RenderWindow& window, Estado estadoActual) {
    if (estadoActual == Estado::Tablero) {
        this->sincronizarPosicionTablero();
        if (this->stats.nombre == "LIBRARIAN" || this->stats.nombre == "HARPY") {
            if (seleccionado) {
                sf::CircleShape anilloSeleccion(25.f);
                anilloSeleccion.setOrigin(25.f, 25.f);
                anilloSeleccion.setPosition(posicionAbsoluta);
                anilloSeleccion.setFillColor(sf::Color::Transparent);
                anilloSeleccion.setOutlineThickness(4.f);
                anilloSeleccion.setOutlineColor(Colores::ColorOutlineSeleccion);
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
        barrasArena.actualizar(stats.vida, stats.vidaMaxima, stats.velAtaque, posicionAbsoluta);
        barrasArena.dibujar(window);
    }
}