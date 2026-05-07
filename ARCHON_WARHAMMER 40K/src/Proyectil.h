#pragma once
#include <SFML/Graphics.hpp>


class Proyectil
{

	sf::CircleShape forma;  //Le damos al proyectil una forma provisional de esfera
	sf::Vector2f velocidad;   //Velocidad del proyectil
	bool activo;    //Booleano que detecta si el proyectil está activo o no

public:

	//Constructor:
	Proyectil(sf::Vector2f posicionInicial, sf::Vector2f direccion, double rapidez, sf::Color color);
	
	//Función actualizar, encargada de mover el proyectil:
	void ActualizarProyectil();

	//GETTER PÚBLICOS:
	sf::CircleShape getFormaProyectil() const { return forma;}

	sf::Vector2f getPosicionProyectil() const { return forma.getPosition(); }

	bool getEstadoProyectil()const { return activo; }


	//SETTER PÚBLICOS:
	void setEstadoProyectil(bool estadoproyectil) {
		activo = estadoproyectil;
	}
};
