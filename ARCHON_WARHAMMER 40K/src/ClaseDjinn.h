#pragma once
#include "PiezaTeletransporte.h"
#include "Arena.h"
#include "Color.h"
#include <string>
#include <SFML/Graphics.hpp> 

class ClaseDjinn : public PiezaTeletransporte {
private:
    //SPRITES Y TEXTURAS 
    sf::Texture texturaTablero;
    sf::Sprite spriteTablero;

    sf::Texture texturaArena;
    sf::Sprite spriteArena;

    //VARIABLES DE ANIMACIÓN (ARENA)
    int frameActual;
    float temporizadorAnimacion;
    int anchoFrame;
    int altoFrame;


public:
    ClaseDjinn(Bando b, sf::Vector2i pos, std::string tipo);

    Pieza* clonar() const override {
        ClaseDjinn* clon = new ClaseDjinn(*this);
        // Reconectamos sus propios sprites a sus propias texturas
        clon->spriteTablero.setTexture(clon->texturaTablero);
        clon->spriteArena.setTexture(clon->texturaArena);
        return clon;
    }

    //FUNCIONES DE MOVIMIENTO  
    void procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) override;
    //FUNCIONES VISUALES
    void dibujar(sf::RenderWindow& window, Estado estadoActual) override;
    void animar(float dt, sf::Vector2f direccion);
    void usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) override;
};



