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



    //DIBUJO DEL CUADRADO:
    //Generamos un cuadrado de SFML:
    sf::RectangleShape cuadrado(sf::Vector2f(tamCasilla - margen, tamCasilla - margen));

    //Calculamos su posición en la ventana según su fila y su columna.
    //Multiplicamos por 55 para dejar margen entre ellas:
    cuadrado.setPosition(sf::Vector2f(columna * tamCasilla + 50.f, fila * tamCasilla + 30.f));

    //Asignamos el color gráfico de SFML según el enum ColorCasilla: blanco
    if (colorcasilla == ColorActual::Blanco_pico)cuadrado.setFillColor(Colores::ColorBlanco_pico);

    //Asignamos el color gráfico de SFML según el enum ColorCasilla: negro
    else if (colorcasilla == ColorActual::Negro_pico) cuadrado.setFillColor(Colores::ColorNegro_pico);

    //Asignamos los colores de las casillas cambiantes según su estado:
    else if (colorcasilla == ColorActual::Blanco) cuadrado.setFillColor(Colores::ColorBlanco);
    else if (colorcasilla == ColorActual::Gris_claro) cuadrado.setFillColor(Colores::ColorGris_claro);
    else if (colorcasilla == ColorActual::Gris_medio) cuadrado.setFillColor(Colores::ColorGris_medio);
    else if (colorcasilla == ColorActual::Gris_oscuro) cuadrado.setFillColor(Colores::ColorGris_oscuro);
    else if (colorcasilla == ColorActual::Negro) cuadrado.setFillColor(Colores::ColorNegro);

    //Llamamos a la función draw de SFML para que pinte el cuadrado
    window.draw(cuadrado);

    //DIBUJO DEL POWER POINT:
    if (powerpoint == true) {
        PowerPoint::Dibujar(window, ((columna * tamCasilla + 50.f) + ((tamCasilla - margen) / 2.0f)), (fila * tamCasilla + 30.f) + ((tamCasilla - margen) / 2.0f));

    }

}
