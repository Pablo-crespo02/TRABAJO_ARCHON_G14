#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class PantallaCarga {
private:
    sf::Font* fuente;
    sf::Text textoTitulo;
    std::vector<sf::Text> opciones;
    int indiceSeleccionado;
    const int NUM_RANURAS = 4; // Ranura 1, 2, 3 y volver

public:
    PantallaCarga(sf::Font& font, sf::Vector2u tamanoVentana);
    void dibujar(sf::RenderWindow& window);
    void moverArriba();
    void moverAbajo();
    int getIndiceSeleccionado() const { return indiceSeleccionado; }
    void actualizarTextosRanuras(bool r1, bool r2, bool r3);
};

