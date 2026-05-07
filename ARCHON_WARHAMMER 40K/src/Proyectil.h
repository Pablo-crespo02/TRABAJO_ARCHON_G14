#pragma once
#include <SFML/Graphics.hpp>

class Pieza; // Declaración adelantada para poder usar el puntero

class Proyectil
{
    sf::CircleShape forma;
    sf::Vector2f velocidad;
    bool activo;
    Pieza* disparador; // Guardamos LA DIRECCIÓN DE MEMORIA del que dispara en vez del enum
    float dano;

public:
    // Constructor actualizado
    Proyectil(sf::Vector2f posicionInicial, sf::Vector2f direccion, double rapidez, sf::Color color, Pieza* owner, float danoProyectil);

    void ActualizarProyectil();

    // GETTERS PÚBLICOS:
    sf::CircleShape getFormaProyectil() const { return forma; }
    sf::Vector2f getPosicionProyectil() const { return forma.getPosition(); }
    bool getEstadoProyectil() const { return activo; }
    Pieza* getDisparador() const { return disparador; } // Nuevo getter
    float getDano() const { return dano; }

    // SETTER PÚBLICOS:
    void setEstadoProyectil(bool estadoproyectil) { activo = estadoproyectil; }
};