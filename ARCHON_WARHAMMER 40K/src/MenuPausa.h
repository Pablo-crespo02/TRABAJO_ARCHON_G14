#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class MenuPausa {
private:
    sf::Font* fuente;
    sf::RectangleShape fondoOscuro;

    sf::Texture texturaFondo;
    sf::Sprite spriteFondo;

    sf::Text textoTitulo;
    std::vector<sf::Text> opciones;
    int indiceSeleccionado;
    const int NUM_OPCIONES = 6;

public:
    MenuPausa(sf::Font& font, sf::Vector2u tamanoVentana);
    void dibujar(sf::RenderWindow& window);
    void moverArriba();
    void moverAbajo();
    int getIndiceSeleccionado() const { return indiceSeleccionado; }
};