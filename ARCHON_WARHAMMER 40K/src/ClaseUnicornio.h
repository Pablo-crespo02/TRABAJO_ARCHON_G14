#pragma once
#include "PiezaTerrestre.h"
#include "Arena.h"
#include "Color.h"
#include <string>
#include <vector>


class ClaseUnicornio : public PiezaTerrestre {
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

private:

public:
    ClaseUnicornio(Bando b, sf::Vector2i pos, std::string tipo);

    //FUNCIONES DE MOVIMIENTO  
    void procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) override;
    void usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) override;
    //FUNCIONES VISUALES
    void dibujar(sf::RenderWindow& window, Estado estadoActual) override;
    void animar(float dt, sf::Vector2f direccion);
};