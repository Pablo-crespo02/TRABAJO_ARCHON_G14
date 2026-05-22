#pragma once
#include "PiezaTerrestre.h"
#include "Arena.h"
#include "Color.h"
#include <string>
#include <SFML/Graphics.hpp>
//ESPADITA
class ClaseKnight : public PiezaTerrestre {
private:
    
    //VARIABLES DE ANIMACIÓN (ARENA)
    int frameActual;
    float temporizadorAnimacion;
    
public:

    ClaseKnight(Bando b, sf::Vector2i pos, std::string tipo);

    Pieza* clonar() const override {
        ClaseKnight* clon = new ClaseKnight(this->bando, this->posicionTablero, this->stats.nombre);
        return clon;
    }
    //FUNCIONES DE MOVIMIENTO
    void procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) override;

    void usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) override;
    //FUNCIONES VISUALES
    void dibujar(sf::RenderWindow& window, Estado estadoActual) override;

    void animar(float dt, sf::Vector2f direccion);
    
    //PARA QUE LOS MINIONS INVOCADOS SEAN MAS PEQUEÑOS
    void setEscalaMinion(float porcentaje) {
        // Obtenemos la escala que ya calculó el constructor original
        // y la multiplicamos por el porcentaje (ej: 0.6f para un 60% del tamaño)
        sf::Vector2f escalaActual = spriteArena.getScale();
        spriteArena.setScale(escalaActual.x * porcentaje, escalaActual.y * porcentaje);
    }
};