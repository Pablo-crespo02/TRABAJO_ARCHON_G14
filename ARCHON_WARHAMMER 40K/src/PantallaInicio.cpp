#include "PantallaInicio.h"

PantallaInicio::PantallaInicio() {
    //cargar imagen de pantalla de inicio. La imagen esta en la carpeta de imagenes
    if (!texturaFondo.loadFromFile("imagenes/pantalla_inicio.png")) {
        std::cout << "Error: No se pudo cargar pantalla_inicio.png. Comprueba la ruta." << std::endl;
    }


    spriteFondo.setTexture(texturaFondo);

    //generamos una ventana acorde al tamaño de la imagen
    sf::Vector2u tamanoImagen = texturaFondo.getSize();
    if (tamanoImagen.x > 0 && tamanoImagen.y > 0) {
        float escalaX = 800.f / tamanoImagen.x;
        float escalaY = 600.f / tamanoImagen.y;
        spriteFondo.setScale(escalaX, escalaY);
    }
}

void PantallaInicio::dibujar(sf::RenderWindow& window) {
    window.draw(spriteFondo);
}
