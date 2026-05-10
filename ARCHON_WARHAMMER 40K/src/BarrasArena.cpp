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

    // Cambiar color de vida según salud
    if (porcentajeVida > 0.5f) barraActual.setFillColor(sf::Color::Green);
    else if (porcentajeVida > 0.25f) barraActual.setFillColor(sf::Color::Yellow);
    else barraActual.setFillColor(sf::Color::Red);

    // --- 2. LÓGICA DE ATAQUE ---
    float tiempoTranscurrido = relojInterno.getElapsedTime().asSeconds();
    float porcentajeAtaque = (velAtaque > 0) ? (tiempoTranscurrido / velAtaque) : 1.f;

    if (porcentajeAtaque >= 1.f) {
        porcentajeAtaque = 1.f; // Se queda llena
        barraAtaque.setFillColor(sf::Color(200, 200, 200)); // Gris claro: ¡LISTO!
    }
    else {
        // Mientras carga, usamos el gris oscuro que querías
        barraAtaque.setFillColor(sf::Color(100, 100, 100, 180));
    }

    barraAtaque.setSize(sf::Vector2f(anchoMaximo * porcentajeAtaque, alto / 3.f));

    // --- 3. POSICIONAMIENTO ---
    float offsetX = anchoMaximo / 2.f;
    // Posicionamos el conjunto (puedes ajustar el 35.f según el tamaño de tus piezas)
    fondo.setPosition(posicionPieza.x - offsetX, posicionPieza.y - 35.f);
    barraActual.setPosition(posicionPieza.x - offsetX, posicionPieza.y - 35.f);

    // La de ataque la ponemos justo debajo de la de vida
    barraAtaque.setPosition(posicionPieza.x - offsetX, posicionPieza.y - 35.f + alto + 2.f);
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