#pragma once
#include "PiezaTerrestre.h"
#include "Arena.h"
#include "Color.h"
#include <string>
#include <SFML/Graphics.hpp>

class ClaseKnight : public PiezaTerrestre {
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

    ClaseKnight(Bando b, sf::Vector2i pos, std::string tipo);

    //FUNCIONES DE MOVIMIENTO
    void procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) override;

    void usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) override;
    //FUNCIONES VISUALES
    void animar(float dt, sf::Vector2f direccion);

    void dibujar(sf::RenderWindow& window, Estado estadoActual) override;
};