#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map> // Diccionario, permite buscar elementos asociando una clave (como por ejemplo su nombre)
#include <string>

struct ClipAnimacion {  //Almacena los parámetros de un clip de animación
	int fila;  //Fila en la hoja de sprites
	int columnaInicial; //Columna en la que empieza la aniumación
	int columnaFinal;  //Columna en la que acaba la ai¡nimación
	double velocidadFrame;  //Tiempo en segundos que pasa en cada fotograma, determina la velocidad de la animación
	bool bucle;  //Determina si la animación es un "loop", si debe reiniciarse al llegar al último frame
};

class AnimadorSprites
{
private:
	sf::Sprite& spriteObjetivo; //Sprite de la piza que vamos a animar
	int anchoFrame;  //ancho en Píxeles de un frame
	int altoFrame;   //alto en píxeles de un frame

	std::unordered_map<std::string, ClipAnimacion>animaciones; //Diccionario donde se guardan estructuras "ClipAnimacion" y se buscan utilizando un "string" (su nombre)
	std::string animacionActual = ""; //Guarda el nombre de la animación que se está reproduciendo
	int frameActual = 0;  //índice de la columna del rame que se está mostrando actualmente
	double temporizador = 0;
	bool animacionTerminada = false;

public:
	AnimadorSprites(sf::Sprite& sprite, int ancho, int alto)      //Constructor vacío
		:spriteObjetivo(sprite), anchoFrame(ancho), altoFrame(alto) {}

	void agreganAnimacion(const std::string& nombre, int fila, int colInicio, int colFin, double velocidad, bool bucle = true) {
		//Si la clave de búsqueda e el unordered map no existe, la crea, y le asigna la estructura de ClipAnimacion:
		animaciones[nombre] = { fila,colInicio,colFin,velocidad,bucle};
	}

	void jugar(const std::string& nombre) {  //Cambia el estado viaual de la pieza
		if (animacionActual == nombre)return; //Si ya se está reproduciendo la animación, se evita que se reinicie

		if (animaciones.find(nombre) != animaciones.end()) {  //Si la animación existe en el diccionario unordered map...
			animacionActual = nombre; //Cambia el nombre de la animación activa
			frameActual = animaciones[nombre].columnaInicial;  //Establece el frame actual como el primero de la nueva animación
			temporizador = 0;
			animacionTerminada = false;
			actualizarRectanguloSprite();
		}
	}

	//Utilza el delta time que se usa en la arena para actualizar los fotogramas:
	void actualizar(double dt) {
		if (animacionActual.empty() || animacionTerminada) return; //Si no hay animación actual o ha terminado salimos de la función

		const auto& clip = animaciones[animacionActual];  //Crea un alias únicamente de lectura del clip actual
		temporizador += dt;

		if (temporizador >= clip.velocidadFrame) { //Si el temporizador es mayor que el tiempo establecido de duración del frame...
			temporizador = 0;
			frameActual++;

			if (frameActual > clip.columnaFinal) { //Si se supera la columna final de la animación...
				if (clip.bucle) {
					frameActual = clip.columnaInicial; //Si el clip es unn bucle se reinicia
				}
				else {
					frameActual = clip.columnaFinal;  
					animacionTerminada = true;      //Si el clip no es un bucle se acaba la animación
				}
			}
			actualizarRectanguloSprite();
		}

	}


	//GETTERS PÚBLICOS:
	bool getTerminada()const { return animacionTerminada; }
	std::string getAnimacionnActual()const { return animacionActual; }

private:

	//Aplica un recorte rectangular sobre la imagen. para seleccionar los frames:
	void actualizarRectanguloSprite() {
		const auto& clip = animaciones[animacionActual];  //Obtenemos los datos del clip actual
		sf::IntRect rect(frameActual * anchoFrame, clip.fila * altoFrame, anchoFrame, altoFrame); //Define un uevo ectángulo de recorte en función del ancho y alto de los frames
		spriteObjetivo.setTextureRect(rect); //Muestra únicamente el nuevo rectángulo de coordenadas calculadas.
	}
};

