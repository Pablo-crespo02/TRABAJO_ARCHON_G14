#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>     
#include "EstadoJuego.h" 
#include "Tablero.h"
#include "Pieza.h"
#include "Arena.h"
#include "Hitboxes.h"
#include "InterfazHUD.h"
#include "PantallaInicio.h"

class Coordinador; // Predeclaración

class Motor {
private:
    // --- LÓGICA DE JUEGO ---
    Tablero tablero;
    Arena arena;
    InterfazHUD hud;
    Estado estadoActual;
    int jugadorActual = 1;
    int cicloActual = 1;
    int rondaActual = 1;
    int ganadorPartida = 0;
    std::string cicloNombre = "Luz";

    std::vector<Hitbox> Hitboxes;
    double limitecolision = 36;

    // Referencias externas 
    sf::RenderWindow& window;
    sf::Font& fuenteGlobal;

    // Sonidos
    sf::SoundBuffer bufferError;
    sf::SoundBuffer bufferMover;
    sf::Sound sonidoMover;
    sf::Sound sonidoError;

    // Sistema de Historial Local
    sf::Clock relojPartida;
    int puntuacionImperium = 0;
    int puntuacionTyranidos = 0;
    void guardarPartidaEnHistorial(std::string bandoGanador);

    void procesarInput(Pieza* p, sf::Keyboard::Key arriba, sf::Keyboard::Key abajo,
        sf::Keyboard::Key izqda, sf::Keyboard::Key dcha,
        sf::Keyboard::Key ataque, sf::Vector2f dirPorDefecto, float dt);

public:
    std::vector<Pieza*> listaPiezas;
    Pieza* piezaSeleccionada = nullptr;
    Pieza* piezaAtacante = nullptr;
    Pieza* piezaDefensor = nullptr;
    PantallaInicio pantallaInicio;

    Motor(sf::RenderWindow& win, sf::Font& fuente);
    ~Motor();

    Estado getEstado() const { return estadoActual; }
    int getGanador() const { return ganadorPartida; }
    std::vector<Pieza*>& getListaPiezas() { return listaPiezas; }
    void setListaPiezas(const std::vector<Pieza*>& nuevasPiezas) { listaPiezas = nuevasPiezas; }

    int getRondaActual() const { return rondaActual; }
    int getCicloActual() const { return cicloActual; }
    int getJugadorActual() const { return jugadorActual; }

    void setRondaActual(int r) { rondaActual = r; }
    void setJugadorActual(int j) { jugadorActual = j; }

    void setCicloActual(int c) {
        cicloActual = c;
        tablero.actualizarColores(c);
    }

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
    void gestionarEntrada(const sf::Event& evento, const sf::View& vistaTablero);
};