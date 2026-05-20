#pragma once
#include <SFML/Graphics.hpp>

class Pieza;

class Hitbox {
private:
    sf::CircleShape forma;
    sf::Vector2f velocidad;
    bool activo;
    Pieza* atacante;
    float dano;
    float tiempoVida;

    // Mecánica: Djinn
    bool esErratico;
    float tiempoCambioDir;
    float rapidezOriginal;

    // Mecánica: Fénix
    bool haHechoDano;
    bool esDanoContinuo;

    // Mecánica: Basilisco
    bool causaInmovilizacion;
    float duracionCC;

    // Mecánica: Granada
    float radioExplosion;
    float danoExplosion;
    float temporizadorVuelo;
    float temporizadorSuelo;
    bool estaEnSuelo;
    bool explotando;
    bool yaDanoAtacante;
    bool yaDanoDefensor;

public:
    // Atributo público
    bool esGranada;

    // Constructor 
    Hitbox(sf::Vector2f posicionInicial, sf::Vector2f direccion, double rapidez, sf::Color color,
        Pieza* propietario, double danoHitbox, double tiempodevida, double radio,
        bool esDoT = false, bool erratico = false, bool inmoviliza = false, double duracionControl = 0.0,
        bool esGranadaParam = false, double radioExp = 0.0);

    void ActualizarHitbox(double dt);
    void rebotar();
    void Detonar();

    // GETTERS
    const sf::CircleShape& getFormaHitbox() const { return forma; }
    sf::Vector2f getPosicionHitbox() const { return forma.getPosition(); }
    bool getEstadoHitbox() const { return activo; }
    Pieza* getAtacante() const { return atacante; }
    float getDano() const { return dano; }
    sf::Vector2f getVelocidadHitbox() const { return velocidad; }
    bool getYaDanoAtacante() const { return yaDanoAtacante; }
    bool getYaDanoDefensor() const { return yaDanoDefensor; }

    // AQUÍ ESTÁ EL GETTER QUE PEDÍA EL MOTOR PARA LA GRANADA
    float getTiempoVuelo() const { return temporizadorVuelo; }

    // Fénix
    bool getYaHizoDano() const { return haHechoDano; }
    bool getEsDanoContinuo() const { return esDanoContinuo; }

    // Djinn
    bool getEsErratico() const { return esErratico; }

    // Basilisco
    bool getCausaInmovilizacion() const { return causaInmovilizacion; }
    double getDuracionCC() const { return duracionCC; }
};