
#include "BarrasArena.h"
#include <algorithm> // Para std::max

// Constructor
BarrasArena::BarrasArena(float ancho, float alto) : anchoMaximo(ancho), alto(alto) {
    // Fondo de salud
    fondo.setSize(sf::Vector2f(ancho, alto));
    fondo.setFillColor(sf::Color(50, 50, 50, 200));

    // Barra de salud
    barraActual.setSize(sf::Vector2f(ancho, alto));
    barraActual.setFillColor(sf::Color::Green);

    // Barra de ataque (más delgada, color azul/cian)
    barraAtaque.setSize(sf::Vector2f(0.f, alto / 2.f));
    barraAtaque.setFillColor(sf::Color::Cyan);
}

// Función Actualizar
void BarrasArena::actualizar(float vidaActual, float vidaMaxima, float velAtaque, sf::Vector2f posicionPieza) {
    // --- 1. LÓGICA DE VIDA ---
    float porcentajeVida = (vidaMaxima > 0) ? (std::max(0.f, vidaActual) / vidaMaxima) : 0.f;
    barraActual.setSize(sf::Vector2f(anchoMaximo * porcentajeVida, alto));

    // --- 2. LÓGICA DE ATAQUE (Se detiene al final) ---
    float tiempoTranscurrido = relojInterno.getElapsedTime().asSeconds();
    float porcentajeAtaque = (velAtaque > 0) ? (tiempoTranscurrido / velAtaque) : 1.f;

    // CRUCIAL: Bloqueamos el porcentaje en 1.0 (100%) para que se quede llena
    if (porcentajeAtaque > 1.f) {
        porcentajeAtaque = 1.f;
        barraAtaque.setFillColor(sf::Color(150, 150, 150)); // Color de "Listo"
    }
    else {
        barraAtaque.setFillColor(sf::Color(90, 90, 90)); // Color de "Cargando"
    }

    barraAtaque.setSize(sf::Vector2f(anchoMaximo * porcentajeAtaque, alto / 3.f));

    // --- 3. POSICIONAMIENTO ---
    float offsetX = anchoMaximo / 2.f;
    float offsetY = 35.f;
    fondo.setPosition(posicionPieza.x - offsetX, posicionPieza.y - offsetY);
    barraActual.setPosition(posicionPieza.x - offsetX, posicionPieza.y - offsetY);
    barraAtaque.setPosition(posicionPieza.x - offsetX, posicionPieza.y - offsetY + alto + 2.f);
}

// La pieza llama a esto manualmente tras disparar/golpear
void BarrasArena::reiniciarRecarga() {
    relojInterno.restart();
}

// Función Dibujar
void BarrasArena::dibujar(sf::RenderWindow& window) const {
    window.draw(fondo);
    window.draw(barraActual);
    window.draw(barraAtaque);
}