#pragma once
#include "Tablero.h"
#include "Pieza.h"
#include <vector>

class Mundo {
private:
    Tablero tablero;
    std::vector<Pieza*> listaPiezas; // Lista de todas las piezas en juego
    int jugadorActual = 1;       // 1 o 2
    int cicloActual = 1;         // De 1 a 12
    int rondaActual = 1;         // Incrementa tras ciclo 12
    Pieza* piezaSeleccionada = nullptr; // Para saber qué pieza estamos moviendo
public:
    Mundo(); //Declaración constructor
    ~Mundo(); // Para borrar las piezas al cerrar

    // Nueva función para manejar el click
    void manejarClick(sf::Vector2i mousePos);
    // Se llama cuando un jugador termina su acción
    void intentarAccionJugador(int idJugador);

    // Muestra el estado por consola
    void imprimirEstado();

    // Dibuja el mundo (el tablero y lo que contenga)
    void dibujar(sf::RenderWindow& window);
};

