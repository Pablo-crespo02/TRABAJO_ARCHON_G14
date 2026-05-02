#pragma once
#include <SFML/Graphics.hpp>
#include "Tablero.h"
#include "EstadoJuego.h"
#include "Color.h"
#include "Motor.h"
#include "Arena.h"

class Renderizador
{
	//Ctes. estáticas que rigen el renderizado. 
	//static constexpr = variale estática que queda declarado antes de iniciarse el juego...
	//...de tal forma que evita coste de computación CPU mientras está activo el main.
	static constexpr float TAMANOCASILLA = 60.0F;
	static constexpr float MARGEN = 2.0f;
	static constexpr float OFFSETX = 50.0f;
	static constexpr float OFFSETY = 30.0f;
	static constexpr float RADIOPIEZA = 20.0f;

public:
	static void dibujarCasilla(sf::RenderWindow& window, const Casilla& casilla);
	static void dibujarTablero(sf::RenderWindow& window, Tablero& tablero);
	static void dibujarMotor(sf::RenderWindow& window, Motor& motor);
	static void dibujarPieza(sf::RenderWindow& window, Pieza* p, Estado estadoact);
	static void dibujarArena(sf::RenderWindow& window, const Arena&);
};
