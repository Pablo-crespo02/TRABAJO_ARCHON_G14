#include "Renderizador.h"

//RENDERIZADO DE LAS CASILLAS
void Renderizador::dibujarCasilla(sf::RenderWindow& window, const Casilla& casilla) {

	//DIBUJO DEL CUADRADO:
	//Generamos un cuadrado de SFML:
	sf::RectangleShape cuadrado(sf::Vector2f(TAMANOCASILLA - MARGEN, TAMANOCASILLA - MARGEN));

	//Calculamos su posición en la ventana según su fila y su columna.
	//Multiplicamos por 55 para dejar margen entre ellas:
	cuadrado.setPosition(sf::Vector2f(casilla.columna * TAMANOCASILLA + OFFSETX, casilla.fila * TAMANOCASILLA + OFFSETY));

	//Asignamos el color gráfico de SFML según el enum ColorCasilla: blanco
	if (casilla.colorcasilla == ColorActual::Blanco_pico)cuadrado.setFillColor(Colores::ColorBlanco_pico);

	//Asignamos el color gráfico de SFML según el enum ColorCasilla: negro
	else if (casilla.colorcasilla == ColorActual::Negro_pico) cuadrado.setFillColor(Colores::ColorNegro_pico);

	//Asignamos los colores de las casillas cambiantes según su estado:
	else if (casilla.colorcasilla == ColorActual::Blanco) cuadrado.setFillColor(Colores::ColorBlanco);
	else if (casilla.colorcasilla == ColorActual::Gris_claro) cuadrado.setFillColor(Colores::ColorGris_claro);
	else if (casilla.colorcasilla == ColorActual::Gris_medio) cuadrado.setFillColor(Colores::ColorGris_medio);
	else if (casilla.colorcasilla == ColorActual::Gris_oscuro) cuadrado.setFillColor(Colores::ColorGris_oscuro);
	else if (casilla.colorcasilla == ColorActual::Negro) cuadrado.setFillColor(Colores::ColorNegro);

	//Llamamos a la función draw de SFML para que pinte el cuadrado
	window.draw(cuadrado);

	//DIBUJO DEL POWER POINT:
	if (casilla.powerpoint == true) {
		PowerPoint::Dibujar(window, ((casilla.columna * TAMANOCASILLA + OFFSETX) + ((TAMANOCASILLA - MARGEN) / 2.0f)), (casilla.fila * TAMANOCASILLA + OFFSETY) + ((TAMANOCASILLA - MARGEN) / 2.0f));

	}

}

//RENDERIZADO DEL TABLERO
void Renderizador::dibujarTablero(sf::RenderWindow& window, Tablero& tablero) {

	//Recorremos las casillas de la matriz "tablero" y llamamos a la función "Dibujar" de "Casilla" en cada posición:


	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			dibujarCasilla(window, tablero.tablero[i][j]); //Llamamos a la función "Dibujar" de "Renderizador.h"...
			//... una vez por posición de la matriz.
		}
	}
}

//RENDERIZADO DE LAS PIEZAS:
void Renderizador::dibujarPieza(sf::RenderWindow& window, Pieza* p, Estado estadoActual) {
	if (p == nullptr) return;
	p->dibujar(window, estadoActual);
}

void Renderizador::dibujarArena(sf::RenderWindow& window, Arena& arena) {
	// La arena también debería saber dibujarse a sí misma (fondo, muros, etc.)
	arena.dibujar(window);
}

void Renderizador::dibujarMotor(sf::RenderWindow& window, Motor& mundo) {
	// Obtenemos el estado actual (podemos acceder directo por ser friend)
	Estado estado = mundo.estadoActual;

	if (estado == Estado::Tablero) {
		// Dibujar el fondo del tablero
		dibujarTablero(window, mundo.tablero);

		// Dibujar todas las piezas en modo tablero
		for (auto p : mundo.listaPiezas) {
			// Pasamos 'estado' como segundo argumento
			dibujarPieza(window, p, estado);
		}
	}
	else if (estado == Estado::Arena) {
		// Dibujar el escenario de combate (suelo, rocas, rejillas...)
		dibujarArena(window, mundo.arena);

		// Dibujar solo los dos combatientes en modo arena
		if (mundo.piezaAtacante != nullptr) {
			dibujarPieza(window, mundo.piezaAtacante, estado);
		}
		if (mundo.piezaDefensor != nullptr) {
			dibujarPieza(window, mundo.piezaDefensor, estado);
		}
	}
}
	