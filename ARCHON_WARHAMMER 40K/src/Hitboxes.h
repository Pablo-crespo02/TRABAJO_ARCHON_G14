#pragma once
#include <SFML/Graphics.hpp>

class Pieza;

class Hitbox {
private: // Es buena práctica encapsular esto
    sf::CircleShape forma;
    sf::Vector2f velocidad;
    bool activo;
    Pieza* atacante;
    float dano;
    float tiempoVida;
    bool esErratico;            // Para el torbellino del djinn
    float tiempoCambioDir;      // Temporizador interno para el movimiento errático
    float rapidezOriginal;      // Guardamos la rapidez para los cambios de dirección
    
    bool haHechoDano;//Para el fuego del fenix
    bool esDanoContinuo;

public:
    Hitbox(sf::Vector2f posicionInicial, sf::Vector2f direccion, double rapidez, sf::Color color,
        Pieza* propietario, double danoHitbox, double tiempodevida, double radio,
        bool esDoT = false, bool erratico = false);

    void ActualizarHitbox(double dt);
    void rebotar();

    //getters
    sf::CircleShape getFormaHitbox() const { return forma; }
    sf::Vector2f getPosicionHitbox() const { return forma.getPosition(); }
    bool getEstadoHitbox() const { return activo; }
    Pieza* getAtacante() const { return atacante; }
    float getDano() const { return dano; }
    sf::Vector2f getVelocidadHitbox() const { return velocidad; }
    bool getYaHizoDano() const { return haHechoDano; }
    bool getEsDanoContinuo() const { return esDanoContinuo; }//Fenix
    bool getEsErratico() const { return esErratico; }//Djinn
    // setters
    void setEstadoHitbox(bool estadoHitbox) { activo = estadoHitbox; }
    void setYaHizoDano(bool valor) { haHechoDano = valor; }
};