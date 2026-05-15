#include "PantallaCarga.h"

PantallaCarga::PantallaCarga(sf::Font& font, sf::Vector2u tamano) {
    this->fuente = &font;
    indiceSeleccionado = 0;

    textoTitulo.setFont(*fuente);
    textoTitulo.setString("SELECCIONAR RANURA DE CARGA");
    textoTitulo.setCharacterSize(50);
    textoTitulo.setFillColor(sf::Color::Yellow);
    textoTitulo.setOrigin(textoTitulo.getLocalBounds().width / 2, 0);
    textoTitulo.setPosition(tamano.x / 2, 150.f);

    std::string nombres[] = { "RANURA 1", "RANURA 2", "RANURA 3", "VOLVER AL MENU" };

    for (int i = 0; i < NUM_RANURAS; i++) {
        sf::Text t;
        t.setFont(*fuente);
        t.setString(nombres[i]);
        t.setCharacterSize(40);
        t.setFillColor(sf::Color::White);
        t.setOutlineThickness(2);
        t.setOutlineColor(sf::Color::Black);
        t.setOrigin(t.getLocalBounds().width / 2, 0);
        t.setPosition(tamano.x / 2, 350.f + (i * 80.f));
        opciones.push_back(t);
    }
}

void PantallaCarga::dibujar(sf::RenderWindow& window) {
    window.draw(textoTitulo);
    for (int i = 0; i < NUM_RANURAS; i++) {
        if (i == indiceSeleccionado) { // <--- Cambiado a indiceSeleccionado
            opciones[i].setFillColor(sf::Color::Yellow);
        }
        else {
            opciones[i].setFillColor(sf::Color::White);
        }
        window.draw(opciones[i]);
    }
}

void PantallaCarga::moverArriba() { if (indiceSeleccionado > 0) indiceSeleccionado--; }


void PantallaCarga::moverAbajo() { if (indiceSeleccionado < NUM_RANURAS - 1) indiceSeleccionado++; }

//funcion para que ponga el (vacia) si la ranura lo está
void PantallaCarga::actualizarTextosRanuras(bool r1, bool r2, bool r3) {
    bool ocupadas[3] = { r1, r2, r3 };
    for (int i = 0; i < 3; i++) {
        if (ocupadas[i]) {
            opciones[i].setString("RANURA " + std::to_string(i + 1) + " (DATOS)");
        }
        else {
            opciones[i].setString("RANURA " + std::to_string(i + 1) + " (VACIA)");
        }
    }
}