#pragma once
#include "PiezaTerrestre.h"
#include "Arena.h"
#include "Color.h"
#include <string>
#include <SFML/Graphics.hpp> 

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
        ClaseArcher* clon = new ClaseArcher(*this);

        // Reconectamos los sprites a las texturas que hereda de Pieza / PiezaTerrestre
        clon->spriteTablero.setTexture(clon->texturaTablero);
        clon->spriteArena.setTexture(clon->texturaArena);

        return clon;
    }
    void usarHechizo(std::vector<Hitbox>& hitboxes, Pieza* enemigo) override;
    void procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) override;
    void animar(float dt, sf::Vector2f direccion);
    void dibujar(sf::RenderWindow& window, Estado estadoActual)override;

    void gestionarInvisibilidad(double dt);//sirve para controlar el tiempo que permanece invisible en función del tiempo "dt"
    void gestionarBoostVelocidad(double dt); //controla el tiempo que dura el boost de velocidad
};


