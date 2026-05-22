#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <fstream>

struct RegistroPartida {
    std::string nombre;
    std::string bando;
    int puntosLuz;
    int puntosOscuridad;
    float tiempo;
};

class MenuNoInteractivo
{
private:

    //PANTALLA DE VICTORIA:
    sf::Texture texturaFondo;
    sf::Sprite spriteFondo;
    sf::Font fuente;
    sf::Text textoVictoria;
    sf::Text textoJugadorGanador;
    sf::Text textoContinuar;
    sf::Text textoTiempo;
    sf::Text textoPuntuaciones;

    //OTRAS PANTALLAS ESTÁTICAS:
    sf::Text textoCreditos;
    sf::Text textoInstrucciones;
    sf::Text textoRankingTitulo;
    sf::Text textoRankingLineas;

public:
    MenuNoInteractivo() {};
    void inicializarTextos();
   
    //Pantallas Estáticas
    void configurarPantallaVictoria(int ganador, int ptosLuz, int ptosOscuridad, float tiempoJugado, std::string nombreGanador, sf::RenderWindow& window);
    void dibujarPantallaVictoria(sf::RenderWindow& window);
    void dibujarPantallaCreditos(sf::RenderWindow& window);
    void dibujarPantallaInstrucciones(sf::RenderWindow& window);
    void dibujarPantallaRanking(sf::RenderWindow& window);
};