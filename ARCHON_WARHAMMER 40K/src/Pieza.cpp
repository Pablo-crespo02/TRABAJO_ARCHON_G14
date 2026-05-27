#include "Pieza.h"
#include <iostream>
#include <cmath>

// Constructor
// Ahora es mucho más sencillo y recibe los datos mínimos obligatorios
Pieza::Pieza(Bando b, sf::Vector2i pos) {
    bando = b;
    posicionTablero = pos;
    seleccionado = false;
    hechizoDisponible = true;

    // Inicialización de los estados alterados (Ralentización Gárgola)
    this->tiempoRalentizado = 0.0;
    this->multiplicadorVelocidadActual = 1.0;
}

// Sincronización visual:
// Esto es igual para todas las piezas: todas se dibujan en el tablero
void Pieza::sincronizarPosicionTablero() {
    // 60.f es el valor de TAMANOCASILLA. 
    float px = (posicionTablero.x * 60.f) + (60.f / 2.f);
    float py = (posicionTablero.y * 60.f) + (60.f / 2.f);

    // Guardamos la posición y movemos el sprite o forma
    posicionAbsoluta = sf::Vector2f(px, py);
}

// Movimiento en el tablero:
void Pieza::mover(sf::Vector2i destino) {
    posicionTablero = destino;
    sincronizarPosicionTablero();
}

// Movimiento en la arena:
void Pieza::moverEnArena(float dx, float dy) {
    posicionAbsoluta.x += dx;
    posicionAbsoluta.y += dy;
}

// Detecta conflicto y pasa al combate:
// Esta lógica es universal: si estoy en la misma celda que un enemigo, hay conflicto
bool Pieza::detectarConflicto(const std::vector<Pieza*>& otrasPiezas) {
    for (const auto* otra : otrasPiezas) {
        if (otra != this && otra->getPosicionTablero() == this->posicionTablero) {
            if (otra->getBando() != this->bando) {
                return true;
            }
        }
    }
    return false;
}

//GESTIÓN DE PROYECTILES:

bool Pieza::puedeAtacar() const {
    // Usa la velocidad de ataque definida en cada Clase
    if (stats.relojHitbox.getElapsedTime().asSeconds() >= stats.velAtaque) {
        return true;
    }
    return false;
}

void Pieza::reiniciarRelojHitbox() {
    stats.relojHitbox.restart();
    barrasArena.reiniciarRecarga();
}

//Gestión de la inmovilización del basilisco (activación y tiempo)
void Pieza::aplicarInmovilizacion(double duracion) {
    inmovilizado = true;
    temporizadorInmovilizacion = duracion;
}

//Gestiona inmovilización e invulnerabilidad 
void Pieza::gestionarEstadosAlterados(double dt) {
    //parálisis basilisco
    if (inmovilizado) {
        temporizadorInmovilizacion -= dt;
        if (temporizadorInmovilizacion <= 0.0) {
            inmovilizado = false;
            temporizadorInmovilizacion = 0.0;
        }
    }
    //invulnerabildiad unicornio
    if (invulnerable) {
        temporizadorInvulnerabilidad -= dt; // Restamos el tiempo por frame
        if (temporizadorInvulnerabilidad <= 0.0) {
            invulnerable = false;
        }
    }
    //ralentización Gárgola
    if (tiempoRalentizado > 0.0) {
        tiempoRalentizado -= dt;
        if (tiempoRalentizado <= 0.0) {
            tiempoRalentizado = 0.0;
            // Restauramos la velocidad a la normalidad (100%)
            multiplicadorVelocidadActual = 1.0;
        }
    }
}

//Invulnerabilidad del unicornio: (la activa y controla el tiempo)
void Pieza::aplicarInvulnerabilidad(double duracion) {
    invulnerable = true;
    temporizadorInvulnerabilidad = duracion;
}

//Ralentización de la Gárgola:
void Pieza::aplicarRalentizacion(double factor, double duracion) {
    // Si ya estaba ralentizado, se sobreescribe con el nuevo valor. 
    this->multiplicadorVelocidadActual = factor;
    this->tiempoRalentizado = duracion;
}

void Pieza::actualizarEstadosAlterados(double dt) {
    if (this->tiempoRalentizado > 0.0) {
        this->tiempoRalentizado -= dt;

        // Si el tiempo se agota, restauramos la velocidad base
        if (this->tiempoRalentizado <= 0.0) {
            this->tiempoRalentizado = 0.0;
            this->multiplicadorVelocidadActual = 1.0;
        }
    }
}

// Sprites y animaciones:

