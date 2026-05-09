#pragma once
#include "Tablero.h"
#include "Pieza.h"
#include "Arena.h"
#include "EstadoJuego.h"
#include <vector>
#include "Pantallainicio.h"
#include "Hitboxes.h"
#include "InterfazHUD.h"
class Motor {
private:

    //ATRIBUTOS MOTOR:
    Tablero tablero;
    Arena arena;
    PantallaInicio pantallaInicio;
    InterfazHUD hud;
    int jugadorActual = 1;       // 1 o 2
    int cicloActual = 1;         // De 1 a 12
    int rondaActual = 1;         // Incrementa tras ciclo 12
    int ganadorPartida = 0; //0 = NADIE; 1 = LUZ; 2 = OSCURIDAD
    std::string cicloNombre = "Luz";
    Estado estadoActual;
    sf::RenderWindow window;
    sf::View vistaTablero;
    sf::View vistaUI;
    sf::Clock reloj;
    sf::Font fuenteGlobal;
    std::vector<Hitbox> Hitboxes;    //Contenedor para los hitboxes activos
    //IMPORTANTE: hacer CLEAR al vector cuando termine una batalla en la arena
    double limitecolision = 36;

public:
    bool windowAbierta() { return window.isOpen(); }

    std::vector<Pieza*> listaPiezas;
    Pieza* piezaSeleccionada = nullptr; // Para saber qué pieza estamos moviendo
    Pieza* piezaAtacante = nullptr;
    Pieza* piezaDefensor = nullptr; // Lista de todas las piezas en juego...
    //... pública porque se modificará desde otros .cpp ('Generador'), también se puede declarar amiga

    Motor(); //Declaración constructor
    ~Motor(); // Para borrar las piezas al cerrar

    static void generarMapa(Arena& arena, sf::Color cLuz = sf::Color::White, sf::Color cOsc = sf::Color(50, 50, 50));
    void manejarClick(sf::Vector2i mousePos);
    void manejarEventos();
    void actualizar();
    void renderizar();
    void dibujarHUD();
    void dibujarInfoPieza(sf::RenderWindow& window, Pieza* pieza, float x, float y);

    // Se llama cuando un jugador termina su acción
    void intentarAccionJugador(int idJugador);

    // Muestra el estado por consola
    void imprimirEstado();

    void iniciarCombate(Pieza* atacante, Pieza* defensor);

    //Verifica las condiciones de victoria:
    void VerificarVictoria();

    //Función de vuelta al menú principal, reiniciando todos los parámetros
    void reiniciarJuego();
};

