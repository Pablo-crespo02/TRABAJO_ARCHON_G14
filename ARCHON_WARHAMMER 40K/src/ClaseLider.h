#pragma once
#include "PiezaTeletransporte.h"
#include "Arena.h"
#include "Color.h"
#include <string>
#include <vector>
#include <SFML/Graphics.hpp> 
#include <SFML/Audio.hpp>

class ClaseLider : public PiezaTeletransporte {
private:

    std::vector<Pieza*> minionsInvocados;
   
    //Variables de animación (ARENA)
    int frameActual;
    float temporizadorAnimacion;

    //Sonidos para los minions
    sf::SoundBuffer bufferMinionXeno;
    sf::Sound sonidoMinionXeno;

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

    std::string getDescripcionHechizo() const override {
        if (this->stats.nombre == "CAPTAIN") {
            return "SOLICITA APOYO AEREO\nINVOCA A UN HELICOPTERO";
        }
        return "INVOCA 2 SIERVOS PARA\nQUE LO PROTEJAN";
    }

    //Añadimos std::vector<Hitbox>& hitboxes para que el helicóptero dispare 
    void actualizarMinions(float dt, Arena& arena, Pieza* enemigo, std::vector<Hitbox>& hitboxes);

    std::vector<Pieza*>& getMinionsInvocados() override { return minionsInvocados; }
    void limpiarMinions() override;
};
