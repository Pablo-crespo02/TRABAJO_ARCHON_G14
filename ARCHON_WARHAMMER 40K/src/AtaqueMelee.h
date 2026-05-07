#pragma once
#include <SFML/Graphics.hpp>

class Pieza; // Declaramos pieza

class AtaqueMelee {
    sf::CircleShape forma;
    float tiempoDeVida;    // Temporizador (cuanto dura el golpe en pantalla)
    bool activo;
    Pieza* atacante;
    float dano;

public:
    AtaqueMelee(sf::Vector2f posicion, Pieza* owner, float danoAtaque);

    // Recibe el delta time (dt) para ir restando su tiempo de vida
    void actualizar(float dt);

    // Getters
    const sf::CircleShape& getForma() const { return forma; }
    sf::Vector2f getPosicion() const { return forma.getPosition(); }
    bool getEstado() const { return activo; }
    Pieza* getAtacante() const { return atacante; }
    float getDano() const { return dano; }

    // Setter
    void setEstado(bool est) { activo = est; }
};