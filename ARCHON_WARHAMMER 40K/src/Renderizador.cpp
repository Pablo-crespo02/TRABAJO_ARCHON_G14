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
void Renderizador::dibujarPieza(sf::RenderWindow& window, Pieza* pieza, Estado estadoActual) {
	if (pieza == nullptr) return;

	// --- 1. Configuración Visual Común ---
	pieza->formaVisual.setOrigin(RADIOPIEZA, RADIOPIEZA);
	pieza->formaVisual.setFillColor(pieza->bando == Bando::LUZ ? Colores::ColorFichaLuz : Colores::ColorFichaOscuridad);

	// --- 2. Lógica de Posicionamiento ---
	if (estadoActual == Estado::Arena) {
		// Modo Combate: usamos la posición absoluta (píxeles)
		pieza->formaVisual.setPosition(pieza->posicionAbsoluta);
		pieza->formaVisual.setOutlineThickness(0.0f); // Sin borde en combate
	}
	else {
		// Modo Tablero: calculamos la posición según su casilla (x, y)
		float px = static_cast<float>(pieza->posicionTablero.x) * TAMANOCASILLA + OFFSETX + 30.0f;
		float py = static_cast<float>(pieza->posicionTablero.y) * TAMANOCASILLA + OFFSETY + 30.0f;
		pieza->formaVisual.setPosition(px, py);

		// Selección (solo visible en el tablero)
		if (pieza->seleccionado) {
			pieza->formaVisual.setOutlineThickness(4.0f);
			pieza->formaVisual.setOutlineColor(Colores::ColorOutlineSeleccion);
		}
		else {
			pieza->formaVisual.setOutlineThickness(0.0f);
		}
	}

	window.draw(pieza->formaVisual);
}
void Renderizador::dibujarArena(sf::RenderWindow& window, const Arena& arena) {
	window.setView(arena.vistaArena);

	// 1. Capa Base: Suelo y Rejillas
	window.draw(arena.suelo);
	for (const auto& r : arena.rejillasSuelo) {
		window.draw(r.marco);
		for (const auto& barra : r.barras) window.draw(barra);
	}

	// 2. Capa de Suelo: Sangre (Charcos y Gotas)
	for (const auto& mancha : arena.charcosSangre) window.draw(mancha);
	for (const auto& gota : arena.gotasSangre) window.draw(gota);

	// 3. Capa de Sombras de Muros 
	// Dibujamos las sombras de los muros antes que las rocas para que el suelo se vea coherente
	for (const auto& sombraMuro : arena.sombrasMuros) {
		window.draw(sombraMuro);
	}

	// 4. Rocas con efecto 3D
	for (size_t i = 0; i < arena.rocas.size(); ++i) {
		window.draw(arena.sombras[i]); // Sombra proyectada de la roca

		sf::ConvexShape cuerpo = arena.rocas[i];
		cuerpo.setFillColor(sf::Color(80, 80, 85)); // Color lateral (extrusión)

		// Dibujamos las capas de extrusión para dar altura
		for (int j = 0; j < 10; ++j) {
			cuerpo.move(0.f, -1.f);
			window.draw(cuerpo);
		}

		// Tapa superior de la roca
		sf::ConvexShape tapa = arena.rocas[i];
		tapa.move(0.f, -10.f);
		tapa.setFillColor(sf::Color(100, 100, 105));
		window.draw(tapa);
	}

	// 5. Capa Final: Muros (Límites)
	// Se dibujan al final para que nada (sangre o sombras) tape el borde iluminado
	for (const auto& muro : arena.muros) {
		window.draw(muro);
	}
}
void Renderizador::dibujarMotor(sf::RenderWindow& window, Motor& mundo) {
	// Obtenemos el estado actual (podemos acceder directo por ser friend)
	Estado estado = mundo.estadoActual;

	if (estado == Estado::Tablero) {
		// 1. Dibujar el fondo del tablero
		dibujarTablero(window, mundo.tablero);

		// 2. Dibujar todas las piezas en modo tablero
		for (auto p : mundo.listaPiezas) {
			// Pasamos 'estado' como segundo argumento
			dibujarPieza(window, p, estado);
		}
	}
	else if (estado == Estado::Arena) {
		// 1. Dibujar el escenario de combate (suelo, rocas, rejillas...)
		dibujarArena(window, mundo.arena);

		// 2. Dibujar solo los dos combatientes en modo arena
		if (mundo.piezaAtacante != nullptr) {
			dibujarPieza(window, mundo.piezaAtacante, estado);
		}
		if (mundo.piezaDefensor != nullptr) {
			dibujarPieza(window, mundo.piezaDefensor, estado);
		}
	}
}
