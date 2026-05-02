#pragma once
#include <SFML/Graphics.hpp>

//No tenía sentio crear una clase o un struct para PowerPoint, que únicamente es un booleano.
//No limitamos a generar un archivo.h y .cpp que contenga las escasas funciones que tienen que ver con...
//... PowerPoint ("inline" functions)
namespace PowerPoint {

	//Función dibujar en pantalla
	void Dibujar(sf::RenderWindow& window, float x, float y);

};
