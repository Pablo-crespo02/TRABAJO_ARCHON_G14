#include <string>
#include "PantallaCarga.h"

PantallaCarga::PantallaCarga(sf::Font& font, sf::Vector2u tamano) {
    this->fuente = &font;
    indiceSeleccionado = 0;

    textoTitulo.setFont(*fuente);
    textoTitulo.setString("SELECCIONAR RANURA DE CARGA");
    textoTitulo.setCharacterSize(50);
    textoTitulo.setFillColor(sf::Color::Yellow);
    textoTitulo.setOrigin(textoTitulo.getLocalBounds().width / 2, 0);
    textoTitulo.setPosition(tamano.x / 2, 100.f);
    
    std::string nombres[9];
    for (int i = 0; i < 8; i++) {
        nombres[i] = "RANURA " + std::to_string(i + 1);
    }
    nombres[8] = "VOLVER AL MENU";

    for (int i = 0; i < NUM_RANURAS; i++) {
        sf::Text t;
        t.setFont(*fuente);
        t.setString(nombres[i]);
        t.setCharacterSize(32);
        t.setFillColor(sf::Color::White);
        t.setOutlineThickness(2);
        t.setOutlineColor(sf::Color::Black);
        t.setOrigin(t.getLocalBounds().width / 2, 0);
        t.setPosition(tamano.x / 2, 220.f + (i * 55.f));
        opciones.push_back(t);
    }
}

void PantallaCarga::dibujar(sf::RenderWindow& window) {
    window.draw(textoTitulo);
    for (int i = 0; i < NUM_RANURAS; i++) {
        if (i == indiceSeleccionado) { 
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
void PantallaCarga::actualizarTextosRanuras(const std::vector<bool>& ocupadas) {
    for (int i = 0; i < 8; i++) {
        if (ocupadas[i]) {
            opciones[i].setString("RANURA " + std::to_string(i + 1) + " (OCUPADA)");
        }
        else {
            opciones[i].setString("RANURA " + std::to_string(i + 1) + " (VACIA)");
        }
        opciones[i].setOrigin(opciones[i].getLocalBounds().width / 2, 0);
    }
}
