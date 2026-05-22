#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class MenuNombre {
private:
    sf::Font* fuente;
    sf::Texture texturaFondo;
    sf::Sprite spriteFondo;

    sf::Text textoInputTitulo;
    sf::Text textoNombreEscribiendo;

    std::string nombreActual;

public:
    MenuNombre(sf::Font& font, sf::Vector2u tamanoVentana);

    void configurarGanador(int ganador);
    void procesarTexto(sf::Uint32 unicode);
    void borrarLetra();

    std::string getNombre() const { return nombreActual; }
    void limpiarNombre() { nombreActual = ""; }

    void dibujar(sf::RenderWindow& window);
};
