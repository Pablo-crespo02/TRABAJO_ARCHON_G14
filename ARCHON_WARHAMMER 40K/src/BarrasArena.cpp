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
    float vida = std::max(0.f, vidaActual);
    float porcentajeVida = (vidaMaxima > 0) ? vida / vidaMaxima : 0;
    barraActual.setSize(sf::Vector2f(anchoMaximo * porcentajeVida, alto));

    // Colores salud
    if (porcentajeVida > 0.5f) barraActual.setFillColor(sf::Color::Green);
    else if (porcentajeVida > 0.25f) barraActual.setFillColor(sf::Color::Yellow);
    else barraActual.setFillColor(sf::Color::Red);

    // --- LÓGICA DE ATAQUE ---
    float carga = std::max(0.f, cargaAtaque);
    float porcentajeAtaque = (ataqueMaximo > 0) ? carga / ataqueMaximo : 0;

    if (porcentajeAtaque > 1.f) {
        porcentajeAtaque = 1.f;
    }

    barraAtaque.setSize(sf::Vector2f(anchoMaximo * porcentajeAtaque, alto / 3.f));

    // --- POSICIONAMIENTO ---
    float offsetX = anchoMaximo / 2.f;
    float offsetY = 35.f;

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

    // La barra de ataque la ponemos justo pegada debajo de la de salud (alto + 2px de margen)
    barraAtaque.setPosition(posicionPieza.x - offsetX, posicionPieza.y - offsetY + alto + 2.f);
}

void BarrasArena::dibujar(sf::RenderWindow& window) const {
    window.draw(fondo);
    window.draw(barraActual);
    window.draw(barraAtaque); // <-- Dibujamos la nueva barra
}