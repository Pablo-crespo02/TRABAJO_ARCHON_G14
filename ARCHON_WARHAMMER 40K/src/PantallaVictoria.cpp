#include "PantallaVictoria.h"
#include <iostream>

void PantallaVictoria::configurarPantallaVictoria(int ganador, sf::RenderWindow& window) {

    //CARGA DE RECURSOS Y MENSAJE EN CASO DE ERROR, FUENTE:
    if (!fuente.loadFromFile("fuentes/fuente_pixel.ttf")) {
        std::cout << "Error cargando fuente" << std::endl;
        return;
    }

    //VICTORIA IMPERIUM O XENOS:
    switch (ganador) {
    case 1:

        //Carga de recursos y mensaje de error
        if (!texturaFondoVictoriaLuz.loadFromFile("imagenes/Pantalla_Victoria_Imperium.png")) {
            std::cout << "Error cargando la pantalla de Victoria LUZ" << std::endl;
            return;
        }

        spriteFondo.setTexture(texturaFondoVictoriaLuz,true);  //true garantiza el correcto scaling de la imagen
        textoVictoria.setString("VICTORIA DEL IMPERIUM");
        textoVictoria.setFillColor(sf::Color::Yellow);
        break;

    case 2:

        if (!texturaFondoVictoriaOscuridad.loadFromFile("imagenes/Pantalla_Victoria_Tiranidos.png")) {
            std::cout << "Error cargando la pantalla de Victoria OSCURIDAD" << std::endl;
            return;
        }

        spriteFondo.setTexture(texturaFondoVictoriaOscuridad,true);
        textoVictoria.setString("VICTORIA XENOS");
        textoVictoria.setFillColor(sf::Color::Red);
        break;

    default:
        std::cout << "Error de entrada a la función constrcutora de PantallaVictoria, entero distinto de 1-2";
        return;
    }

    //AJUSTE DE ESCALA DEL MONITOR:
    sf::Vector2u tamanoVentana = window.getSize();
    sf::Vector2u tamanoImg = spriteFondo.getTexture()->getSize();

    float escalaX = (float)tamanoVentana.x/ tamanoImg.x;
    float escalaY = (float)tamanoVentana.y / tamanoImg.y;
    spriteFondo.setScale(escalaX, escalaY);


    //CONFIGURACIÓN DEL TEXTO:

    //Texto principal:
    textoVictoria.setFont(fuente);
    textoVictoria.setCharacterSize(80);
    textoVictoria.setOutlineColor(sf::Color::Black);
    textoVictoria.setOutlineThickness(4.0f);
    sf::FloatRect textRect = textoVictoria.getLocalBounds();
    textoVictoria.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    textoVictoria.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 50.f);

    //Texto de volver al menú:
    textoContinuar.setFont(fuente);
    textoContinuar.setString("Pulsa ENTER para volver al menu");
    textoContinuar.setCharacterSize(30);
    textoContinuar.setOutlineColor(sf::Color::Black);
    textoContinuar.setOutlineThickness(2.0f);
    textoContinuar.setFillColor(sf::Color::White);
    sf::FloatRect contRect = textoContinuar.getLocalBounds();
    textoContinuar.setOrigin(contRect.left + contRect.width / 2.0f, contRect.top + contRect.height / 2.0f);
    textoContinuar.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 50.0f);
}

void PantallaVictoria::dibujar(sf::RenderWindow& window) {

    // Aseguramos la vista por defecto para que el menú no se mueva con la cámara del juego
    window.setView(window.getDefaultView());

    window.draw(spriteFondo);
    window.draw(textoVictoria);
    window.draw(textoContinuar);
}
