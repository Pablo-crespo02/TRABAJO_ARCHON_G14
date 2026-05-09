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
    //hechizo del fenix:
    bool haHechoDano;
    bool esDanoContinuo;
    //hechizo del basilisco:
    bool causaInmovilizacion;
    double duracionCC; // Duración del Crowd Control

public:
    Hitbox(sf::Vector2f posicionInicial, sf::Vector2f direccion, double rapidez, sf::Color color,
        Pieza* propietario, double danoHitbox, double tiempodevida, double radio,
        bool esDoT = false, bool erratico = false, bool inmoviliza = false, double duracionControl = 0.0);
    void ActualizarHitbox(double dt);
    void rebotar();

    //getters
    sf::CircleShape getFormaHitbox() const { return forma; }
    sf::Vector2f getPosicionHitbox() const { return forma.getPosition(); }
    bool getEstadoHitbox() const { return activo; }
    Pieza* getAtacante() const { return atacante; }
    float getDano() const { return dano; }
    sf::Vector2f getVelocidadHitbox() const { return velocidad; }
    //Fenix:
    bool getYaHizoDano() const { return haHechoDano; }
    bool getEsDanoContinuo() const { return esDanoContinuo; }
    //Djinn
    bool getEsErratico() const { return esErratico; }
    //Basilisco:
    bool getCausaInmovilizacion() const { return causaInmovilizacion; }
    double getDuracionCC() const { return duracionCC; }
    // setters
    void setEstadoHitbox(bool estadoHitbox) { activo = estadoHitbox; }
    void setYaHizoDano(bool valor) { haHechoDano = valor; }
};