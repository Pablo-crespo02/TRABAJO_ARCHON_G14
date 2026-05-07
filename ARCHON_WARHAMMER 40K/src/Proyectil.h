#pragma once
#include <SFML/Graphics.hpp>


class Proyectil
{

	sf::CircleShape forma;  //Le damos al proyectil una forma provisional de esfera
	sf::Vector2i velocidad;   //Velocidad del proyectil
	bool activo;    //Booleano que detecta si el proyectil está activo o no

public:

	//Constructor:
	Proyectil(sf::Vector2i posicionInicial, sf::Vector2i direccion, double rapidez, sf::Color color);
	
	//Función actualizar, encargada de mover el proyectil:
	void ActualizarProyectil();

	//GETTER PÚBLICOS:
	sf::CircleShape getFormaProyectil() const { return forma;}
};
