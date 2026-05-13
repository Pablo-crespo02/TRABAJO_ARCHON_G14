#include "MenuPausa.h"
#include <iostream>

MenuPausa::MenuPausa(sf::Font& font, sf::Vector2u tamano) {
    this->fuente = &font;
    indiceSeleccionado = 0;

    if (!texturaFondo.loadFromFile("imagenes/pantalla_pausa.png")) {
        std::cout << "Error cargando imagen de pausa" << std::endl;
    }
    spriteFondo.setTexture(texturaFondo);

    float escalaX = (float)tamano.x / texturaFondo.getSize().x;
    float escalaY = (float)tamano.y / texturaFondo.getSize().y;
    spriteFondo.setScale(escalaX, escalaY);

    // Fondo de oscurecimiento 
    fondoOscuro.setSize(sf::Vector2f(tamano.x, tamano.y));
    fondoOscuro.setFillColor(sf::Color(0, 0, 0, 180));
    // Título
    textoTitulo.setFont(*fuente);
    textoTitulo.setString("PAUSA");
    textoTitulo.setCharacterSize(80);
    textoTitulo.setFillColor(sf::Color::Yellow);
    textoTitulo.setOrigin(textoTitulo.getLocalBounds().width / 2, 0);
    textoTitulo.setPosition(tamano.x / 2, tamano.y * 0.15f);
    // Opciones del menú
    std::string nombres[] = {
        "REANUDAR", "REINICIAR PARTIDA", "MENU PRINCIPAL",
        "INSTRUCCIONES", "GUARDAR PARTIDA", "SALIR AL ESCRITORIO"
    };

    for (int i = 0; i < NUM_OPCIONES; i++) {
        sf::Text t;
        t.setFont(*fuente);
        t.setString(nombres[i]);
        t.setCharacterSize(45);
        t.setFillColor(sf::Color::White);
        t.setOrigin(t.getLocalBounds().width / 2, 0);
        t.setPosition(tamano.x / 2, tamano.y * 0.4f + (i * 70.f));
        opciones.push_back(t);
    }
}

void MenuPausa::dibujar(sf::RenderWindow& window) {
    window.setView(window.getDefaultView()); // HUD plano
   // Dibujar la imagen de fondo primero
    window.draw(spriteFondo);
    // Dibujamos el rectángulo oscuro encima para que actúe de sombreado
    window.draw(fondoOscuro);

    window.draw(textoTitulo);

    for (int i = 0; i < NUM_OPCIONES; i++) {
        if (i == indiceSeleccionado) {
            opciones[i].setFillColor(sf::Color::Yellow);
            opciones[i].setScale(1.2f, 1.2f);
        }
        else {
            opciones[i].setFillColor(sf::Color::White);
            opciones[i].setScale(1.0f, 1.0f);
        }
        window.draw(opciones[i]);
    }
}

void MenuPausa::moverArriba() {
    indiceSeleccionado = (indiceSeleccionado == 0) ? NUM_OPCIONES - 1 : indiceSeleccionado - 1;
}

void MenuPausa::moverAbajo() {
    indiceSeleccionado = (indiceSeleccionado + 1) % NUM_OPCIONES;
}