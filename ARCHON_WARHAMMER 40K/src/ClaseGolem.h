#pragma once
#include "PiezaTerrestre.h"
#include "Arena.h"
#include "Color.h"
#include <string>
#include <SFML/Graphics.hpp> 

class ClaseGolem : public PiezaTerrestre {
private:
   
    //VARIABLES DE ANIMACIÓN (ARENA)
    int frameActual;
    float temporizadorAnimacion;

public:
   
    ClaseGolem(Bando b, sf::Vector2i pos, std::string tipo);

    Pieza* clonar() const override {
        ClaseGolem* clon = new ClaseGolem(this->bando, this->posicionTablero, this->stats.nombre);
        return clon;
    }

    //FUNCIONES DE MOVIMIENTO  
    void procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) override;
    void usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) override;
    //FUNCIONES VISUALES
    void dibujar(sf::RenderWindow& window, Estado estadoActual) override;
    void animar(float dt, sf::Vector2f direccion);
    std::string getDescripcionHechizo() const override {
        if (this->stats.nombre == "DREADNOUGHT") {
            return "SE CURA A SI MISMO";
        }
        return "GENERA UN AURA\nDE ATRACCION";
    }
};