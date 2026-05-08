#include "Renderizador.h"

//RENDERIZADO DE LAS CASILLAS
void Renderizador::dibujarCasilla(sf::RenderWindow& window, const Casilla& casilla) {

	// Configuración geométrica del cuadrado
   // Eliminamos los márgenes externos, dejamos que la Vista posicione el tablero
	sf::RectangleShape cuadrado(sf::Vector2f(TAMANOCASILLA - MARGEN, TAMANOCASILLA - MARGEN));

	// Posición relativa al origen (0,0) del tablero
	// La Vista del Motor se encargará de mover este (0,0) a la izquierda de la pantalla
	float posX = casilla.columna * TAMANOCASILLA;
	float posY = casilla.fila * TAMANOCASILLA;

	cuadrado.setPosition(sf::Vector2f(posX, posY));

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
	if (casilla.powerpoint) {
		// Calculamos el centro exacto de la casilla para el PowerPoint
		float centroX = posX + (TAMANOCASILLA - MARGEN) / 2.0f;
		float centroY = posY + (TAMANOCASILLA - MARGEN) / 2.0f;

		PowerPoint::Dibujar(window, centroX, centroY);
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
		// Dibujar hitboxes
		for (const auto& h : mundo.Hitboxes) { window.draw(h.getFormaHitbox()); }
	}
}
