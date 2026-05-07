#include "Proyectil.h"
#include "Color.h"

//Constructor
Proyectil::Proyectil(sf::Vector2f posicionInicial, sf::Vector2f direccion, double rapidez, sf::Color color) {
	forma.setRadius(16); //Radio provisional del proyectil=4
	forma.setOrigin(16, 16);  //Centramos el origen de la forma
	forma.setPosition(posicionInicial.x, posicionInicial.y);  //Inicializamos su posición según la entrada
	forma.setFillColor(Colores::ColorProyectil); //Inicializamos su color al definido en colores.h

	//Cálculo de la velocidad:
	velocidad.x = direccion.x * rapidez;
	velocidad.y = direccion.y * rapidez;

	//marcado como activo (El constructor se llama desde el motor al pulsar tecla, si se ha llamado esque está activo)
	activo = true;
}

//Función actualzar, encargada de mover el proyectil:
void Proyectil::ActualizarProyectil() {
	forma.move(velocidad);   //Mueve el proyectil según su vector velocidad
}

