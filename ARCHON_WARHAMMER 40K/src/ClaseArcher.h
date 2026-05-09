#pragma once
#include "Pieza.h"
#include "Arena.h"
#include "Color.h"
#include "PiezaTerrestre.h"
#include <string>
#include <vector>

class ClaseArcher : public PiezaTerrestre {
private:
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
    void usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) override;
    void procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) override;
    void dibujar(sf::RenderWindow& window, Estado estadoActual);
    void gestionarInvisibilidad(double dt);//sirve para controlar el tiempo que permanece invisible en función del tiempo "dt"
    void gestionarBoostVelocidad(double dt); //controla el tiempo que dura el boost de velocidad
};


