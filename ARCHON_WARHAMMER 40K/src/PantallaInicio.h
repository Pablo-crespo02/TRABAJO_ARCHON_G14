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
        "CREDITOS", "CARGAR PARTIDA", "SALIR DEL JUEGO"
    };

    int indiceSeleccionado;
    const int NUM_OPCIONES = 6;
public:
    PantallaInicio();
    void inicializar(float ancho, float alto); // Para configurar posiciones según resolución
    void dibujar(sf::RenderWindow& window);
    void moverArriba();
    void moverAbajo();
    int getIndiceSeleccionado() const { return indiceSeleccionado; }
};