#pragma once
#include "Casilla.h"

class Tablero {

private:

    Casilla tablero[9][9];

public:
    Tablero();

    //Función "puente" para que la clase constructora 'Generador' pueda...
    //... acceder a la función 'SetValoresCasilla' desde 'tablero.cpp':
    void SetValoresCasillasDesdeTablero(int fila, int columna, TipoCasilla tipo, ColorActual color, bool esPowerPoint);
    void dibujar(sf::RenderWindow& window);//Orden para dibujarse
    void actualizarColores(int numCiclo);

    bool getpowerpoint(sf::Vector2i posicion)const { return tablero[posicion.x][posicion.y].getpowerpointcasilla(); };
    ColorActual getcoloractualcasilla(sf::Vector2i posicion) { return tablero[posicion.y][posicion.x].getcoloractualcasilla(); }
};