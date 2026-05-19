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
// EL MOTOR DEBE IR AL FINAL de los includes de componentes
#include "Motor.h" 

struct DatosGuardados {
    bool ocupada = false;
    std::vector<Pieza*> piezas;
};


class Coordinador {
private:
    // --- INFRAESTRUCTURA ---
    sf::View vistaTablero;
    sf::View vistaUI;
    sf::RenderWindow window;
    sf::Font fuenteGlobal;
    sf::Clock reloj;
    int ganadorPartida = 0; //0 = NADIE; 1 = LUZ; 2 = OSCURIDAD
    int indiceSeleccionado = 0;
    const int MAX_OPCIONES = 6;
    bool modoGuardar = false;
    bool partidaEnCurso = false;
    DatosGuardados ranuras[3];

    // --- AUDIO ---
    sf::SoundBuffer bufferClick;
    sf::Sound sonidoClick;


    // --- COMPONENTES ---
    Motor motor; // El motor se inicializa 
    PantallaInicio pantallaInicio;
    PantallaVictoria pantallavictoria;
    Estado estadoActual;
    MenuPausa* menuPausa;
    Estado estadoAnterior;
    PantallaCarga* pantallaCarga;

public:

    Coordinador();
    void ejecutar();
    void gestionarEventos();
    void reiniciarPartida();
    void dibujarPantallaVictoria();
    void actualizar(float dt);
    void dibujar();

    void guardarEnRanura(int indice);
    void cargarDesdeRanura(int indice);
    
};