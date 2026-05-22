#include "ClaseKnight.h"
#include <cmath> 
#include <iostream>

const float PIEZA_ALTURA_TABLERO = 90.0f;
const float PIEZA_ALTURA_ARENA = 80.0f;

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
    //CARGA DE SPRITES (Chibi)
    if (tipo == "INTERCESSOR" || tipo == "TERMAGANT") {

        std::string rutaTablero = (tipo == "INTERCESSOR") ? "imagenes/BASE-INTERCESSOR-Humanidad.png" : "imagenes/BASE-TERMAGANT-TYRANIDS.png";
        std::string rutaArena = (tipo == "INTERCESSOR") ? "imagenes/Chibi-INTERCESSOR-Humanidad.png" : "imagenes/Chibi-TERMAGANT-TYRANIDS-1.0.png";
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

void ClaseKnight::procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) {
    // Actualizamos la lógica de tiempo del salto primero
    actualizarSalto(dt);

    // Si el Termagant está saltando, bloqueamos el movimiento terrestre físico
    // Esto impide que el movimiento normal interfiera con la trayectoria del salto
    if (estaSaltando) {
        // Aún llamamos a animar para que mantenga el sprite de "salto" activo
        this->animar(dt, direccion);
        return;
    }

    // Si NO está saltando, dejamos que la clase padre maneje el movimiento normal
    PiezaTerrestre::procesarMovimientoArena(direccion, dt, arena);

    // Actualizamos la animación normal (caminar/quieto/atacar)
    if (this->stats.nombre == "INTERCESSOR" || this->stats.nombre == "TERMAGANT") {
        animar(dt, direccion);
    }
}

void ClaseKnight::animar(float dt, sf::Vector2f direccion) {
    int fila = 0;
    int colInicial = 0;
    int colFinal = 0;

    // Leemos el reloj interno. Si hace menos de 0.2 segundos que disparamos, estamos atacando.
    bool estaAtacando = (this->stats.relojHitbox.getElapsedTime().asSeconds() < 0.2f);

    if (estaAtacando) {
        //FOTOGRAMA DE ATAQUE 
        fila = 1;
        colInicial = 0;
        colFinal = 1;
    }
    else if (estaSaltando) {
        //FOTOGRAMA DE ATAQUE 
        fila = 1;
        colInicial = 2;
        colFinal = 2;
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
    if (!enemigo) return;

    // LÓGICA PARA INTERCESSOR (Granada)
    if (this->stats.nombre == "INTERCESSOR") {
        sf::Vector2f dirAtaque = this->getultimadireccion();
        float magnitud = std::hypot(dirAtaque.x, dirAtaque.y);
        dirAtaque = (magnitud != 0) ? (dirAtaque / magnitud) : sf::Vector2f(1.f, 0.f);

        sf::Vector2f puntoSpawn = this->posicionAbsoluta + (dirAtaque * 35.f);

        hitboxes.emplace_back(
            puntoSpawn, dirAtaque, 400.0, sf::Color(100, 100, 100), this,
            (10.0 * this->multiplicadorArena), 2.5, 10.0,
            false, false, false, 0.0, true, 120.0
        );
        std::cout << "Intercessor ha lanzado una Granada de Fragmentación!" << std::endl;
    }

    // LÓGICA PARA TERMAGANT (Salto Acechante)
    else if (this->stats.nombre == "TERMAGANT") {
        // Solo iniciamos si no estamos saltando ya
        if (estaSaltando) return;

        sf::Vector2f posEnemigo = enemigo->getPosicionAbsoluta();
        sf::Vector2f miPos = this->posicionAbsoluta;
        sf::Vector2f dir = posEnemigo - miPos;
        float dist = std::hypot(dir.x, dir.y);

        if (dist > 50.f) {
            // Normalizar dirección
            if (dist != 0) dir /= dist;

            // Guardamos los puntos para la trayectoria
            posInicioSalto = miPos;
            posDestinoSalto = posEnemigo - (dir * 40.f);

            // Reseteamos el reloj del salto al inicio (0.0f)
            tiempoSalto = 0.0f;
            estaSaltando = true;

            std::cout << "Termagant inicia salto acechante!" << std::endl;
        }
    }
}
void ClaseKnight::actualizarSalto(float dt) {
    if (!estaSaltando) return;

    tiempoSalto += dt;

    // Calculamos el progreso (de 0.0 a 1.0)
    float progreso = tiempoSalto / duracionTotalSalto;

    // Si llega al final, aterrizaje forzoso
    if (progreso >= 1.0f) {
        this->posicionAbsoluta = posDestinoSalto;
        estaSaltando = false;
        tiempoSalto = 0.0f;
    }
    else {
        // Movimiento lineal X/Y (la base del salto)
        sf::Vector2f nuevaPos = posInicioSalto + (posDestinoSalto - posInicioSalto) * progreso;

        // EL TRUCO: La parábola siempre debe ir de 0 a PI (180 grados)
        // sin importar cuánto dure el tiempo.
        // Al usar 'progreso * 3.14159f', garantizamos que el seno haga 
        // una curva completa de 0 a 180 grados en cualquier duración.
        float alturaMaxima = 150.0f; // Ajusta esto para que sea más alto o bajo
        float altura = -alturaMaxima * sin(progreso * 3.14159f);

        this->posicionAbsoluta = nuevaPos + sf::Vector2f(0.0f, altura);
    }
}