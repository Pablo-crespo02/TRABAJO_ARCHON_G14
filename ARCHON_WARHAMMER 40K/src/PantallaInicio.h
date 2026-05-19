#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class PantallaInicio {
private:
    sf::Texture texturaFondo;
    sf::Sprite spriteFondo;
    sf::Font fuente;
    sf::Text textoTitulo;

    std::vector<sf::Text> opciones;
    std::vector<std::string> nombres = {
        "INICIAR PARTIDA", "REANUDAR PARTIDA", "INSTRUCCIONES",
        "CREDITOS","SALIR DEL JUEGO","CARGAR PARTIDA",
    };

    int indiceSeleccionado;
    const int NUM_OPCIONES = 6;
    bool partidaActiva = false;
public:
    PantallaInicio();
    void dibujar(sf::RenderWindow& window);
    void moverArriba();
    void moverAbajo();
    int getIndiceSeleccionado() const { return indiceSeleccionado; }
    void setPartidaActiva(bool activa) { partidaActiva = activa; }
};