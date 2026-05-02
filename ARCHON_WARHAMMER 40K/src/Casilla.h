#pragma once
#include "PowerPoint.h"
#include <SFML/Graphics.hpp>

enum class TipoCasilla { FijaBlanca, FijaNegra, ColorCambiante };
enum class ColorActual { Blanco_pico, Blanco, Gris_claro, Gris_medio, Gris_oscuro, Negro, Negro_pico };

class Casilla
{
    int fila;
    int columna;
    bool powerpoint;

public:

    //Atributos que queremos que se puedan modificar libremente:
    TipoCasilla tipocasilla;
    ColorActual colorcasilla;

    //Función que asigna valores a las casillas
    void SetValoresCasillas(int f, int c, TipoCasilla t, ColorActual C, bool Pp);

    //Función para dibujar por pantalla:
    void Dibujar(sf::RenderWindow& window);    //Función dibujar, recibe la ventana "window" para...
    friend class Renderizador;
};
