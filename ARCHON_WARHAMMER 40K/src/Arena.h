#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "GenerarArena.h"

// --- CONSTANTES ---
const float ANCHO_MAPA = 800.f;
const float ALTO_MAPA = 600.f;
const float GROSOR_MURO = 20.f;

// --- TIPOS AUXILIARES ---
enum class TipoObjeto { Roca, Item, Decoracion };

struct HitboxCircular {
    sf::Vector2f centro;
    float radio;
};

struct HitboxesArena {
    std::vector<sf::FloatRect> rectangulos; // Para muros
    std::vector<HitboxCircular> circulos;   // Para rocas/obstáculos
};

struct Rejilla {
    sf::RectangleShape marco;
    std::vector<sf::RectangleShape> barras;
};

// Declaración adelantada para no romper la inclusión circular
class SpawnPoint;

class Arena {
private:
    // Elementos visuales base
    sf::RectangleShape suelo;
    sf::View vistaArena;

    // Contenedores de objetos
    std::vector<SpawnPoint> spawns;
    std::vector<sf::ConvexShape> rocas;
    std::vector<sf::ConvexShape> sombras;
    std::vector<sf::RectangleShape> muros;
    std::vector<sf::RectangleShape> sombrasMuros;
    std::vector<Rejilla> rejillasSuelo;
    std::vector<sf::ConvexShape> charcosSangre;
    std::vector<sf::CircleShape> gotasSangre;

public:
    Arena(); // Constructor

    // --- Lógica de Colisiones ---
    HitboxesArena obtenerColisiones() const;
    bool esPosicionValida(sf::Vector2f pos, float radio, bool esVoladora = false) const;
    bool esGeneracionValida(sf::Vector2f posActual, float radioActual, TipoObjeto tipo);

    // --- Getters ---
    const std::vector<SpawnPoint>& getSpawns() const { return spawns; }
    const std::vector<sf::ConvexShape>& getRocas() const { return rocas; }
    const std::vector<Rejilla>& getRejillas() const { return rejillasSuelo; }
    const std::vector<sf::ConvexShape>& getCharcos() const { return charcosSangre; }
    sf::View getVista() const { return vistaArena; }

    // --- Adders (Inyectores de datos) ---
    void addSpawnPoint(const SpawnPoint& sp) { spawns.push_back(sp); }
    void addRoca(const sf::ConvexShape& roca, const sf::ConvexShape& sombra) {
        rocas.push_back(roca); sombras.push_back(sombra);
    }
    void addMuro(const sf::RectangleShape& muro, const sf::RectangleShape& sombra) {
        muros.push_back(muro); sombrasMuros.push_back(sombra);
    }
    void addRejilla(const Rejilla& r) { rejillasSuelo.push_back(r); }
    void addCharcoSangre(const sf::ConvexShape& c) { charcosSangre.push_back(c); }
    void addGotaSangre(const sf::CircleShape& g) { gotasSangre.push_back(g); }

    // --- Otros ---
    void establecerAmbiente(sf::Color colorCasilla);
    void limpiarTodo() {
        spawns.clear();
        rocas.clear();
        sombras.clear();
        muros.clear();
        sombrasMuros.clear();
        rejillasSuelo.clear();
        charcosSangre.clear();
        gotasSangre.clear();
    }
    void dibujar(sf::RenderWindow& window) const;
};