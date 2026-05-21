#include "ClaseHelicoptero.h"
#include <cmath>
#include <iostream>

const float HELI_ALTURA_ARENA = 120.0f; // Escala visual en la arena

ClaseHelicoptero::ClaseHelicoptero(Bando b, sf::Vector2f posArena)
    : PiezaVoladora(b, sf::Vector2i(0, 0)) // Posición de tablero ficticia (no se usa)
{
    // Asignamos la posición física inicial directamente en la arena
    this->posicionAbsoluta = posArena;

    // STATS DE UNIDAD PESADA (Más fuerte que un minion común)
    this->stats.nombre = "HELICOPTER";
    this->stats.vida = 15.0f;       // Bastante más vida que los Termagants
    this->stats.vidaMaxima = 15.0f;
    this->stats.ataque = 4.0f;      // Daño por proyectil ametralladora
    this->stats.defensa = 3.0f;
    this->stats.velAtaque = 1.2f;   // Cadencia de disparo automático
    this->stats.esRango = true;     // Ataca con proyectiles
    this->bando = b;

    // CARGA DE SPRITESHEET
    // Cambia "imagenes/Helicoptero-Sheet.png" por la ruta real de tus sprites
    if (!texturaArena.loadFromFile("imagenes/Chibi-HELICOPTER-Humanidad-1.0.png")) {
        std::cout << "Error: No se encontro el sprite del helicopetero" << std::endl;
    }

    spriteArena.setTexture(texturaArena);

    int columnas = 5;
    int filas = 2;

    anchoFrame = texturaArena.getSize().x / columnas;
    altoFrame = texturaArena.getSize().y / filas;

    spriteArena.setTextureRect(sf::IntRect(0, 0, anchoFrame, altoFrame));
    spriteArena.setOrigin(anchoFrame / 2.0f, altoFrame / 2.0f);

    // Ajuste de escala e inversión de bando por defecto
    float escalaArena = HELI_ALTURA_ARENA / altoFrame;
    if (this->bando == Bando::OSCURIDAD) {
        spriteArena.setScale(-escalaArena, escalaArena);
    }
    else {
        spriteArena.setScale(escalaArena, escalaArena);
    }

    frameActual = 0;
    temporizadorAnimacion = 0.0f;
}

void ClaseHelicoptero::dibujar(sf::RenderWindow& window, Estado estadoActual) {
    if (estadoActual == Estado::Arena) {
        spriteArena.setPosition(this->posicionAbsoluta);
        window.draw(spriteArena);

        // Si usas las barras de vida:
        barrasArena.actualizar(stats.vida, stats.vidaMaxima, stats.velAtaque, this->posicionAbsoluta);
        barrasArena.dibujar(window);
    }
}
void ClaseHelicoptero::actualizarIA(float dt, Arena& arena, Pieza* enemigo, std::vector<Hitbox>& hitboxes) {
    if (!enemigo) return;

    sf::Vector2f posHeli = this->posicionAbsoluta;
    sf::Vector2f posEnemigo = enemigo->getPosicionAbsoluta();

    // 1. MOVIMIENTO: Calcular vector hacia el enemigo
    sf::Vector2f dir = posEnemigo - posHeli;
    float distancia = std::hypot(dir.x, dir.y);

    if (distancia != 0.f) dir /= distancia; // Normalizar vector

    // Mantener una distancia prudencial de combate (ej: 180 píxeles)
    sf::Vector2f dirMovimiento = dir;
    if (distancia < 180.f) {
        dirMovimiento = -dir; // Se aleja un poco para ametrallar desde el aire si el enemigo se acerca
    }

    float velocidadVuelo = 130.f;
    sf::Vector2f desplazamiento = dirMovimiento * velocidadVuelo * dt;
    sf::Vector2f nuevaPos = posHeli + desplazamiento;

    // =========================================================================
    // ¡CORRECCIÓN VOLADORA!: Ignoramos obstáculos internos de la arena
    // Pasamos 'true' para indicarle a la arena que somos una unidad aérea.
    // =========================================================================
    if (arena.esPosicionValida(nuevaPos, 25.f, true)) {
        this->posicionAbsoluta = nuevaPos;
        this->animar(dt, dirMovimiento);
    }
    else {
        // Si llega a tocar los bordes exteriores del mapa (límites de la pantalla), 
        // se frena pero sigue reproduciendo la animación de flotar/quieto.
        this->animar(dt, sf::Vector2f(0.f, 0.f));
    }

    // =========================================================================
    // 2. DISPARO AUTOMÁTICO POR COOLDOWN
    // =========================================================================
    if (relojDisparoAuto.getElapsedTime().asSeconds() >= this->stats.velAtaque) {
        sf::Vector2f dirBala = posEnemigo - this->posicionAbsoluta;
        float magBala = std::hypot(dirBala.x, dirBala.y);
        if (magBala != 0.f) dirBala /= magBala;

        sf::Vector2f spawnBala = this->posicionAbsoluta + (dirBala * 30.f);

        // Instanciamos el proyectil en el contenedor global de Hitboxes
        hitboxes.emplace_back(
            spawnBala,
            dirBala,
            600,
            Colores::ColorProyectil,
            this,
            this->stats.ataque,
            4.0f,
            10.0f
        );

        relojDisparoAuto.restart();
    }
}

