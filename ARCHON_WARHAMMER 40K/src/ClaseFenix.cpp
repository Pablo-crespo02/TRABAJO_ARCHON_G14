#include "ClaseFenix.h"
#include <cmath> 
#include <iostream>

const float PIEZA_ALTURA_TABLERO = 90.0f;
const float PIEZA_ALTURA_ARENA = 180.0f;

ClaseFenix::ClaseFenix(Bando b, sf::Vector2i pos, std::string tipo)
    : PiezaVoladora(b, pos)
{
    //ESTADÍSTICAS 
    this->stats.nombre = tipo;
    this->stats.vida = 25.0f;
    this->stats.vidaMaxima = 25.0f; // Ajusta a valores en el futuro
    this->stats.ataque = 12.0f;
    this->stats.defensa = 5.0f;
    this->stats.velAtaque = 1.5f;
    // --- Lógica de tipos ---
    this->stats.esRango = true;

    this->rangoMovimiento = 4;
    this->patronMovimiento = PatronMovimiento::Ambos;
    this->tipoMov = TipoMovimiento::Volador;  // Solo para el HUD

    //CARGA DE SPRITES
    if (tipo == "LIBRARIAN" || tipo == "HARPY") {

        std::string rutaTablero = (tipo == "LIBRARIAN") ? "imagenes/BASE-LIBRARIAN-Humanidad.png" : "imagenes/BASE-CARNIFEX-TYRANIDS.png";
        std::string rutaArena = (tipo == "LIBRARIAN") ? "imagenes/Chibi-LIBRARIAN-Humanidad-1.0.png" : "imagenes/Chibi-CARNIFEX-TYRANIDS-1.0.png";
        int columnas = 5;
        int filas = 2;

        if (!texturaTablero.loadFromFile(rutaTablero)) {
            std::cout << "Error: No se encontro " << rutaTablero << std::endl;
        }
        spriteTablero.setTexture(texturaTablero);
        spriteTablero.setOrigin(texturaTablero.getSize().x / 2.0f, texturaTablero.getSize().y / 2.0f);

        float escalaTablero = PIEZA_ALTURA_TABLERO / texturaTablero.getSize().y;
        spriteTablero.setScale(escalaTablero, escalaTablero);

        if (!texturaArena.loadFromFile(rutaArena)) {
            std::cout << "Error: No se encontro " << rutaArena << std::endl;
        }
        spriteArena.setTexture(texturaArena);

        anchoFrame = texturaArena.getSize().x / columnas;
        altoFrame = texturaArena.getSize().y / filas;

        spriteArena.setTextureRect(sf::IntRect(0, 0, anchoFrame, altoFrame));
        spriteArena.setOrigin(anchoFrame / 2.0f, altoFrame / 2.0f);

        float escalaArena = PIEZA_ALTURA_ARENA / altoFrame;
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
    int fila = 0;
    int colInicial = 0;
    int colFinal = 0;

    // Leemos el reloj interno. Si hace menos de 0.2 segundos que disparamos, estamos atacando.
    bool estaAtacando = (this->stats.relojHitbox.getElapsedTime().asSeconds() < 0.2f);

    if (estaAtacando) {
        //FOTOGRAMA DE ATAQUE 
        fila = 1;
        colInicial = 1;
        colFinal = 1;
    }
    else if (direccion.x != 0) {
        //FOTOGRAMA DE CAMINAR LATERAL 
        fila = 0;
        colInicial = 1;
        colFinal = 4;
    }
    else if (direccion.y > 0) {
        //FOTOGRAMA DE ABAJO 
        fila = 1;
        colInicial = 3;
        colFinal = 3;
    }
    else if (direccion.y < 0) {
        // FOTOGRAMA DE ARRIBA 
        fila = 1;
        colInicial = 4;
        colFinal = 4;
    }
    else {
        //FOTOGRAMA QUIETO
        fila = 0;
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
        float velocidadAnimacion = 0.15f;

        if (temporizadorAnimacion >= velocidadAnimacion) {
            temporizadorAnimacion = 0.0f;
            frameActual++;

            if (frameActual > colFinal) {
                frameActual = colInicial;
            }

            spriteArena.setTextureRect(sf::IntRect(frameActual * anchoFrame, posY_Textura, anchoFrame, altoFrame));
        }
    }

    //ARREGLO DEL EFECTO ESPEJO
    float escalaArena = PIEZA_ALTURA_ARENA / altoFrame;
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