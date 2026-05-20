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
    sf::Text textoTiempo;

    sf::Text textoCreditos;
    sf::Text textoInstrucciones;
    sf::Text textoPuntuaciones;
public:
    MenuNoInteractivo() {};
    void inicializarTextos();
    void configurarPantallaVictoria(int ganador, int ptosLuz, int ptosOscuridad, float tiempoJugado, sf::RenderWindow& window);
    void dibujarPantallaVictoria(sf::RenderWindow& window);
    void dibujarPantallaCreditos(sf::RenderWindow& window);
    void dibujarPantallaInstrucciones(sf::RenderWindow& window);
  
};

