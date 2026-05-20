#pragma once
#include <SFML/Graphics.hpp>
#include "Pieza.h"

class InterfazHUD {
private:
    sf::RenderWindow* window;
    sf::Font* fuente;

    int puntosLuz = 0;
    int puntosOscuridad = 0;

    void dibujarDato(sf::RenderWindow& window, std::string etiqueta, std::string valor, float x, float& yActual, sf::Color colorVal);

public:
    InterfazHUD(sf::RenderWindow& win, sf::Font& font);
    bool cargarFuente(const std::string& ruta);

    void setPuntuaciones(int luz, int osc) {
        puntosLuz = luz;
        puntosOscuridad = osc;
    }

    // La función principal que encapsula toda la lógica de dibujo
    void dibujar(sf::RenderWindow& window, int ronda, int ciclo, int jugadorActual, Pieza* seleccionada );
};