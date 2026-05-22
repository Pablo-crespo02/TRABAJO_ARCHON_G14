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
   
    //VARIABLES DE ANIMACIÓN (ARENA)
    int frameActual;
    float temporizadorAnimacion;
    
public:

    ClaseLider(Bando b, sf::Vector2i pos, std::string tipo);
    ~ClaseLider();
    Pieza* clonar() const override {
        ClaseLider* clon = new ClaseLider(this->bando, this->posicionTablero, this->stats.nombre);
        return clon;
    }

    //FUNCIONES DE MOVIMIENTO  
    void procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) override;
    void usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo)override;
    //FUNCIONES VISUALES
    void dibujar(sf::RenderWindow& window, Estado estadoActual) override;
    void animar(float dt, sf::Vector2f direccion);

    //Añadimos std::vector<Hitbox>& hitboxes para que el helicóptero dispare 
    void actualizarMinions(float dt, Arena& arena, Pieza* enemigo, std::vector<Hitbox>& hitboxes);

    std::vector<Pieza*>& getMinionsInvocados() override { return minionsInvocados; }
    void limpiarMinions() override;
};
