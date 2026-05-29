#include "GestorSonido.h"
#include <iostream>

bool GestorSonido::cargar(const std::string& nombre, const std::string& ruta) {
    if (!buffers[nombre].loadFromFile(ruta)) {
        std::cout << "Error cargando sonido: " << ruta << std::endl;
        return false;
    }
    sonidos[nombre].setBuffer(buffers[nombre]);
    return true;
}

void GestorSonido::reproducir(const std::string& nombre) {
    if (sonidos.count(nombre)) {
        sonidos[nombre].play();
    }
}

void GestorSonido::detener(const std::string& nombre) {
    if (sonidos.count(nombre)) {
        sonidos[nombre].stop();
    }
}

void GestorSonido::setVolumen(const std::string& nombre, float volumen) {
    if (sonidos.count(nombre)) {
        sonidos[nombre].setVolume(volumen);
    }
}