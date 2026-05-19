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
    //sonido
    sf::SoundBuffer bufferMover;
    sf::Sound sonidoMover;
    void procesarInput(Pieza* p, sf::Keyboard::Key arriba, sf::Keyboard::Key abajo,
        sf::Keyboard::Key izqda, sf::Keyboard::Key dcha,
        sf::Keyboard::Key ataque, sf::Vector2f dirPorDefecto, float dt);
public:
    std::vector<Pieza*> listaPiezas;
    std::vector<Pieza*> getListaPiezas() const { return listaPiezas; }
    void setListaPiezas(const std::vector<Pieza*>& nuevasPiezas) { listaPiezas = nuevasPiezas; }

    //FUNCIONES EXTRA PARA GUARDADO Y CARGA
    //para que el Coordinador pueda leer las variables al guardar
    int getRondaActual() const { return rondaActual; }
    int getCicloActual() const { return cicloActual; }
    int getJugadorActual() const { return jugadorActual; }

    // para que el Coordinador pueda escribir las variables al cargar
    void setRondaActual(int r) { rondaActual = r; }
    void setJugadorActual(int j) { jugadorActual = j; }

    void setCicloActual(int c) {
        cicloActual = c;
        tablero.actualizarColores(c);
    }
    Pieza* piezaSeleccionada = nullptr;
    Pieza* piezaAtacante = nullptr;
    Pieza* piezaDefensor = nullptr;
    PantallaInicio pantallaInicio; 

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