void ClaseHelicoptero::animar(float dt, sf::Vector2f direccion) {
    int fila = 0;
    int colInicial = 0;
    int colFinal = 0;

    // Evaluamos si el helicóptero acaba de disparar usando su propio reloj de ataque
    // Si hace menos de 0.2 segundos que disparó, forzamos la animación de ataque
    bool estaAtacando = (this->relojDisparoAuto.getElapsedTime().asSeconds() < 0.2f);

    if (estaAtacando) {
        // FOTOGRAMA DE ATAQUE (Fila 1, Columna 2)
        fila = 1;
        colInicial = 0;
        colFinal = 0;
    }
    else if (direccion.x != 0.f) {
        // SECUENCIA DE VUELO LATERAL (Fila 0, Columnas de la 1 a la 4)
        fila = 0;
        colInicial = 1;
        colFinal = 2;
    }
    else if (direccion.y > 0.f) {
        // FOTOGRAMA DE DESCENSO / ABAJO (Fila 1, Columna 3)
        fila = 1;
        colInicial = 1;
        colFinal = 2;
    }
    else if (direccion.y < 0.f) {
        // FOTOGRAMA DE ASCENSO / ARRIBA (Fila 1, Columna 4)
        fila = 1;
        colInicial = 3;
        colFinal = 4;
    }
    else {
        // FOTOGRAMA ESTÁTICO / QUIETO (Fila 0, Columna 0)
        fila = 0;
        colInicial = 0;
        colFinal = 0;
    }

    // 1. GESTIÓN DE CAMBIO DE FRAME / FILA
    int posY_Textura = fila * altoFrame;
    int posY_Actual = spriteArena.getTextureRect().top;

    // Si cambia la fila de animación o el frame actual se sale de los márgenes permitidos
    if (posY_Actual != posY_Textura || frameActual < colInicial || frameActual > colFinal) {
        frameActual = colInicial;
        spriteArena.setTextureRect(sf::IntRect(frameActual * anchoFrame, posY_Textura, anchoFrame, altoFrame));
        temporizadorAnimacion = 0.0f;
    }

    // Si es una secuencia en movimiento (Caminar/Volar lateral tiene varias columnas)
    if (colInicial != colFinal) {
        temporizadorAnimacion += dt;
        float velocidadAnimacion = 0.12f; // Ajusta este número si quieres las hélices más rápidas o lentas

        if (temporizadorAnimacion >= velocidadAnimacion) {
            temporizadorAnimacion = 0.0f;
            frameActual++;

            if (frameActual > colFinal) {
                frameActual = colInicial;
            }

            spriteArena.setTextureRect(sf::IntRect(frameActual * anchoFrame, posY_Textura, anchoFrame, altoFrame));
        }
    }

    // =========================================================================
    // 2. EFECTO ESPEJO SEGURO (Usando la constante del Helicóptero)
    // =========================================================================
    float escalaHeli = HELI_ALTURA_ARENA / altoFrame;

    if (direccion.x < 0.f) {
        spriteArena.setScale(-escalaHeli, escalaHeli); // Mira a la izquierda
    }
    else if (direccion.x > 0.f) {
        spriteArena.setScale(escalaHeli, escalaHeli);  // Mira a la derecha
    }
    else {
        // Si sube, baja, ataca o está quieto flotando, mantiene la orientación que llevaba
        float escalaActualX = (spriteArena.getScale().x > 0.f) ? escalaHeli : -escalaHeli;
        spriteArena.setScale(escalaActualX, escalaHeli);
    }
}