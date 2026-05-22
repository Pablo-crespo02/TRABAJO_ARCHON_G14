#include "MenuNombre.h"
#include <iostream>

MenuNombre::MenuNombre(sf::Font& font, sf::Vector2u tamano) {
    this->fuente = &font;
    this->nombreActual = "";

    // Reutilizamos el fondo de las pantallas estáticas (o el que prefieras)
    if (!texturaFondo.loadFromFile("imagenes/Pantalla_nombre.png")) {
        std::cout << "Error cargando imagen en MenuNombre" << std::endl;
    }
    spriteFondo.setTexture(texturaFondo);
    spriteFondo.setScale((float)tamano.x / texturaFondo.getSize().x, (float)tamano.y / texturaFondo.getSize().y);

    // Formato del título
    textoInputTitulo.setFont(*fuente);
    textoInputTitulo.setCharacterSize(45);
    textoInputTitulo.setFillColor(sf::Color::Yellow);
    textoInputTitulo.setOutlineColor(sf::Color::Black);
    textoInputTitulo.setOutlineThickness(3.0f);

    // Formato del texto que se escribe
    textoNombreEscribiendo.setFont(*fuente);
    textoNombreEscribiendo.setCharacterSize(60);
    textoNombreEscribiendo.setFillColor(sf::Color::White);
    textoNombreEscribiendo.setOutlineColor(sf::Color::Black);
    textoNombreEscribiendo.setOutlineThickness(3.0f);
}

void MenuNombre::configurarGanador(int ganador) {
    limpiarNombre(); // Vaciamos el nombre por si venimos de otra partida

    if (ganador == 1) {
        textoInputTitulo.setString("FELICIDADES PRIMARCA INMORTALICE SU VICTORIA:");
    }
    else {
        textoInputTitulo.setString("FELICIDADES LIDER DE LA COLMENA INMORTALICE SU VICTORIA:");
    }
}

void MenuNombre::procesarTexto(sf::Uint32 unicode) {
    // Si pulsa un carácter válido (letras, números, espacios) y no supera 12 caracteres
    if (unicode < 128 && unicode > 31 && nombreActual.size() < 12) {
        nombreActual += std::toupper(static_cast<char>(unicode));
    }
}

void MenuNombre::borrarLetra() {
    if (!nombreActual.empty()) {
        nombreActual.pop_back();
    }
}

void MenuNombre::dibujar(sf::RenderWindow& window) {
    window.setView(window.getDefaultView());
    window.draw(spriteFondo);

    // Centramos el título dinámicamente
    sf::FloatRect rectTitulo = textoInputTitulo.getLocalBounds();
    textoInputTitulo.setOrigin(rectTitulo.left + rectTitulo.width / 2.0f, rectTitulo.top + rectTitulo.height / 2.0f);
    textoInputTitulo.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 100.f);
    window.draw(textoInputTitulo);

    // Le añadimos el guion bajo parpadeante al dibujar
    textoNombreEscribiendo.setString(nombreActual + "_");
    sf::FloatRect rectNombre = textoNombreEscribiendo.getLocalBounds();
    textoNombreEscribiendo.setOrigin(rectNombre.left + rectNombre.width / 2.0f, rectNombre.top + rectNombre.height / 2.0f);
    textoNombreEscribiendo.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 20.f);
    window.draw(textoNombreEscribiendo);
}