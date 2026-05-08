#pragma once
#include <SFML/Graphics.hpp>

class Pieza; // Declaración adelantada para poder usar el puntero

class Hitbox
{
    sf::CircleShape forma;
    sf::Vector2f velocidad;
    bool activo;  //Si está activo se dibuja, si está inactivo el motor lo borra
    Pieza* atacante; // Guardamos LA DIRECCIÓN DE MEMORIA del que dispara en vez del enum
    float dano; //Daño que causa el hitbox
    float tiempoVida; //Tiempo que el hitbox se mantiene activo

public:
    // Constructor actualizado
    Hitbox(sf::Vector2f posicionInicial, sf::Vector2f direccion, double rapidez, sf::Color color, Pieza* propietario, double danoHitbox, double tiempodevida,double radio);

    void ActualizarHitbox(double dt);

    // GETTERS PÚBLICOS:
    sf::CircleShape getFormaHitbox() const { return forma; }
    sf::Vector2f getPosicionHitbox() const { return forma.getPosition(); }
    bool getEstadoHitbox() const { return activo; }
    Pieza* getAtacante() const { return atacante; } // Nuevo getter
    float getDano() const { return dano; }
    sf::Vector2f getVelocidadHitbox() const { return velocidad;}

    // SETTER PÚBLICOS:
    void setEstadoHitbox(bool estadoHitbox) { activo = estadoHitbox; }
};