#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>      // Te faltaba esta para std::string
#include "EstadoJuego.h" 
#include "Tablero.h"
#include "Pieza.h"
#include "Arena.h"
#include "Hitboxes.h"
#include "InterfazHUD.h"
#include "PantallaInicio.h"

class Coordinador; // Predeclaración correcta

class Motor {
    
private:
    // --- LÓGICA DE JUEGO ---
    Tablero tablero;
    Arena arena;
    InterfazHUD hud;
    Estado estadoActual; // Copia local del estado
    int jugadorActual = 1;
    int cicloActual = 1;
    int rondaActual = 1;
    int ganadorPartida = 0;
    std::string cicloNombre = "Luz";

    std::vector<Hitbox> Hitboxes;
    double limitecolision = 36;

    // Referencias externas (necesarias para dibujar y leer recursos)
    sf::RenderWindow& window;
    sf::Font& fuenteGlobal;
public:
    std::vector<Pieza*> listaPiezas;
    Pieza* piezaSeleccionada = nullptr;
    Pieza* piezaAtacante = nullptr;
    Pieza* piezaDefensor = nullptr;
    PantallaInicio pantallaInicio; //En la parte pública

    // El constructor ahora recibe la ventana y la fuente del Coordinador
    Motor(sf::RenderWindow& win, sf::Font& fuente);
    ~Motor();

    // Funciones que se quedan porque son LÓGICA
    void manejarClick(sf::Vector2i mousePos, const sf::View& vistaTablero);
    void actualizar(double dt);
    void renderizar();
    void setEstado(Estado nuevoEstado) { estadoActual = nuevoEstado; }

    void iniciarCombate(Pieza* atacante, Pieza* defensor);
    void intentarAccionJugador(int idJugador);
    void limpiarDatos();
    void VerificarVictoria();
    void dibujarHUD();
    void reiniciarJuego();
    void manejarEventos(const sf::View& vistaTablero);

    // Gestión de entrada específica del juego
    void gestionarEntrada(sf::Event& evento, const sf::View& vistaTablero);
    Estado getEstado() { return estadoActual; }
    int getGanador() const { return ganadorPartida; }
    friend class InterfazHUD; // Para que el HUD siga leyendo datos
};