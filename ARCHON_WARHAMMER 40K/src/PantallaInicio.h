/*#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class PantallaInicio
{
private:
    sf::Texture texturaFondo;
    sf::Sprite spriteFondo;

public:
    PantallaInicio();

    void dibujar(sf::RenderWindow& window);
};*/
#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

class PantallaInicio
{
private:
    sf::Texture texturaFondo;
    sf::Sprite spriteFondo;
    sf::Font fuente;
    sf::Text textoTitulo;

    // Opciones del menú
    std::vector<sf::Text> opciones;
    int indiceSeleccionado;
    const int NUM_OPCIONES = 6;

public:
    PantallaInicio();

    void dibujar(sf::RenderWindow& window);
    void moverArriba();
    void moverAbajo();
    int getIndiceSeleccionado() const { return indiceSeleccionado; }
};