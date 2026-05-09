#include "PantallaInicio.h"
#include<iostream>


PantallaInicio::PantallaInicio() {
    if (!texturaFondo.loadFromFile("imagenes/pantalla_inicio.png")) {
        std::cout << "Error cargando imagen" << std::endl;
    }
    spriteFondo.setTexture(texturaFondo);

    // 1. Obtenemos la resolución del monitor directamente
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    // 2. Escalamos el sprite para que coincida con esa resolución
    sf::Vector2u tamanoImg = texturaFondo.getSize();

    float escalaX = (float)desktop.width / tamanoImg.x;
    float escalaY = (float)desktop.height / tamanoImg.y;

    spriteFondo.setScale(escalaX, escalaY);

    // inicializar menu
    fuente.loadFromFile("fuentes/fuente_pixel.ttf");
    //TITULO
    textoTitulo.setFont(fuente);
    textoTitulo.setString("MENU PRINCIPAL");
    textoTitulo.setCharacterSize(65); // Letra más grande que las opciones
    textoTitulo.setFillColor(sf::Color::Black); // Interior negro
    textoTitulo.setOutlineThickness(3.f); // Borde grueso
    textoTitulo.setOutlineColor(sf::Color::White); // Borde blanco para que destaque
    textoTitulo.setPosition(100.f, 250.f); // Lo colocamos arriba del todo

    std::string nombres[] = { "INICIAR PARTIDA", "REANUDAR PARTIDA", "INSTRUCCIONES", "CREDITOS","SALIR DEL JUEGO","CARGAR PARTIDA"};
    indiceSeleccionado = 0; // 0 es iniciar partida

    for (int i = 0; i < NUM_OPCIONES; i++) {
        sf::Text texto;
        texto.setFont(fuente);
        texto.setString(nombres[i]);
        texto.setCharacterSize(40);
        // Posicionamiento: X=100 (margen izquierdo), Y=400 (hacia abajo) con 60px de separación entre opciones
        texto.setPosition(100.f, 400.f + (i * 60.f));
        opciones.push_back(texto);
    }
}

void PantallaInicio::dibujar(sf::RenderWindow& window) {
    // IMPORTANTE: Asegurarnos de que no hay "vistas" raras activas
    window.setView(window.getDefaultView());
    window.draw(spriteFondo);

    for (int i = 0; i < NUM_OPCIONES; i++) {
        if (i == indiceSeleccionado) {
            // OPCIÓN SELECCIONADA: Amarilla con borde negro
            opciones[i].setFillColor(sf::Color::Yellow);
            opciones[i].setOutlineThickness(3.f);
            opciones[i].setOutlineColor(sf::Color::Black);
        }
        else {
            // OPCIONES NORMALES: Negras con borde blanco
            opciones[i].setFillColor(sf::Color::Black);
            opciones[i].setOutlineThickness(2.f);
            opciones[i].setOutlineColor(sf::Color::White);
        }
        window.draw(opciones[i]);
    }
}

void PantallaInicio::moverArriba() {
    if (indiceSeleccionado - 1 >= 0) {
        indiceSeleccionado--;
    }
}

void PantallaInicio::moverAbajo() {
    if (indiceSeleccionado + 1 < NUM_OPCIONES) {
        indiceSeleccionado++;
    }
}