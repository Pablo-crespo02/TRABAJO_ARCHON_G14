#include "ClaseArcher.h"
#include <iostream>

const float PIEZA_ALTURA_TABLERO = 90.0f;
const float PIEZA_ALTURA_ARENA = 120.0f;

ClaseArcher::ClaseArcher(Bando b, sf::Vector2i pos, std::string tipo)
    : PiezaTerrestre(b, pos)
{
    //ESTADÍSTICAS 
    this->stats.nombre = tipo;
    this->stats.vida = 18.0f;
    this->stats.vidaMaxima = 18.0f;
    this->stats.ataque = 5.0f;
    this->stats.defensa = 8.0f;
    this->rangoMovimiento = 3;
    this->stats.velAtaque = 1.2f;
    this->stats.esRango = true;
    // --- Lógica de tipos ---
   // Asignación del patrón de movimiento
    this->patronMovimiento = PatronMovimiento::Ortogonal;
    this->tipoMov = TipoMovimiento::Terrestre;  // Solo para el HUD
    this->esInvisible = false;
    this->preparandoInvisibilidad = false;
    this->tieneBoostVelocidad = false;
    this->temporizadorInvisibilidad = 0.0;
    this->temporizadorBoost = 0.0;
    this->multiplicadorVelocidad = 1.0f;
    // Forzamos al sprite a que tenga opacidad máxima de salida



    //CARGA DE SPRITES (Chibi)
    if (tipo == "VINDICARE" || tipo == "LICTOR") {

        std::string rutaTablero = (tipo == "VINDICARE") ? "imagenes/BASE-VINDICARE-Humanidad.png" : "imagenes/BASE-LICTOR-TYRANIDS.png";
        std::string rutaArena = (tipo == "VINDICARE") ? "imagenes/Chibi-VINDICARE-Humanidad-1.0.png" : "imagenes/Chibi-LICTOR-TYRANIDS-1.0.png";
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


void ClaseArcher::animar(float dt, sf::Vector2f direccion) {
    int fila = 0;
    int colInicial = 0;
    int colFinal = 0;

    // Leemos el reloj interno. Si hace menos de 0.2 segundos que disparamos, estamos atacando.
    bool estaAtacando = (this->stats.relojHitbox.getElapsedTime().asSeconds() < 0.2f);

    if (preparandoInvisibilidad) {
        fila = 1;       // Fila 1 de la hoja de sprites
        colInicial = 2; // Columna 2 de la hoja de sprites
        colFinal = 2;   // Al ser igual que la inicial, la animación se congela en este frame
    }
    else if (estaAtacando) {
        //FOTOGRAMA DE ATAQUE 
        fila = 1;
        colInicial = 0;
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

void ClaseArcher::dibujar(sf::RenderWindow& window, Estado estadoActual) {
    if (estadoActual == Estado::Tablero) {
        this->sincronizarPosicionTablero();

        if (this->stats.nombre == "VINDICARE" || this->stats.nombre == "LICTOR") {

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
        if (this->stats.nombre == "VINDICARE" || this->stats.nombre == "LICTOR") {
            spriteArena.setPosition(posicionAbsoluta);
            window.draw(spriteArena);
        }
        else {
            formaVisual.setPosition(posicionAbsoluta);
            window.draw(formaVisual);
        }

        //DIBUJAMOS BARRA DE VIDA SOBRE LA PIEZA
        barrasArena.actualizar(stats.vida, stats.vidaMaxima, stats.velAtaque, posicionAbsoluta);
        if (!esInvisible) {
            barrasArena.actualizar(stats.vida, stats.vidaMaxima, stats.velAtaque, posicionAbsoluta);
            barrasArena.dibujar(window);
        }
    }
}
//Hechizo 
void ClaseArcher::usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) {
    // ACTIVACIÓN PARA EL LICTOR (OSCURIDAD)
    if (this->getBando() == Bando::OSCURIDAD) {
        this->preparandoInvisibilidad = true;
        this->esInvisible = false;
        this->temporizadorInvisibilidad = 0.5; // Iniciamos con el medio segundo de carga "clara"
        std::cout << "El Lictor comienza a camuflarse..." << std::endl;
    }
    // ACTIVACIÓN PARA EL ARQUERO (LUZ)
    else if (this->getBando() == Bando::LUZ) {
        this->tieneBoostVelocidad = true;
        this->temporizadorBoost = 5.0; // Duración del sprint (ej: 5 segundos)
        this->multiplicadorVelocidad = 2.5f; // x2.5 de velocidad (ajusta el número a tu gusto)
        std::cout << "Bendición de velocidad activada!" << std::endl;
    }
}




void ClaseArcher::gestionarInvisibilidad(double dt) {
    // PASO 1: Fase de preparación (Medio segundo con el sprite especial)
    if (preparandoInvisibilidad) {
        temporizadorInvisibilidad -= dt;

        // Mientras se prepara, nos aseguramos de que el sprite sea totalmente visible
        sf::Color colorAct = spriteArena.getColor();
        colorAct.a = 255;
        spriteArena.setColor(colorAct);

        // Si pasa el medio segundo, pasamos a la invisibilidad real
        if (temporizadorInvisibilidad <= 0.0) {
            preparandoInvisibilidad = false;
            esInvisible = true;
            temporizadorInvisibilidad = 8.0; // Inician los 8 segundos de sigilo
            std::cout << "El Lictor se vuelve invisible!" << std::endl;
        }
    }
    // PASO 2: Invisibilidad activa (8 segundos casi invisible)
    else if (esInvisible) {
        temporizadorInvisibilidad -= dt;

        // Transparencia casi total para el sprite de la arena
        sf::Color colorAct = spriteArena.getColor();
        colorAct.a = 10;
        spriteArena.setColor(colorAct);

        // Si el tiempo se agota, restauramos todo a la normalidad
        if (temporizadorInvisibilidad <= 0.0) {
            esInvisible = false;
            temporizadorInvisibilidad = 0.0;

            colorAct.a = 255; // Opacidad total
            spriteArena.setColor(colorAct);
            std::cout << "El camuflaje del Lictor se ha agotado." << std::endl;
        }
    }
    else {
        sf::Color colorAct = spriteArena.getColor();
        if (colorAct.a != 255) {
            colorAct.a = 255;
            spriteArena.setColor(colorAct);
        }
    }
}

//Unicamente afecta al bando oscuridad y mide el tiempo que pasa con el x3 de velocidad
void ClaseArcher::gestionarBoostVelocidad(double dt) {
    if (tieneBoostVelocidad) {
        temporizadorBoost -= dt;

        if (temporizadorBoost <= 0.0) {
            tieneBoostVelocidad = false;
            temporizadorBoost = 0.0;
            multiplicadorVelocidad = 1.0f; // Vuelve a su velocidad normal
            std::cout << "El Boost de velocidad de la Luz se ha agotado." << std::endl;
        }
    }
}

// Interceptamos la función de movimiento para insertar nuestra actualización de tiempo
//Sirve para que el motor de las físicas (Motor::actualizar) se ponga de acuerdo con "dt"
void ClaseArcher::procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) {
    double deltaTiempo = static_cast<double>(dt);
    gestionarInvisibilidad(deltaTiempo);
    gestionarBoostVelocidad(deltaTiempo);

    // Actualizamos la animación pasándole la dirección actual 
    // (Si está preparando la habilidad, congelará el sprite en la Fila 1, Columna 2)
    this->animar(dt, direccion);

    // Filtro de movimiento físico: si no se mueve, aquí sí salimos
    if (direccion == sf::Vector2f(0.f, 0.f)) return;

    // Código de físicas de movimiento clásico...
    float velocidadBase = 200.f;
    float velocidadFinal = velocidadBase * multiplicadorVelocidad;
    sf::Vector2f desplazamiento = direccion * velocidadFinal * dt;
    sf::Vector2f nuevaPos = posicionAbsoluta + desplazamiento;

    if (arena.esPosicionValida(nuevaPos, 20.f, false)) {
        this->moverEnArena(desplazamiento.x, desplazamiento.y);
    }
}