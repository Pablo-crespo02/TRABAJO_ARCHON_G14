#pragma once
#include "Tablero.h"
#include "Pieza.h"
#include "Arena.h"
#include "unidades_warhammer.h"
#include "EstadoJuego.h"
#include <vector>
#include "Renderizador.h"

class Renderizador;
class Motor {
private:
    Tablero tablero;
    Arena arena;
    int jugadorActual = 1;       // 1 o 2
    int cicloActual = 1;         // De 1 a 12
    int rondaActual = 1;         // Incrementa tras ciclo 12
    Pieza* piezaSeleccionada = nullptr; // Para saber qué pieza estamos moviendo
    Pieza* piezaAtacante = nullptr;
    Pieza* piezaDefensor = nullptr;
    Estado estadoActual;
    sf::RenderWindow window;
    sf::Clock reloj;
    friend class Renderizador;

public:
    bool windowAbierta() { return window.isOpen(); }

    std::vector<Pieza*> listaPiezas; // Lista de todas las piezas en juego...
    //... pública porque se modificará desde otros .cpp ('Generador')

    Motor(); //Declaración constructor
    ~Motor(); // Para borrar las piezas al cerrar

    //Función para manejar el click
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

