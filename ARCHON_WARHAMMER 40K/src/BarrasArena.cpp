#include "BarraVida.h"
#include <algorithm> // Necesario para std::max

BarraVida::BarraVida(float ancho, float alto) : anchoMaximo(ancho), alto(alto) {
    // Configuración del fondo (la parte vacía de la barra)
    fondo.setSize(sf::Vector2f(ancho, alto));
    fondo.setFillColor(sf::Color(50, 50, 50, 200)); // Gris oscuro semitransparente
    fondo.setOutlineThickness(1.f);
    fondo.setOutlineColor(sf::Color::Black);

    // Configuración de la barra de salud (la parte llena)
    barraActual.setSize(sf::Vector2f(ancho, alto));
    barraActual.setFillColor(sf::Color::Green);
}

void BarraVida::actualizar(float vidaActual, float vidaMaxima, sf::Vector2f posicionPieza) {
    if (vidaMaxima <= 0.f) return; // Evitar divisiones por cero

    // Evitamos que la vida baje de 0 visualmente
    float vida = std::max(0.f, vidaActual);

    // Calcular el porcentaje de salud restante
    float porcentaje = vida / vidaMaxima;

    // Escalar la anchura del rectángulo verde según el porcentaje
    barraActual.setSize(sf::Vector2f(anchoMaximo * porcentaje, alto));

    // Lógica dinámica de colores
    if (porcentaje > 0.5f) {
        barraActual.setFillColor(sf::Color::Green);
    }
    else if (porcentaje > 0.25f) {
        barraActual.setFillColor(sf::Color::Yellow);
    }
    else {
        barraActual.setFillColor(sf::Color::Red);
    }

    // Posicionamiento: centramos la barra y la subimos por encima de la pieza
    float offsetX = anchoMaximo / 2.f;
    float offsetY = 35.f; // La pieza tiene radio 20, así que 35 la coloca justo encima

    fondo.setPosition(posicionPieza.x - offsetX, posicionPieza.y - offsetY);
    barraActual.setPosition(posicionPieza.x - offsetX, posicionPieza.y - offsetY);
}

void BarraVida::dibujar(sf::RenderWindow& window) const {
    window.draw(fondo);
    window.draw(barraActual);
}