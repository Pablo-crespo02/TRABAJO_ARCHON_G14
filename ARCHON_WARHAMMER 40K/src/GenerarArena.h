#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Tablero.h"
#include"Color.h"

// --- CLASE SPAWNPOINT (Definición ligera) ---
class SpawnPoint {
private:
    sf::FloatRect area;
    sf::CircleShape circuloDecorativo;

public:
    SpawnPoint(sf::Vector2f pos, float radio, sf::Color colorBando) {
        area = sf::FloatRect(pos.x - radio, pos.y - radio, radio * 2.f, radio * 2.f);

        circuloDecorativo.setRadius(radio);
        circuloDecorativo.setOrigin(radio, radio);
        circuloDecorativo.setPosition(pos);
        circuloDecorativo.setOutlineThickness(1.f);
        circuloDecorativo.setOutlineColor(colorBando);
        circuloDecorativo.setFillColor(sf::Color::Transparent);
    }

    bool contienePunto(sf::Vector2f punto, float margen) const {
        sf::FloatRect areaSegura = area;
        areaSegura.left -= margen;
        areaSegura.top -= margen;
        areaSegura.width += margen * 2;
        areaSegura.height += margen * 2;
        return areaSegura.contains(punto);
    }

    void dibujar(sf::RenderWindow& window) const {
        window.draw(circuloDecorativo);
    }
};

// --- CLASE GENERADORARENA ---
// Adelantamos la declaración de Arena para que no haya errores
class Arena;

class GeneradorArena {
public:
    // La función principal que llamaremos desde el Motor
    static void generarMapa(Arena& arena, ColorActual colorCasilla);

private:
    // Pasamos la lógica de muros y spawns aquí
    static void prepararSpawns(Arena& arena);

    static void generarRejillas(Arena& arena);
    static void generarRocas(Arena& arena);
    static void generarSangre(Arena& arena);

    static bool esPosicionValida(sf::Vector2f pos, float radio, const Arena& arena);
    static bool sonColoresIguales(sf::Color c1, sf::Color c2) {
        return (c1.r == c2.r && c1.g == c2.g && c1.b == c2.b);
    }
    static bool esPosicionSangreValida(sf::Vector2f pos, float radio, const Arena& arena);
};