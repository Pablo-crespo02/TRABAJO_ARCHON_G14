#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

// Estructuras de soporte para colisiones y elementos visuales
struct ColisionCircular {
    sf::Vector2f centro;
    float radio;
};

struct HitboxesArena {
    std::vector<sf::FloatRect> rectangulos;
    std::vector<ColisionCircular> circulos;
};

struct Rejilla {
    sf::RectangleShape marco;
    std::vector<sf::RectangleShape> barras;
};

class Arena {
private:
    const float ANCHO_MAPA = 800.f;
    const float ALTO_MAPA = 600.f;
    const float GROSOR_MURO = 40.f;

    enum class TipoObjeto { Roca, Rejilla, Decoracion };

    sf::RectangleShape suelo;
    sf::View vistaArena;

    // Elementos del escenario
    std::vector<sf::ConvexShape> rocas;
    std::vector<sf::ConvexShape> sombras;
    std::vector<sf::RectangleShape> muros;
    std::vector<sf::RectangleShape> sombrasMuros;
    std::vector<sf::ConvexShape> charcosSangre;
    std::vector<sf::CircleShape> gotasSangre;
    std::vector<Rejilla> rejillasSuelo;

    // Spawns
    sf::CircleShape circuloLuz;
    sf::CircleShape circuloOscuridad;
    sf::FloatRect zonaSpawnLuz;
    sf::FloatRect zonaSpawnOscuridad;

public:
    Arena();

    void prepararSpawns(sf::Color colorAtacante, sf::Color colorDefensor);
    void generarMapaProcedural();
    bool esPosicionValida(sf::Vector2f pos, float radio, bool esVoladora) const;
    bool esGeneracionValida(sf::Vector2f posActual, float radioActual, TipoObjeto tipo);
    void establecerAmbiente(sf::Color colorCasilla);

    // Getters lógicos
    HitboxesArena obtenerColisiones() const;
    sf::View& getVista() { return vistaArena; }
    sf::Vector2f getCentroSpawnLuz() const;
    sf::Vector2f getCentroSpawnOscuridad() const;

    // Permitimos que el Renderizador acceda a los vectores privados para dibujar
    friend class Renderizador;
};
