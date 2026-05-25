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
    const int NUM_RANURAS = 9; // Ranuras 1,2,3,4,5,6,7,8 y volver

public:
    PantallaCarga(sf::Font& font, sf::Vector2u tamanoVentana);
    void dibujar(sf::RenderWindow& window);
    void moverArriba();
    void moverAbajo();
    int getIndiceSeleccionado() const { return indiceSeleccionado; }
    void actualizarTextosRanuras(const std::vector<bool>& ocupadas);
};

