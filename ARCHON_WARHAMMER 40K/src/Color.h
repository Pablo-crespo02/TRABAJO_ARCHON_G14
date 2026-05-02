#pragma once
#include <SFML/Graphics.hpp>

//Struct con los colores del las casillas
struct Colores
{
	static const sf::Color ColorBlanco_pico;
	static const sf::Color ColorBlanco;
	static const sf::Color ColorGris_claro;
	static const sf::Color ColorGris_medio;
	static const sf::Color ColorGris_oscuro;
	static const sf::Color ColorNegro;
	static const sf::Color ColorNegro_pico;
	static const sf::Color ColorPowerPoint;
	static const sf::Color ColorFichaLuz;
	static const sf::Color ColorFichaOscuridad;
	static const sf::Color ColorOutlineSeleccion;
};


inline const sf::Color Colores::ColorBlanco_pico = sf::Color(255, 255, 255);
inline const sf::Color Colores::ColorBlanco = sf::Color(220, 220, 220);
inline const sf::Color Colores::ColorGris_claro = sf::Color(180, 180, 180);
inline const sf::Color Colores::ColorGris_medio = sf::Color(120, 120, 120);
inline const sf::Color Colores::ColorGris_oscuro = sf::Color(60, 60, 60);
inline const sf::Color Colores::ColorNegro = sf::Color(30, 30, 30);
inline const sf::Color Colores::ColorNegro_pico = sf::Color(0, 0, 0);
inline const sf::Color Colores::ColorPowerPoint = sf::Color(255, 215, 0);
inline const sf::Color Colores::ColorFichaLuz = sf::Color(0, 183, 235);
inline const sf::Color Colores::ColorFichaOscuridad = sf::Color(255, 0, 0);
inline const sf::Color Colores::ColorOutlineSeleccion = sf::Color(255, 255, 0);
