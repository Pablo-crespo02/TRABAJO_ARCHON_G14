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
    float duracionCC; // Cambiado a float para consistencia con SFML

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

    // Constructor (Todos los valores por defecto al final, tipos unificados a float/double según corresponda)
    Hitbox(sf::Vector2f posicionInicial, sf::Vector2f direccion, double rapidez, sf::Color color,
        Pieza* propietario, double danoHitbox, double tiempodevida, double radio,
        bool esDoT = false, bool erratico = false, bool inmoviliza = false, double duracionControl = 0.0,
        bool esGranadaParam = false, double radioExp = 0.0);

    // Asegúrate de que en el .cpp uses exactamente 'double dt' si aquí lo dejas como double
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

    // Fénix
    bool getYaHizoDano() const { return haHechoDano; }
    bool getEsDanoContinuo() const { return esDanoContinuo; }

    // Djinn
    bool getEsErratico() const { return esErratico; }

    // Basilisco
    bool getCausaInmovilizacion() const { return causaInmovilizacion; }
    double getDuracionCC() const { return duracionCC; }

    // Granada
    float getTiempoVuelo() const { return temporizadorVuelo; }

    // SETTERS
    void setEstadoHitbox(bool estadoHitbox) { activo = estadoHitbox; }
    void setYaHizoDano(bool valor) { haHechoDano = valor; }
    void setYaDanoAtacante(bool valor) { yaDanoAtacante = valor; }
    void setYaDanoDefensor(bool valor) { yaDanoDefensor = valor; }
};