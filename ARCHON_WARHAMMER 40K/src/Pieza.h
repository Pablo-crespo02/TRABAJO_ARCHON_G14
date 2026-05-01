#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

enum class Bando { LUZ, OSCURIDAD };
enum class TipoMovimiento { TERRESTRE, VOLADOR };
enum class PatronMovimiento { ORTOGONAL, DIAGONAL, AMBOS };

struct Stats {
    float vida;
    float cooldown;
    bool esRango; // true = rango, false = melée
    int ataque;
    int defensa;
    float velAtaque;
};

class Pieza {
private:
    Bando bando;
    TipoMovimiento tipoMov;
    PatronMovimiento patron;
    int rangoMovimiento;
    Stats stats;
    bool seleccionado = false;

    sf::Vector2i posicionTablero; // Coordenadas (0-8, 0-8)
    sf::CircleShape formaVisual;  // Representación temporal

public:
    // Constructor vacío (inicializa con valores por defecto)
    Pieza();

    // Métodos lógicos
    bool poderMover(sf::Vector2i destino, const std::vector<Pieza*>& otrasPiezas, bool esDestinoOcupado);
    void mover(sf::Vector2i destino);
    void setSeleccionada(bool valor) { seleccionado = valor; };
    bool getEstaSeleccionada() const { return seleccionado; };
    // Métodos de flujo de batalla
    void animacionMover();
    bool detectarConflicto(const std::vector<Pieza*>& otrasPiezas);
    void iniciarConflicto();

    void setBando(Bando nuevoBando) { bando = nuevoBando; }

    void setPosicion(sf::Vector2i nuevaPos) { posicionTablero = nuevaPos; }

    void setTipoMov(TipoMovimiento nuevoTipo) { tipoMov = nuevoTipo; }

    void setPatronMov(PatronMovimiento nuevoTipo) { patron = nuevoTipo; }

    void setRango(int nuevoRango) { rangoMovimiento = nuevoRango; }

    void setColor(sf::Color color) { formaVisual.setFillColor(color); }

    Bando getBando() const {
        return bando;
    }

    sf::Vector2i getPosicion() const { return posicionTablero; }

    void dibujar(sf::RenderWindow& window);
};

