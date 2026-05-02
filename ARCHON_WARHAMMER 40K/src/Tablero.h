#pragma once
#include "Casilla.h"

class Tablero {

private:

    Casilla tablero[9][9];

    friend class Renderizador;

public:
    Tablero();

    //Función "puente" para que la clase constructora 'Generador' pueda...
    //... acceder a la función 'SetValoresCasilla' desde 'tablero.cpp':
    void SetValoresCasillasDesdeTablero(int fila, int columna, TipoCasilla tipo, ColorActual color, bool esPowerPoint);

    void actualizarColores(int numCiclo);
};
