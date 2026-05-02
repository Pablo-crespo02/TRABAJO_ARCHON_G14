#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Arena.h"

enum class Bando { LUZ, OSCURIDAD };
enum class TipoMovimiento { TERRESTRE, VOLADOR };
enum class PatronMovimiento { ORTOGONAL, DIAGONAL, AMBOS };

struct Stats {
    std::string nombre;
    float vida;
    float cooldown;
    bool esRango; // true = rango, false = melée
    float ataque;
    float defensa;
    float velAtaque;
};

class Pieza {
private:
    // Atributos privados que ahora son accesibles para sus "amigos"
    Bando bando;
    TipoMovimiento tipoMov;
    PatronMovimiento patron;
    int rangoMovimiento;
    Stats stats;


    bool seleccionado = false;
    sf::Vector2i posicionTablero;  // Coordenadas (0-8) para el tablero
    sf::Vector2f posicionAbsoluta; // Coordenadas (píxeles) para la Arena y suavizado
    sf::CircleShape formaVisual;

    // DECLARACIÓN DE AMISTAD
    friend class Motor;
    friend class Renderizador;
    friend class Generador; // También útil para la creación inicial

public:
    Pieza();

    // Solo dejamos los métodos que tienen LÓGICA o COMPORTAMIENTO
    bool poderMover(sf::Vector2i destino, const std::vector<Pieza*>& otrasPiezas, bool esDestinoOcupado);
    void mover(sf::Vector2i destino);
    void moverEnArena(float dx, float dy);
    sf::FloatRect getHitbox() const {
        return sf::FloatRect(posicionAbsoluta.x - 15.f, posicionAbsoluta.y - 15.f, 30.f, 30.f);
    }
    void sincronizarPosicionTablero();
    // Detectar si choca con otras piezas en el tablero
    bool detectarConflicto(const std::vector<Pieza*>& otrasPiezas);
};

