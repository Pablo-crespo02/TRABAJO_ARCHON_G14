#pragma once
#include "PiezaTerrestre.h"
#include "Arena.h"
#include "Color.h"
#include <string>
#include <SFML/Graphics.hpp> 

class ClaseArcher : public PiezaTerrestre {
private:

    // VARIABLES DE ANIMACIÓN (ARENA)
    int frameActual;
    float temporizadorAnimacion;
    
    // --- VARIABLES DE INVISIBILIDAD (LICTOR - OSCURIDAD) ---
    bool esInvisible;
    bool preparandoInvisibilidad; // Nuevo: rastrea si está en el medio segundo de carga
    double temporizadorInvisibilidad;

    // --- VARIABLES DE BOOST (LUZ) ---
    bool tieneBoostVelocidad;
    double temporizadorBoost;
    float multiplicadorVelocidad;

public:
    ClaseArcher(Bando b, sf::Vector2i pos, std::string tipo);

    Pieza* clonar() const override {
        ClaseArcher* clon = new ClaseArcher(this->bando, this->posicionTablero, this->stats.nombre);
        return clon;
    }
    // FUNCIONES VISUALES
    void usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) override;
    void procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) override;
    //FUNCIONES VISUALES
    void animar(float dt, sf::Vector2f direccion);
    void dibujar(sf::RenderWindow& window, Estado estadoActual)override;
    std::string getDescripcionHechizo() const override {
        if (this->stats.nombre == "VINDICARE") {
            return "AUMENTA DRASTICAMENTE SU\nVELOCIDAD DE MOVIMIENTO ";
        }
        return "SE VUELVE INVISIBLE \nDURANTE UN BREVE PERIODO";
    }

    void gestionarInvisibilidad(double dt);//sirve para controlar el tiempo que permanece invisible en función del tiempo "dt"
    void gestionarBoostVelocidad(double dt); //controla el tiempo que dura el boost de velocidad
};


