#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "EstadoJuego.h"
#include "InterfazHUD.h"
#include "Pantallainicio.h"
#include "MenusNoInteractivos.h"
#include "MenuPausa.h"
#include "PantallaCarga.h"
#include "MenuNombre.h"
#include "Motor.h" 
#include <fstream> //para fichero de guardar partida
//Creamos DatosPiezaLigera para "Lazy Loading" y asi optimizar la carga
struct DatosPiezaLigera {
    int bando;
    int posX;
    int posY;
    float vida;
    std::string nombre;
};

struct DatosGuardados {
    bool ocupada = false;
    std::vector<DatosPiezaLigera> piezasLigeras;
    int ronda = 1;      
    int ciclo = 1;    
    int jugador = 1;
    int puntosLuz = 0;        // Guardar puntos Imperium
    int puntosOscuridad = 0;  // Guardar puntos Xenos
    float tiempoJugado = 0.0f; // Temporizador
};


class Coordinador {
private:
    // Infraestructura
    sf::View vistaTablero;
    sf::View vistaUI;
    sf::RenderWindow window;
    sf::Font fuente;
    sf::Clock reloj;
    int ganadorPartida = 0; //0 = Nadie; 1 = Luz; 2 = Oscuridad
    int indiceSeleccionado = 0;
    const int MAX_OPCIONES = 6;
    std::string nombreGanador = "";
    bool modoGuardar = false;
    bool partidaEnCurso = false;
    DatosGuardados ranuras[3];

    // Fichero
    void guardarDatosEnFichero();
    void cargarDatosDeFichero();

    // Audio
    sf::SoundBuffer bufferClick;
    sf::Sound sonidoClick;

    sf::Music musicaMenu;

    
    // Componentes
    Motor motor; // El motor se inicializa 
    PantallaInicio pantallaInicio;
    MenuNoInteractivo pantallainfo;
    Estado estadoActual;
    MenuPausa* menuPausa;
    MenuNombre* menuNombre;
    Estado estadoAnterior;
    PantallaCarga* pantallaCarga;

public:

    Coordinador();
    void ejecutar();
    void gestionarEventos();
    void reiniciarPartida();
    void actualizar(float dt);
    void dibujar();

    void guardarEnRanura(int indice);
    void cargarDesdeRanura(int indice);
    
};