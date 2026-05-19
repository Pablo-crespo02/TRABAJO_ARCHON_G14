#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class MenuNoInteractivo
{
private:

    //PANTALLA DE VICTORIA:

    sf::Texture texturaFondo;
    sf::Sprite spriteFondo;
    sf::Font fuente;
    sf::Text textoVictoria;
    sf::Text textoContinuar;

    sf::Text textoCreditos;
    sf::Text textoInstrucciones;
        
public:
    MenuNoInteractivo() {};
    void inicializarTextos();
    void configurarPantallaVictoria(int ganador, sf::RenderWindow& window);
    void dibujarPantallaVictoria(sf::RenderWindow& window);
    void dibujarPantallaCreditos(sf::RenderWindow& window);
    void dibujarPantallaInstrucciones(sf::RenderWindow& window);

};
