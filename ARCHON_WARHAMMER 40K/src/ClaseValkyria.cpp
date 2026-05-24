#include "ClaseValkyria.h"
#include <cmath> 
#include <iostream>

const float PIEZA_ALTURA_TABLERO = 80.0f;
const float PIEZA_ALTURA_ARENA = 120.0f;

ClaseValkyria::ClaseValkyria(Bando b, sf::Vector2i pos, std::string tipo)
    : PiezaVoladora(b, pos)
{
    //ESTADÍSTICAS 
    this->stats.nombre = tipo;
    this->stats.vida = 35.0f;
    this->stats.vidaMaxima = 35.0f; // Ajusta a valores en el futuro
    this->stats.ataque = 5.0f;
    this->stats.defensa = 20.0f;
    this->stats.velAtaque = 1.2f;
    // --- Lógica de tipos ---
    this->stats.esRango = true;    // La valkiria es rango

    this->rangoMovimiento = 4;
    this->patronMovimiento = PatronMovimiento::Ambos;
    this->tipoMov = TipoMovimiento::Volador;  // Solo para el HUD

    //TAMAÑO DE LOS SPRITES:
    this->piezaAlturaTablero = 80.0f;
    this->piezaAlturaArena = 120.0f;

    //CARGA DE SPRITES (Chibi)
    cargarConfigurarSprites(tipo);


        std::string rutaTablero = (tipo == "ASSAULT_MARINE") ? "imagenes/BASE-ASSAULT_MARINE-Humanidad.png" : "imagenes/BASE-GARGOLA-TYRANIDS.png";
        std::string rutaArena = (tipo == "ASSAULT_MARINE") ? "imagenes/Chibi-ASSAULT_MARINE-Humanidad-1.0.png" : "imagenes/Chibi-GARGOLA-TIRANIDS-1.0.png";
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

    // 4. REGISTRO DE CLIPS DE ANIMACIÓN EN EL DICCIONARIO
    if (animador) {
        animador->agreganAnimacion("QUIETO", 0, 0, 0, 0.20f, true);
        animador->agreganAnimacion("CAMINAR_LATERAL", 0, 1, 4, 0.15f, true);
        animador->agreganAnimacion("ATAQUE", 1, 2, 2, 0.20f, true);
        animador->agreganAnimacion("ABAJO", 1, 3, 3, 0.20f, true);
        animador->agreganAnimacion("ARRIBA", 1, 4, 4, 0.20f, true);

    }
}

//ENLACE DE FÍSICAS Y ANIMACIÓN
void ClaseValkyria::procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) {
    //Dejamos que la clase padre (PiezaTerrestre) mueva las coordenadas físicas
    PiezaVoladora::procesarMovimientoArena(direccion, dt, arena);

    //Actualizamos la imagen visible con nuestra máquina de estados
    if (this->stats.nombre == "ASSAULT_MARINE" || this->stats.nombre == "GARGOLA") {
        animar(dt, direccion);
    }
}

void ClaseValkyria::animar(float dt, sf::Vector2f direccion) {
    if (!animador) return;

    // Leemos el reloj interno para comprobar el estado de ataque
    bool estaAtacando = (this->stats.relojHitbox.getElapsedTime().asSeconds() < 0.2f);

    // 1. EVALUACIÓN DE ESTADOS (Máquina de estados visual)
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

    // 2. AVANCE DEL TIEMPO DE ANIMACIÓN CENTRALIZADA
    actualizarAnimacion(dt);

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

void ClaseValkyria::dibujar(sf::RenderWindow& window, Estado estadoActual) {
    if (estadoActual == Estado::Tablero) {
        this->sincronizarPosicionTablero();

        if (this->stats.nombre == "ASSAULT_MARINE" || this->stats.nombre == "GARGOLA") {

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
        if (this->stats.nombre == "ASSAULT_MARINE" || this->stats.nombre == "GARGOLA") {
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
void ClaseValkyria::usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) {
    // El Golem se repara a sí mismo (Heal)
    float curacion = 10.0f;
    this->stats.vida += curacion;

    // Evitamos que se cure por encima de su vida máxima
    if (this->stats.vida > this->stats.vidaMaxima) {
        this->stats.vida = this->stats.vidaMaxima;
    }
}