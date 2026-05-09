#include "PantallaInicio.h"

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
}

void PantallaInicio::dibujar(sf::RenderWindow& window) {
    // IMPORTANTE: Asegurarnos de que no hay "vistas" raras activas
    window.setView(window.getDefaultView());
    window.draw(spriteFondo);
}