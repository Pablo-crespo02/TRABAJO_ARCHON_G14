#pragma once
#include "PiezaVoladora.h"
#include "Arena.h"
#include "Color.h"
#include <string>
#include <SFML/Graphics.hpp> 

class ClaseValkyria : public PiezaVoladora {
private:
   
    //VARIABLES DE ANIMACIÓN (ARENA)
    int frameActual;
    float temporizadorAnimacion;
    

public:

    ClaseValkyria(Bando b, sf::Vector2i pos, std::string tipo);

    Pieza* clonar() const override {
        ClaseValkyria* clon = new ClaseValkyria(this->bando, this->posicionTablero, this->stats.nombre);
        return clon;
    }

    //FUNCIONES DE MOVIMIENTO  
    void procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) override;
    void usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) override;
    //FUNCIONES VISUALES
    void dibujar(sf::RenderWindow& window, Estado estadoActual) override;
};