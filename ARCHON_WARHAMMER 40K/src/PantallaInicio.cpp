#include "PantallaInicio.h"
#include <iostream>

PantallaInicio::PantallaInicio() {
    // 1. Carga de recursos
    if (!texturaFondo.loadFromFile("imagenes/pantalla_inicio.png")) {
        std::cout << "Error cargando imagen de fondo" << std::endl;
    }
    spriteFondo.setTexture(texturaFondo);

    if (!fuente.loadFromFile("fuentes/fuente_pixel.ttf")) {
        std::cout << "Error cargando fuente" << std::endl;
    }

    // 2. Ajuste de escala a la resolución del monitor
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::Vector2u tamanoImg = texturaFondo.getSize();

    float escalaX = (float)desktop.width / tamanoImg.x;
    float escalaY = (float)desktop.height / tamanoImg.y;
    spriteFondo.setScale(escalaX, escalaY);

    // 3. Configuración del Título
    textoTitulo.setFont(fuente);
    textoTitulo.setString("MENU PRINCIPAL");
    textoTitulo.setCharacterSize(65);
    textoTitulo.setFillColor(sf::Color::Black);
    textoTitulo.setOutlineThickness(3.f);
    textoTitulo.setOutlineColor(sf::Color::White);
    textoTitulo.setPosition(100.f, 250.f);

    // 4. Configuración de las Opciones
    std::string nombres[] = {
        "INICIAR PARTIDA",
        "REANUDAR PARTIDA",
        "INSTRUCCIONES",
        "CREDITOS",
        "SALIR DEL JUEGO",
        "CARGAR PARTIDA"
    };

    indiceSeleccionado = 0; // Empezamos en Iniciar Partida

    for (int i = 0; i < NUM_OPCIONES; i++) {
        sf::Text texto;
        texto.setFont(fuente);
        texto.setString(nombres[i]);
        texto.setCharacterSize(40);
        // Posicionamiento: X=100, Y=400 con 60px de separación
        texto.setPosition(100.f, 400.f + (i * 60.f));
        opciones.push_back(texto);
    }
}

void PantallaInicio::dibujar(sf::RenderWindow& window) {
    // Aseguramos la vista por defecto para que el menú no se mueva con la cámara del juego
    window.setView(window.getDefaultView());

    window.draw(spriteFondo);
    window.draw(textoTitulo);

    for (int i = 0; i < NUM_OPCIONES; i++) {
        if (i == indiceSeleccionado) {
            // OPCIÓN SELECCIONADA: Amarilla, borde negro y un poco más grande
            opciones[i].setFillColor(sf::Color::Yellow);
            opciones[i].setOutlineThickness(3.f);
            opciones[i].setOutlineColor(sf::Color::Black);
            opciones[i].setScale(1.1f, 1.1f); // Efecto visual de resaltado
        }
        else {
            // OPCIONES NORMALES: Negras con borde blanco
            opciones[i].setFillColor(sf::Color::Black);
            opciones[i].setOutlineThickness(2.f);
            opciones[i].setOutlineColor(sf::Color::White);
            opciones[i].setScale(1.0f, 1.0f);
        }
        window.draw(opciones[i]);
    }
}

void PantallaInicio::moverArriba() {
    // Navegación circular: si estamos arriba y pulsamos arriba, vamos al final
    indiceSeleccionado = (indiceSeleccionado == 0) ? NUM_OPCIONES - 1 : indiceSeleccionado - 1;
}

void PantallaInicio::moverAbajo() {
    // Navegación circular: si estamos abajo y pulsamos abajo, vamos al inicio
    indiceSeleccionado = (indiceSeleccionado + 1) % NUM_OPCIONES;
}