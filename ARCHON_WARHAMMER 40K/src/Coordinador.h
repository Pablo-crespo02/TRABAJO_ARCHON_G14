#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "EstadoJuego.h"
#include "InterfazHUD.h"
#include "Pantallainicio.h"
#include "PantallaVictoria.h"
#include "MenuPausa.h"
#include "PantallaCarga.h"
#include "Motor.h" 

struct DatosGuardados {
    bool ocupada = false;
    std::vector<Pieza*> piezas;
    int ronda = 1;
    int ciclo = 1;
    int jugador = 1;
};

class Coordinador {
private:
    // --- INFRAESTRUCTURA ---
    sf::View vistaTablero;
    sf::View vistaUI;
    sf::RenderWindow window;
    sf::Font fuenteGlobal;
    sf::Clock reloj;
    int ganadorPartida = 0;
    int indiceSeleccionado = 0;
    const int MAX_OPCIONES = 6;
    bool modoGuardar = false;
    bool partidaEnCurso = false;
    DatosGuardados ranuras[3];

    // --- AUDIO ---
    sf::SoundBuffer bufferClick;
    sf::Sound sonidoClick;

    // --- COMPONENTES ---
    Motor motor;
    PantallaInicio pantallaInicio;
    PantallaVictoria pantallavictoria;
    MenuPausa* menuPausa;
    PantallaCarga* pantallaCarga;
    Estado estadoActual;
    Estado estadoAnterior;

public:
    Coordinador();
    void ejecutar();
    void gestionarEventos();
    void actualizar(float dt);
    void dibujar();

    void reiniciarPartida();
    void guardarEnRanura(int indice);
    void cargarDesdeRanura(int indice);
};