void Pieza::cargarConfigurarSprites(const std::string& tipo) {
    std::string rutaTablero = "";
    std::string rutaArena = "";

    if (tipo == "VINDICARE" || tipo == "LICTOR") {
        rutaTablero = (tipo == "VINDICARE") ? "imagenes/BASE-VINDICARE-Humanidad.png" : "imagenes/BASE-LICTOR-TYRANIDS.png";
        rutaArena = (tipo == "VINDICARE") ? "imagenes/Chibi-VINDICARE-Humanidad-1.0.png" : "imagenes/Chibi-LICTOR-TYRANIDS-1.0.png";
    }
    else if (tipo == "CULEXUS" || tipo == "GENESTEALER") {
        rutaTablero = (tipo == "CULEXUS") ? "imagenes/BASE-CULEXUS-Humanidad.png" : "imagenes/BASE-GENESTEALER-TYRANIDS.png";
        rutaArena = (tipo == "CULEXUS") ? "imagenes/Chibi-CULEXUS-Humanidad-1.0.png" : "imagenes/Chibi-GENESTEALER-TYRANIDS-1.0.png";
    }
    else if (tipo == "DREADNOUGHT" || tipo == "CARNIFEX") {
        rutaTablero = (tipo == "DREADNOUGHT") ? "imagenes/BASE-DREADNOUGHT-Humanidad.png" : "imagenes/BASE-CARNIFEX-TYRANIDS.png";
        rutaArena = (tipo == "DREADNOUGHT") ? "imagenes/Chibi-DREADNOUGHT-Humanidad-1.0.png" : "imagenes/Chibi-CARNIFEX-TYRANIDS-1.0.png";
    }
    else if (tipo == "LIBRARIAN" || tipo == "HARPY") {
        rutaTablero = (tipo == "LIBRARIAN") ? "imagenes/BASE-LIBRARIAN-Humanidad.png" : "imagenes/BASE-HARPY-TYRANIDS.png";
        rutaArena = (tipo == "LIBRARIAN") ? "imagenes/Chibi-LIBRARIAN-Humanidad-1.0.png" : "imagenes/Chibi-HARPY-TYRANIDS-1.0.png";
    }
    else if (tipo == "INTERCESSOR" || tipo == "TERMAGANT") {
        rutaTablero = (tipo == "INTERCESSOR") ? "imagenes/BASE-INTERCESSOR-Humanidad.png" : "imagenes/BASE-TERMAGANT-TYRANIDS.png";
        rutaArena = (tipo == "INTERCESSOR") ? "imagenes/Chibi-INTERCESSOR-Humanidad.png" : "imagenes/Chibi-TERMAGANT-TYRANIDS-1.0.png";
    }
    else if (tipo == "CAPTAIN" || tipo == "HIVE TYRANT") {
        rutaTablero = (tipo == "CAPTAIN") ? "imagenes/BASE-CAPTAIN-Humanidad.png" : "imagenes/BASE-HIVE_TYRANT-TYRANIDS.png";
        rutaArena = (tipo == "CAPTAIN") ? "imagenes/Chibi-CAPTAIN-Humanidad-1.0.png" : "imagenes/Chibi-HIVE_TYRANT-TYRANIDS-1.0.png";
    }
    else if (tipo == "PRIMARIS" || tipo == "TOXICRENO") {
        rutaTablero = (tipo == "PRIMARIS") ? "imagenes/BASE-PRIMARIS-Humanidad.png" : "imagenes/BASE-TOXICRENO-TYRANIDS.png";
        rutaArena = (tipo == "PRIMARIS") ? "imagenes/Chibi-PRIMARIS-Humanidad-1.0.png" : "imagenes/Chibi-TOXICRENO-TYRANIDS-1.0.png";
    }
    else if (tipo == "ASSAULT MARINE" || tipo == "GARGOLA") {
        rutaTablero = (tipo == "ASSAULT MARINE") ? "imagenes/BASE-ASSAULT_MARINE-Humanidad.png" : "imagenes/BASE-GARGOLA-TYRANIDS.png";
        rutaArena = (tipo == "ASSAULT MARINE") ? "imagenes/Chibi-ASSAULT_MARINE-Humanidad-1.0.png" : "imagenes/Chibi-GARGOLA-TYRANIDS-1.0.png";
    }

    //Todas las hojas de sprites comparten número de filas y columnas:
    int columnas = 5;
    int filas = 2;

    //Gestión de la carga de la textura y el sprite del TABLERO:
    if (!texturaTablero.loadFromFile(rutaTablero)) {
        std::cout << "ERROR: TEXTURA NO ENCONTRADA TABLERO: " << rutaTablero << std::endl;
    }
    else {
        spriteTablero.setTexture(texturaTablero);
        spriteTablero.setOrigin(texturaTablero.getSize().x / 2.0f, texturaTablero.getSize().y / 2.0f);

        float escalaTablero = piezaAlturaTablero / texturaTablero.getSize().y;
        spriteTablero.setScale(escalaTablero, escalaTablero);
    }

    //Gestión de la carga de la textura, sprite y animaciones de la ARENA:
    if (!texturaArena.loadFromFile(rutaArena)) {
        std::cout << "ERROR: TEXTURA NO ENCONTRADA ARENA: " << rutaArena << std::endl;
    }
    else {
        spriteArena.setTexture(texturaArena);

        // Medida del frame individual
        anchoFrame = texturaArena.getSize().x / columnas;
        altoFrame = texturaArena.getSize().y / filas;

        spriteArena.setTextureRect(sf::IntRect(0, 0, anchoFrame, altoFrame));
        spriteArena.setOrigin(anchoFrame / 2.0f, altoFrame / 2.0f);

        double escalaArena = piezaAlturaArena / altoFrame;

        // Volteado lateral de escala automática según el bando
        if (this->bando == Bando::OSCURIDAD) {
            spriteArena.setScale(-escalaArena, escalaArena);
        }
        else {
            spriteArena.setScale(escalaArena, escalaArena);
        }

        //Constructor para instanciar punteros inteligentes.
        //Vincula "AnimadorSprites" con el sprite de la arena, su alto y su ancho.
        //Los punteros inteligentes se eliminan automáticamente de la RAM cuando la pieza el eliminada, evitando fugas de memoria.
        animador = std::make_unique<AnimadorSprites>(spriteArena, anchoFrame, altoFrame);


        //Registro de animaciones comunes:
        animador->agreganAnimacion("QUIETO", 0, 0, 0, 0.20f, true);
        animador->agreganAnimacion("CAMINAR_LATERAL", 0, 1, 4, 0.15f, true);
        animador->agreganAnimacion("ABAJO", 1, 3, 3, 0.20f, true);
        animador->agreganAnimacion("ARRIBA", 1, 4, 4, 0.20f, true);

        animador->reproducir("QUIETO"); //animación default, la pieza está quieta
    }
}

