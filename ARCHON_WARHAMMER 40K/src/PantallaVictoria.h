#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class PantallaVictoria
{
private:
    sf::Texture texturaFondoVictoriaLuz;
    sf::Texture texturaFondoVictoriaOscuridad;
    sf::Sprite spriteFondo;
    sf::Font fuente;
    sf::Text textoVictoria;
    sf::Text textoContinuar;

public:
    PantallaVictoria() {};
    void configurarPantallaVictoria(int ganador, sf::RenderWindow& window);
    void dibujar(sf::RenderWindow& window);
};
