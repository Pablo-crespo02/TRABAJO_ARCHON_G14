#pragma once
#include "Tablero.h"
#include "Pieza.h"
#include "Arena.h"
#include "EstadoJuego.h"
#include <vector>
#include "Renderizador.h"
#include "Pantallainicio.h"
#include "Proyectil.h"
#include "AtaqueMelee.h"

class Renderizador;
class Motor {
private:

    //TAMAÑO DE LA PANTALLA:
    double anchopantalla = 800;
    double altopantalla = 600;

    //ATRIBUTOS MOTOR:
    Tablero tablero;
    Arena arena;
    PantallaInicio pantallaInicio;
    int jugadorActual;       // 1 o 2
    int cicloActual;         // De 1 a 12
    int rondaActual;         // Incrementa tras ciclo 12
    Estado estadoActual;
    sf::RenderWindow window;
    sf::Clock reloj;
    friend class Renderizador;
    std::vector<Proyectil> proyectiles;    //Contenedor para los proyectiles activos
    std::vector<AtaqueMelee> ataquesMelee; //Igual pero para melee
    //IMPORTANTE: hacer CLEAR al vector cuando termine una batalla en la arena

public:
    bool windowAbierta() { return window.isOpen(); }

    std::vector<Pieza*> listaPiezas;
    Pieza* piezaSeleccionada = nullptr; // Para saber qué pieza estamos moviendo
    Pieza* piezaAtacante = nullptr;
    Pieza* piezaDefensor = nullptr; // Lista de todas las piezas en juego...
    //... pública porque se modificará desde otros .cpp ('Generador'), tambiémn se puede declarar amiga

    Motor(); //Declaración constructor
    ~Motor(); // Para borrar las piezas al cerrar

    static void generarMapa(Arena& arena, sf::Color cLuz = sf::Color::White, sf::Color cOsc = sf::Color(50, 50, 50));
    void manejarClick(sf::Vector2i mousePos);
    void manejarEventos();
    void actualizar();
    void renderizar();

    // Se llama cuando un jugador termina su acción
    void intentarAccionJugador(int idJugador);

    // Muestra el estado por consola
    void imprimirEstado();

    void iniciarCombate(Pieza* atacante, Pieza* defensor);
};

