#pragma once
#include <SFML/Graphics.hpp>
#include "Arena.h"
#include "EstadoJuego.h"
#include <vector>
#include <string>

// Enums básicos para todo el juego
enum class Bando { LUZ, OSCURIDAD };

// Enum para leer los patrones de movimiento fácilmente
enum class PatronMovimiento {
    Ortogonal,
    Diagonal,
    Ambos,
    Libre     // Para teletransporte
};

// Estructura de estadísticas que heredará cada pieza
struct Stats {
    std::string nombre;
    float vida;
    float cooldown;
    bool esRango;
    float ataque;
    float defensa;
    float velAtaque;
};

class Pieza {
protected:
    // Atributos protegidos: las clases hijas pueden usarlos directamente
    Bando bando;
    int rangoMovimiento;
    PatronMovimiento patronMovimiento; 
    Stats stats;
    bool seleccionado;

    sf::Vector2i posicionTablero;
    sf::Vector2f posicionAbsoluta;
    sf::CircleShape formaVisual;

    // Amistades
    friend class Motor;
    friend class Renderizador;
    friend class Generador;

public:
    // Constructor
    Pieza(Bando b, sf::Vector2i pos);

    // Destructor Virtual: CRÍTICO al usar herencia para evitar fugas de memoria
    virtual ~Pieza() = default;

    // Métodos Virtuales Puros: Obligan a las clases hijas a implementarlos[cite: 1]
    virtual bool poderMover(sf::Vector2i destino, const std::vector<Pieza*>& otrasPiezas, bool esDestinoOcupado) = 0;
    virtual void procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) = 0;
    virtual void dibujar(sf::RenderWindow& window, Estado estadoActual) = 0;

    // Métodos Comunes: Lógica igual para todos
    void mover(sf::Vector2i destino);
    void moverEnArena(float dx, float dy);
    void sincronizarPosicionTablero();
    bool detectarConflicto(const std::vector<Pieza*>& otrasPiezas);

    // Getters
    sf::Vector2i getPosicionTablero() const { return posicionTablero; }
    Bando getBando() const { return bando; }
    sf::Color getColorVisual() const { return formaVisual.getFillColor(); }
    sf::FloatRect getHitbox() const {
        return sf::FloatRect(posicionAbsoluta.x - 15.f, posicionAbsoluta.y - 15.f, 30.f, 30.f);
    }

    // Setters
    void setSeleccionado(bool valor) { seleccionado = valor; }
    void setPosicionAbsoluta(sf::Vector2f nuevaPos) {
        posicionAbsoluta = nuevaPos;
        formaVisual.setPosition(posicionAbsoluta);
    }
};