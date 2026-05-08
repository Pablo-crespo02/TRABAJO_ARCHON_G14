#pragma once
#include <SFML/Graphics.hpp>
#include "Pieza.h"

class InterfazHUD {
private:
    sf::Font fuente;

    // Formatos predefinidos para optimizar
    void dibujarDato(sf::RenderWindow& window, std::string etiqueta, std::string valor, float x, float& yActual, sf::Color colorVal);

public:
    InterfazHUD();
    bool cargarFuente(const std::string& ruta);

    // La función principal que encapsula toda la lógica de dibujo
    void dibujar(sf::RenderWindow& window, int ronda, int ciclo, int jugadorActual, Pieza* seleccionada);
};