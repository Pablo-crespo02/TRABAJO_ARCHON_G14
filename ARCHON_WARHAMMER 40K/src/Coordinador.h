#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "EstadoJuego.h"
#include "InterfazHUD.h"
#include "Pantallainicio.h"
#include "PantallaVictoria.h"
#include "MenuPausa.h"
// EL MOTOR DEBE IR AL FINAL de los includes de componentes
#include "Motor.h" 

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

    // --- COMPONENTES ---
    Motor motor; // El motor se inicializa 
    PantallaInicio pantallaInicio;
    PantallaVictoria pantallavictoria;
    Estado estadoActual;
    MenuPausa* menuPausa;
    Estado estadoAnterior;

public:

    Coordinador();
    void ejecutar();
    void gestionarEventos();
    void reiniciarPartida();
    void dibujarPantallaVictoria();
    void actualizar(float dt);
    void dibujar();
    
};