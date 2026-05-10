#pragma once
#include <SFML/Graphics.hpp>

class BarrasArena {
private:
    sf::RectangleShape fondo;
    sf::RectangleShape barraActual;
    sf::RectangleShape barraAtaque;
    sf::Clock relojInterno;
    sf::RectangleShape barraAtaque; // <-- Nueva barra
    float anchoMaximo;
    float alto;

public:
    BarrasArena(float ancho = 40.f, float alto = 6.f);
    // Actualiza la barra pero NO la reinicia sola
    void actualizar(float vidaActual, float vidaMaxima, float velAtaque, sf::Vector2f posicionPieza);

    // Este es el método que llamará el Golem justo cuando ataque
    void reiniciarRecarga();

    // Ahora pedimos también el progreso del ataque (de 0.0 a 1.0 o según tu lógica)
    void actualizar(float vidaActual, float vidaMaxima, float cargaAtaque, float ataqueMaximo, sf::Vector2f posicionPieza);

    void dibujar(sf::RenderWindow& window) const;
};