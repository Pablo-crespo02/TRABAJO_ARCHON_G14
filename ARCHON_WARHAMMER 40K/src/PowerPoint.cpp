#include "PowerPoint.h"
#include "color.h"

namespace PowerPoint {

    void Dibujar(sf::RenderWindow& window, float x, float y)
    {
        // Forma característica para los PowerPoints
        // Vamos a usar un rombo (un cuadrado girado 45 grados)
        sf::RectangleShape glifoPower(sf::Vector2f(20.0f, 20.0f)); // Tamaño del símbolo central
        glifoPower.setOrigin(10.0f, 10.0f); // Ponemos el origen en su centro para girarlo bien
        glifoPower.setRotation(45.0f); // Giramos 45 grados para que parezca un rombo
        glifoPower.setFillColor(Colores::ColorPowerPoint); // Color Oro (Gold) para que resalte
        glifoPower.setOutlineThickness(1.5f);
        glifoPower.setOutlineColor(sf::Color::Black); // Borde negro para que se vea sobre fondo blanco

        //Establece la posición del glifo utilizando las variables calculadas...
        //... mediante el tamaño de la casilla y el margen en COMPLETAR!!!!!!!
        glifoPower.setPosition(x, y);

        //Dibuja el glifo
        window.draw(glifoPower);
    }
}
