#pragma once
#include "PiezaTeletransporte.h"
#include "Arena.h"
#include "Color.h"
#include <string>
#include <vector>
#include <SFML/Graphics.hpp> 

class ClaseLider : public PiezaTeletransporte {
private:

    std::vector<Pieza*> minionsInvocados;
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

    ClaseLider(Bando b, sf::Vector2i pos, std::string tipo);
    ~ClaseLider();
    Pieza* clonar() const override {
        ClaseLider* clon = new ClaseLider(*this);
        // Reconectamos sus propios sprites a sus propias texturas
        clon->spriteTablero.setTexture(clon->texturaTablero);
        clon->spriteArena.setTexture(clon->texturaArena);
        return clon;
    }

    //FUNCIONES DE MOVIMIENTO  
    void procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) override;
    void usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo)override;
    //FUNCIONES VISUALES
    void dibujar(sf::RenderWindow& window, Estado estadoActual) override;
    void animar(float dt, sf::Vector2f direccion);
    void actualizarMinions(float dt, Arena& arena, Pieza* enemigo)override;
    std::vector<Pieza*>& getMinionsInvocados() override { return minionsInvocados; }
    void limpiarMinions()override;
};
