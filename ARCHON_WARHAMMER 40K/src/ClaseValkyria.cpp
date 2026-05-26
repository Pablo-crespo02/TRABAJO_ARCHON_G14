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

}

//ENLACE DE FÍSICAS Y ANIMACIÓN
void ClaseValkyria::procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) {
    //Dejamos que la clase padre (PiezaTerrestre) mueva las coordenadas físicas
    PiezaVoladora::procesarMovimientoArena(direccion, dt, arena);

    //Actualizamos la imagen visible con nuestra máquina de estados
    if (this->stats.nombre == "ASSAULT_MARINE" || this->stats.nombre == "GARGOLA") {
        Pieza::Animar(dt, direccion);
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