#pragma once
#include <SFML/Graphics.hpp>
#include "Pieza.h"

class InterfazHUD {
private:
    sf::RenderWindow* window;
    sf::Font* fuente;
    std::unordered_map<std::string, sf::Texture> mapaTexturas;
    int puntosLuz = 0;
    int puntosOscuridad = 0;

    float tiempoJugado = 0.0f; 

    float tiempoTurno = 0.0f;
    float limiteTurno = 30.0f;

    void dibujarDato(sf::RenderWindow& window, std::string etiqueta, std::string valor, float x, float& yActual, sf::Color colorVal);

public:
    InterfazHUD(sf::RenderWindow& win, sf::Font& font);
    void cargarTexturas();

    void setDatosHUD(int luz, int osc, float tiempo) {
        puntosLuz = luz;
        puntosOscuridad = osc;
        tiempoJugado = tiempo;
    }
    void setDatosTurno(float tiempo, float limite) {
        tiempoTurno = tiempo;
        limiteTurno = limite;
    }

    // La función principal que encapsula toda la lógica de dibujo
    void dibujar(sf::RenderWindow& window, int ronda, int ciclo, int jugadorActual, Pieza* seleccionada );
    void dibujarDesdeMapa(sf::RenderWindow& window, std::string nombre, sf::Vector2f pos, sf::Vector2f tamano);
};