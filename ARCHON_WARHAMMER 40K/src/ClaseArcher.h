#pragma once
#include "Pieza.h"
#include "Arena.h"
#include "Color.h"
#include "PiezaTerrestre.h"
#include <string>
#include <vector>

class ClaseArcher : public PiezaTerrestre {
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

    //Control de la invisibilidad (bando luminoso)
    bool esInvisible;
    double temporizadorInvisibilidad;
    double duracionInvisibilidad;

    //Boost de velocidad (bando oscuridad)
    bool tieneBoostVelocidad;
    double temporizadorBoost;
    double duracionBoost;
    double multiplicadorVelocidad;
public:
    ClaseArcher(Bando b, sf::Vector2i pos, std::string tipo);

    Pieza* clonar() const override {
        ClaseArcher* clon = new ClaseArcher(*this);

        // Reconectamos los sprites a las texturas que hereda de Pieza / PiezaTerrestre
        clon->spriteTablero.setTexture(clon->texturaTablero);
        clon->spriteArena.setTexture(clon->texturaArena);

        return clon;
    }
    void usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) override;
    void procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) override;
    void dibujar(sf::RenderWindow& window, Estado estadoActual);
    void gestionarInvisibilidad(double dt);//sirve para controlar el tiempo que permanece invisible en función del tiempo "dt"
    void gestionarBoostVelocidad(double dt); //controla el tiempo que dura el boost de velocidad
};


