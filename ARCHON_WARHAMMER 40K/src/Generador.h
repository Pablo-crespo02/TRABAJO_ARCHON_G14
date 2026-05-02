#pragma once
#include "Tablero.h"
#include "Pieza.h"
#include "unidades_warhammer.h"
#include "Motor.h"
#include "Arena.h"

//CLASE ENCARGADA EXCLUSIVAMENTE DE LA CONSTRUCCIÓN DE ELEMENTOS DEL JUEGO

class Generador
{

public:

	//Función que inicializa el tablero:
	static void GenerarTablero(Tablero& tablero);

	//Función que permite generar una unidad con sus atributos:
	static void AnadirUnidad(Motor& motor, Bando bando, Warhammer::DatosPieza datos, sf::Vector2i pos);

	//Función que inicializa las piezas en el tablero:
	static void GenerarDespliegueUnidades(Motor& motor);
	//Función que inicializa la 
	static void InicializarArena(Arena& arena, sf::Color c1, sf::Color c2);

};
