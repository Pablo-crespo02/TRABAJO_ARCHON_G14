#pragma once
#include "Pieza.h"
#include "PiezaVoladora.h"
#include "Arena.h"
#include "Color.h"
#include "Hitboxes.h" // Asegúrate de tener este include para las hitboxes
#include <string>
#include <SFML/Graphics.hpp>

class ClaseFenix : public PiezaVoladora {
private:
    // SPRITES Y TEXTURAS 
    sf::Texture texturaTablero;
    sf::Sprite spriteTablero;

    sf::Texture texturaArena;
    sf::Sprite spriteArena;

    // VARIABLES DE ANIMACIÓN (ARENA)
    int frameActual;
    float temporizadorAnimacion;
    int anchoFrame;
    int altoFrame;

public:
    ClaseFenix(Bando b, sf::Vector2i pos, std::string tipo);

    // FUNCIONES DE LÓGICA Y MOVIMIENTO
    void usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo);
    void procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) override;

    // FUNCIONES VISUALES
    void dibujar(sf::RenderWindow& window, Estado estadoActual) override;
    void animar(float dt, sf::Vector2f direccion);
};
