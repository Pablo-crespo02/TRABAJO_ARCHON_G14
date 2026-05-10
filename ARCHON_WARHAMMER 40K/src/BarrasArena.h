#pragma once
#include <SFML/Graphics.hpp>

class BarrasArena {
private:
    sf::RectangleShape fondo;
    sf::RectangleShape barraActual;
    sf::RectangleShape barraAtaque;
    sf::Clock relojInterno;
    float anchoMaximo;
    float alto;

public:
    BarrasArena(float ancho = 40.f, float alto = 6.f);
    // Actualiza la barra pero NO la reinicia sola
    void actualizar(float vidaActual, float vidaMaxima, float velAtaque, sf::Vector2f posicionPieza);

    // Este es el método que llamará el Golem justo cuando ataque
    void reiniciarRecarga();

    void dibujar(sf::RenderWindow& window) const;
};