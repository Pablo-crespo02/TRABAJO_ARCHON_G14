#include "ClaseDjinn.h"
#include <iostream>

ClaseDjinn::ClaseDjinn(Bando b, sf::Vector2i pos, std::string tipo)
    : PiezaTeletransporte(b, pos) // Llama al constructor de la clase intermedia
{
    this->stats.nombre = tipo;
    this->stats.vida = 18.0f;
    this->stats.vidaMaxima = 18.0f;
    this->stats.defensa = 3.0f;
    this->stats.ataque = 15.0f;
    this->stats.velAtaque = 1.2f;
    this->rangoMovimiento = 4;
    this->stats.esRango = true;
    // Asignación del patrón de movimiento
    this->patronMovimiento = PatronMovimiento::Ambos;

    //Tamaño de los sprites:
    this->piezaAlturaArena = 120;
    this->piezaAlturaTablero = 90;

    //CARGA DE SPRITES
    cargarConfigurarSprites(tipo);
     
    //Animación de ataque en función de las posiciones en el spritesheet (APAÑO)
    if (animador) {
        animador->agreganAnimacion("ATAQUE", 1, 2, 2, 0.20f, true);
    }
}
void ClaseDjinn::procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) {
    //Actualizamos la imagen visible con nuestra máquina de estados
    if (this->stats.nombre == "CULEXUS" || this->stats.nombre == "GENESTEALER") {
        Pieza::Animar(dt, direccion);
    }
        
    //Dejamos que la clase padre (PiezaTerrestre) mueva las coordenadas físicas
    PiezaTeletransporte::procesarMovimientoArena(direccion, dt, arena);
}

void ClaseDjinn::dibujar(sf::RenderWindow& window, Estado estadoActual) {
    if (estadoActual == Estado::Tablero) {
        this->sincronizarPosicionTablero();

        if (this->stats.nombre == "CULEXUS" || this->stats.nombre == "GENESTEALER") {

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
    }
    else if (estadoActual == Estado::Arena) {
        if (this->stats.nombre == "CULEXUS" || this->stats.nombre == "GENESTEALER") {
            spriteArena.setPosition(posicionAbsoluta);
            window.draw(spriteArena);
        }
       
        //DIBUJAMOS BARRA DE VIDA SOBRE LA PIEZA

        barrasArena.actualizar(stats.vida, stats.vidaMaxima, stats.velAtaque, posicionAbsoluta);
        barrasArena.dibujar(window);
    }
}


void ClaseDjinn::usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) {
    // Dirección inicial aleatoria para el torbellino
    float anguloInicial = static_cast<float>(std::rand() % 360) * 3.14159f / 180.f;
    sf::Vector2f dirInicial(std::cos(anguloInicial), std::sin(anguloInicial));

    hitboxes.emplace_back(
        this->posicionAbsoluta,      // Origen: la posición del Djinn
        dirInicial,                  // Dirección inicial
        150.0f,                      // Rapidez (se moverá por la arena)
        sf::Color(0, 255, 255, 120), // Color Cian (místico) con transparencia
        this,                        // Propietario
        8.0f,                        // Daño por segundo (DoT)
        10.0f,                       // Dura 10 segundos
        60.0f,                       // Radio del torbellino
        true,                        // Es Daño Continuo (DoT)
        true                         // ¡ES ERRÁTICO!
    );

    std::cout << "El Djinn invoca un Torbellino Erratico!" << std::endl;
}