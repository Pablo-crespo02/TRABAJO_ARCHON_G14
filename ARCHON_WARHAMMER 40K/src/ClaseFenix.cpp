#include "ClaseFenix.h"
#include <cmath> 
#include <iostream>

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

    //Tamaño de los sprites:
    this->piezaAlturaArena = 150;
    this->piezaAlturaTablero = 90;

    // CARGA DE SPRITES
    cargarConfigurarSprites(tipo);

    //REGISTRO DE ANIMACIONES EN EL DICCIONARIO
    if (animador) {
        animador->agreganAnimacion("QUIETO", 0, 0, 0, 0.20f, true);
        animador->agreganAnimacion("CAMINAR_LATERAL", 0, 1, 4, 0.15f, true);
        animador->agreganAnimacion("ATAQUE", 1, 1, 1, 0.20f, true);
        animador->agreganAnimacion("ABAJO", 1, 3, 3, 0.20f, true);
        animador->agreganAnimacion("ARRIBA", 1, 4, 4, 0.20f, true);
    }
}

void ClaseFenix::procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) {
    PiezaVoladora::procesarMovimientoArena(direccion, dt, arena);
    if (this->stats.nombre == "LIBRARIAN" || this->stats.nombre == "HARPY") {
        animar(dt, direccion);
    }
}

void ClaseFenix::animar(float dt, sf::Vector2f direccion) {
    if (!animador) return;

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

    //  ARREGLO DEL EFECTO ESPEJO:
    float escalaArena = piezaAlturaArena / altoFrame;
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