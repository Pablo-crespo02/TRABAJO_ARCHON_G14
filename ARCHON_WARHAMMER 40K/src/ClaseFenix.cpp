#include "ClaseFenix.h"
#include <cmath> 
#include <iostream>

const float PIEZA_ALTURA_TABLERO = 90.0f;
const float PIEZA_ALTURA_ARENA = 150.0f;

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

    this->rangoMovimiento = 4;
    this->hechizoDisponible = true;
    this->patronMovimiento = PatronMovimiento::Ambos;
    this->tipoMov = TipoMovimiento::Volador;

    // Inicializamos el puntero por seguridad
    this->enemigoEnlazado = nullptr;

    // CARGA DE SPRITES
    if (tipo == "LIBRARIAN" || tipo == "HARPY") {
        std::string rutaTablero = (tipo == "LIBRARIAN") ? "imagenes/BASE-LIBRARIAN-Humanidad.png" : "imagenes/BASE-HARPY-TYRANIDS.png";
        std::string rutaArena = (tipo == "LIBRARIAN") ? "imagenes/Chibi-LIBRARIAN-Humanidad-1.0.png" : "imagenes/Chibi-HARPY-TYRANIDS-1.0.png";

        int columnas = 5;
        int filas = 2;

        if (!texturaTablero.loadFromFile(rutaTablero)) {
            std::cout << "Error: No se encontro " << rutaTablero << std::endl;
        }
        else {
            spriteTablero.setTexture(texturaTablero);
            spriteTablero.setOrigin(texturaTablero.getSize().x / 2.0f, texturaTablero.getSize().y / 2.0f);

            float escalaTablero = PIEZA_ALTURA_TABLERO / texturaTablero.getSize().y;
            spriteTablero.setScale(escalaTablero, escalaTablero);
        }
        
        if (!texturaArena.loadFromFile(rutaArena)) {
            std::cout << "Error: No se encontro " << rutaArena << std::endl;
        }
        else {
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
        }
    

        frameActual = 0;
        temporizadorAnimacion = 0.0f;
    }
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

    bool estaAtacando = (this->stats.relojHitbox.getElapsedTime().asSeconds() < 0.2f);

    if (estaAtacando) {
        fila = 1; colInicial = 1; colFinal = 1;
    }
    else if (direccion.x != 0) {
        fila = 0; colInicial = 1; colFinal = 4;
    }
    else if (direccion.y > 0) {
        fila = 1; colInicial = 3; colFinal = 3;
    }
    else if (direccion.y < 0) {
        fila = 1; colInicial = 4; colFinal = 4;
    }
    else {
        fila = 0; colInicial = 0; colFinal = 0;
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

    float escalaArena = PIEZA_ALTURA_ARENA / altoFrame;
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

// =========================================================================
// INTERFAZ DE DIBUJADO (Soporta el renderizado autónomo del tentáculo)
// =========================================================================
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
            if (seleccionado) { formaVisual.setOutlineThickness(4.0f); formaVisual.setOutlineColor(Colores::ColorOutlineSeleccion); }
            else { formaVisual.setOutlineThickness(0.0f); }
            window.draw(formaVisual);
        }
    }
    else if (estadoActual == Estado::Arena) {

        // --- RENDERIZADO DEL TENTÁCULO GRUESO
        if (this->stats.nombre == "HARPY" && enemigoEnlazado) {
            float tiempoTranscurrido = this->stats.relojHabilidad.getElapsedTime().asSeconds();
            if (tiempoTranscurrido <= 10.0f) {
                sf::Vector2f posHarpy = this->getPosicionAbsoluta();
                sf::Vector2f posEnemigo = enemigoEnlazado->getPosicionAbsoluta();

                float dx = posEnemigo.x - posHarpy.x;
                float dy = posEnemigo.y - posHarpy.y;
                float distancia = std::hypot(dx, dy);

                if (distancia <= 160.f) {
                    // Configuramos el grosor de la línea roja (puedes subirlo si lo quieres más ancho)
                    float grosorRayo = 6.f;

                    sf::RectangleShape rayoEnergia(sf::Vector2f(distancia, grosorRayo));

                    // Centramos el origen en Y para que salga exactamente desde el centro de los personajes
                    rayoEnergia.setOrigin(0.f, grosorRayo / 2.f);
                    rayoEnergia.setPosition(posHarpy);

                    // Calculamos el ángulo para que apunte directamente al enemigo
                    float angulo = std::atan2(dy, dx) * 180.f / 3.14159265f;
                    rayoEnergia.setRotation(angulo);

                    // Rojo puro y sólido de SFML, sin mezclas
                    rayoEnergia.setFillColor(sf::Color::Red);

                    // Dibujamos el rayo en la ventana
                    window.draw(rayoEnergia);
                }
            }
        }

        // Pintado tradicional de la pieza
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

// =========================================================================
// ACTIVACIÓN DEL HECHIZO
// =========================================================================
void ClaseFenix::usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) {
    // 1. Lógica del LIBRARIAN: No depende del enemigo
    if (this->stats.nombre == "LIBRARIAN") {
        sf::Vector2f dirFija(0, 0);
        hitboxes.emplace_back(
            this->posicionAbsoluta,
            dirFija,
            0,
            sf::Color(255, 69, 0, 150),
            this,
            10.0f, // Daño
            5.0f,  // Tiempo de vida
            150.0f,// Radio
            true   // ¿Es área?
        );

        // ¡IMPORTANTE! Reiniciamos el reloj para que el cooldown actúe
        this->stats.relojHabilidad.restart();
        std::cout << "El Librarian desata una Supernova!" << std::endl;
    }
    // 2. Lógica de la HARPY: Sí depende del enemigo
    else if (this->stats.nombre == "HARPY") {
        if (!enemigo) return; // Solo devolvemos si la Harpy necesita al enemigo y no hay

        this->enemigoEnlazado = enemigo;
        this->stats.relojHabilidad.restart();
        std::cout << "El Harpy activa Enlace de Sangre!" << std::endl;
    }
}
// =========================================================================
// ACTUALIZACIÓN DE VIDA CONTINUA
// =========================================================================
void ClaseFenix::actualizarLogicaHechizo(float dt) {
    if (this->stats.nombre != "HARPY" || !enemigoEnlazado) return;

    // Control de tiempo: límite de 10 segundos
    float tiempoTranscurrido = this->stats.relojHabilidad.getElapsedTime().asSeconds();
    if (tiempoTranscurrido > 10.0f) {
        enemigoEnlazado = nullptr; // Rompemos el enlace
        return;
    }

    // Control de distancia máxima
    sf::Vector2f posHarpy = this->getPosicionAbsoluta();
    sf::Vector2f posEnemigo = enemigoEnlazado->getPosicionAbsoluta();
    float distancia = std::hypot(posEnemigo.x - posHarpy.x, posEnemigo.y - posHarpy.y);

    float rangoMaximoDrenaje = 160.f;

    if (distancia <= rangoMaximoDrenaje) {
        float danoPorSegundo = 2.0f;
        float curaPorSegundo = 1.5f;

        if (!enemigoEnlazado->getInvulnerable()) {
            enemigoEnlazado->stats.vida -= danoPorSegundo * dt;
        }
        this->stats.vida = std::min(this->stats.vidaMaxima, this->stats.vida + (curaPorSegundo * dt));
    }
}