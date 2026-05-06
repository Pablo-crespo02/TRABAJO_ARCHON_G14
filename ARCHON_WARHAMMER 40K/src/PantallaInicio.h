#pragma once
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
};
