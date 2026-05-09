#pragma once
#include <SFML/Graphics.hpp>

class BarraVida {
private:
    sf::RectangleShape fondo;
    sf::RectangleShape barraActual;
    float anchoMaximo;
    float alto;

public:
    // El constructor define un tamaño estándar (40x6) que encaja con el radio 20 de tus piezas
    BarraVida(float ancho = 40.f, float alto = 6.f);

    // Recibe la vida, el máximo y las coordenadas de la pieza para dibujarse justo encima
    void actualizar(float vidaActual, float vidaMaxima, sf::Vector2f posicionPieza);

    void dibujar(sf::RenderWindow& window) const;
};