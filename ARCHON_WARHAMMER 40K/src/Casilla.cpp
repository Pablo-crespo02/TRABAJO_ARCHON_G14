#include "Casilla.h"
#include "Color.h"
#include "PowerPoint.h"
#include<SFML/graphics.hpp>

//Constructor: inicializa las variables a los valores que le entran al llamar la función
void Casilla::SetValoresCasillas(int f, int c, TipoCasilla t, ColorActual C, bool Pp) {
    fila = f;
    columna = c;
    tipocasilla = t;
    colorcasilla = C;
    powerpoint = Pp;
}

//Dibuja por pantalla:
void Casilla::Dibujar(sf::RenderWindow& window) {
    const float tamCasilla = 60.0f;
    const float margen = 2.0f;

    // DIBUJO DEL CUADRADO:
    sf::RectangleShape cuadrado(sf::Vector2f(tamCasilla - margen, tamCasilla - margen));

    // CORRECCIÓN: Quitamos los +50.f y +30.f. 
    // Usamos coordenadas puras para alinear con Pieza::sincronizarPosicionTablero()
    float posX = columna * tamCasilla;
    float posY = fila * tamCasilla;
    cuadrado.setPosition(sf::Vector2f(posX, posY));

    // Asignamos los colores (sin cambios)
    if (colorcasilla == ColorActual::Blanco_pico) cuadrado.setFillColor(Colores::ColorBlanco_pico);
    else if (colorcasilla == ColorActual::Negro_pico) cuadrado.setFillColor(Colores::ColorNegro_pico);
    else if (colorcasilla == ColorActual::Blanco) cuadrado.setFillColor(Colores::ColorBlanco);
    else if (colorcasilla == ColorActual::Gris_claro) cuadrado.setFillColor(Colores::ColorGris_claro);
    else if (colorcasilla == ColorActual::Gris_medio) cuadrado.setFillColor(Colores::ColorGris_medio);
    else if (colorcasilla == ColorActual::Gris_oscuro) cuadrado.setFillColor(Colores::ColorGris_oscuro);
    else if (colorcasilla == ColorActual::Negro) cuadrado.setFillColor(Colores::ColorNegro);

    // Pintamos
    window.draw(cuadrado);

    // DIBUJO DEL POWER POINT:
    if (powerpoint) {
        // CORRECCIÓN: Calculamos el centro exacto basado en posX y posY limpios
        float centroX = posX + ((tamCasilla - margen) / 2.0f);
        float centroY = posY + ((tamCasilla - margen) / 2.0f);
        PowerPoint::Dibujar(window, centroX, centroY);
    }
}