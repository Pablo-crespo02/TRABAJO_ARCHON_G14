#pragma once
#include "PiezaTeletransporte.h"
#include "Arena.h"
#include "Color.h"
#include <string>
#include <SFML/Graphics.hpp> 

class ClaseDjinn : public PiezaTeletransporte {
private:
   
    //VARIABLES DE ANIMACIÓN (ARENA)
    int frameActual;
    float temporizadorAnimacion;
   


public:
    ClaseDjinn(Bando b, sf::Vector2i pos, std::string tipo);

    Pieza* clonar() const override {
        ClaseDjinn* clon = new ClaseDjinn(this->bando, this->posicionTablero, this->stats.nombre);
        clon->stats.vida = this->stats.vida;
        clon->stats.vidaMaxima = this->stats.vidaMaxima;
        return clon;
    }

    //FUNCIONES DE MOVIMIENTO  
    void procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) override;
    //FUNCIONES VISUALES
    void dibujar(sf::RenderWindow& window, Estado estadoActual) override;
    void usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) override;
};