// Lógica de selección de las animaciones:
void Pieza::Animar(float dt, sf::Vector2f direccion) {
    if (!animador) return;

    //Comprobamos el reloj interno compartido por todas las piezas:
    bool estaAtacando = (stats.relojHitbox.getElapsedTime().asSeconds() < 0.2f); //Por qué compara el reloj con 0.2 en vezde otro valor?

    if (estaAtacando) {
        animador->reproducir("ATAQUE");
    }
    else if (direccion.x != 0) {
        animador->reproducir("CAMINAR_LATERAL");
    }
    else if (direccion.y > 0) {
        animador->reproducir("ABAJO");
    }
    else if (direccion.y < 0) {
        animador->reproducir("ARRIBA");
    }
    else {
        animador->reproducir("QUIETO");
    }

    actualizarAnimacion(dt);

    //Arreglo del efecto espejo genérico:
    float escalaArena = piezaAlturaArena / altoFrame;
    if (direccion.x < 0) {
        spriteArena.setScale(-escalaArena, escalaArena); // Mira a la izquierda
    }
    else if (direccion.x > 0) {
        spriteArena.setScale(escalaArena, escalaArena);  // Mira a la derecha
    }
    else {
        // Respeta la dirección actual
        float escalaActualX = (spriteArena.getScale().x > 0) ? escalaArena : -escalaArena;
        spriteArena.setScale(escalaActualX, escalaArena);
    }
}

void Pieza::actualizarAnimacion(double dt) {
    if (animador) animador->actualizar(dt);
}

void Pieza::actualizarFlash(float dt) {
    if (temporizadorFlashDano > 0.f) {
        temporizadorFlashDano -= dt;

        // Si está sufriendo daño, teñimos el sprite de ROJO (manteniendo su transparencia actual por si es un Lictor invisible)
        spriteArena.setColor(sf::Color(255, 50, 50, spriteArena.getColor().a));
    }
    else {
        // Si no está sufriendo daño, restauramos su color BLANCO original (que significa "sin filtro")
        spriteArena.setColor(sf::Color(255, 255, 255, spriteArena.getColor().a));
    }
}

void Pieza::dibujarAnilloSeleccion(sf::RenderWindow& window) {
        sf::CircleShape anilloSeleccion(25.f);
        anilloSeleccion.setOrigin(25.f, 25.f);
        anilloSeleccion.setPosition(posicionAbsoluta);
        anilloSeleccion.setFillColor(sf::Color::Transparent); // Fondo vacío
        anilloSeleccion.setOutlineThickness(4.f);             // Borde grueso
        anilloSeleccion.setOutlineColor(Colores::ColorOutlineSeleccion); // Amarillo
        window.draw(anilloSeleccion);
    }

void Pieza::intentarUsarHechizo(std::vector<Hitbox>& Hitboxes, Pieza* enemigo) {
    // Regla 1: ¿Tengo cargas en la ronda?
    // Regla 2: ¿He usado mi "cupo" en este combate específico?
    if (hechizosRestantes > 0 && !usadoEnEstaArena) {
        usarHechizo(Hitboxes, enemigo);

        hechizosRestantes--; // Restamos 1 carga de la ronda
        usadoEnEstaArena = true; // Bloqueamos uso adicional en este combate

        std::cout << "Hechizo usado. Quedan " << hechizosRestantes << " cargas para la ronda." << std::endl;
    }
    else {
        std::cout << "No puedes lanzar el hechizo (Sin cargas o ya usado en este combate)." << std::endl;
    }
}