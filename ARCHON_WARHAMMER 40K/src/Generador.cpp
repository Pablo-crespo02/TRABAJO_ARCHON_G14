#include "Generador.h"
#include "Color.h"
#include "Tablero.h"
#include "Motor.h"

//FUNCIÓN QUE INICIALIZA EL TABLERO:
void Generador::GenerarTablero(Tablero& tablero) {

    //Se genera una matriz "monigote" para definir qué tipo de casilla es cada posición
    TipoCasilla esquemacasillas[9][9] = {
       {TipoCasilla::FijaNegra,TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra,TipoCasilla::ColorCambiante,TipoCasilla::ColorCambiante,TipoCasilla::ColorCambiante,TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra,TipoCasilla::FijaBlanca},
       {TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra,TipoCasilla::ColorCambiante,TipoCasilla::FijaBlanca,TipoCasilla::ColorCambiante,TipoCasilla::FijaNegra, TipoCasilla::ColorCambiante,TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra},
       {TipoCasilla::FijaNegra,TipoCasilla::ColorCambiante,TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra,TipoCasilla::ColorCambiante,TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra,TipoCasilla::ColorCambiante,TipoCasilla::FijaBlanca},
       {TipoCasilla::ColorCambiante,TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra,TipoCasilla::FijaBlanca,TipoCasilla::ColorCambiante,TipoCasilla::FijaNegra,TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra,TipoCasilla::ColorCambiante},
       {TipoCasilla::FijaBlanca,TipoCasilla::ColorCambiante,TipoCasilla::ColorCambiante,TipoCasilla::ColorCambiante,TipoCasilla::ColorCambiante,TipoCasilla::ColorCambiante,TipoCasilla::ColorCambiante,TipoCasilla::ColorCambiante,TipoCasilla::FijaNegra},
       {TipoCasilla::ColorCambiante,TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra,TipoCasilla::FijaBlanca,TipoCasilla::ColorCambiante,TipoCasilla::FijaNegra,TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra,TipoCasilla::ColorCambiante},
       {TipoCasilla::FijaNegra,TipoCasilla::ColorCambiante,TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra,TipoCasilla::ColorCambiante,TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra,TipoCasilla::ColorCambiante,TipoCasilla::FijaBlanca},
       {TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra,TipoCasilla::ColorCambiante,TipoCasilla::FijaBlanca,TipoCasilla::ColorCambiante,TipoCasilla::FijaNegra, TipoCasilla::ColorCambiante,TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra},
       {TipoCasilla::FijaNegra,TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra,TipoCasilla::ColorCambiante,TipoCasilla::ColorCambiante,TipoCasilla::ColorCambiante,TipoCasilla::FijaBlanca,TipoCasilla::FijaNegra,TipoCasilla::FijaBlanca},
    };

    //Recorremos la matriz "esquemacasillas" para inicializar nuestros valores:
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {

            //Definimos una variables locales para pasarle al consructor de casilla:
            ColorActual colorinicial;
            bool esPowerPoint = false;

            //Definimos qué posiciones son Power Points:
            if ((i == 0 && j == 4) || (i == 4 && j == 0) || (i == 4 && j == 4) || (i == 4 && j == 8) || (i == 8 && j == 4)) {
                esPowerPoint = true;
            }

            //Definimos colores iniciales (cambiantes empiezan en blanco)
            if (esquemacasillas[i][j] == TipoCasilla::FijaNegra) {
                colorinicial = ColorActual::Negro_pico;
            }
            else {
                colorinicial = ColorActual::Blanco_pico;
            }

            //Llamamos al constructor de casilla para cada posición del tablero:
            tablero.SetValoresCasillasDesdeTablero(i, j, esquemacasillas[i][j], colorinicial, esPowerPoint);

        }
    }
}

//FUNCIÓN QUE PERMITE CREAR UNA UNIDAD CON SUS ATRIBUTOS:
void Generador::AnadirUnidad(Motor& motor, Bando bando, Warhammer::DatosPieza datos, sf::Vector2i pos) {

    Pieza* p = new Pieza();

    p->bando = bando;
    p->posicionTablero = pos;
    p->rangoMovimiento = datos.Rango_movimiento;

    p->tipoMov = static_cast<TipoMovimiento>(datos.tipo_movimiento);
    p->patron = static_cast<PatronMovimiento>(datos.patron_movimiento);

    p->stats.vida = datos.vida;
    p->stats.ataque = datos.ataque;
    p->stats.defensa = datos.defensa;
    p->stats.cooldown = datos.cooldown;
    p->stats.esRango = datos.esRango;
    p->stats.velAtaque = datos.velAtaque;

    p->sincronizarPosicionTablero();

    // Como ahora la lista es pública en Mundo, accedemos a ella directamente:
    motor.listaPiezas.push_back(p);

}

//FUNCIÓN QUE INICIALIZA EL DESPLIEGUE DE UNIDADES:
void Generador::GenerarDespliegueUnidades(Motor& motor) {

    // BANDO LUZ DESPLIEGUE
    AnadirUnidad(motor, Bando::LUZ, Warhammer::ASSAULT_MARINE, sf::Vector2i(0, 0));
    AnadirUnidad(motor, Bando::LUZ, Warhammer::DREADNOUGHT, sf::Vector2i(0, 1));
    AnadirUnidad(motor, Bando::LUZ, Warhammer::PRIMARIS, sf::Vector2i(0, 2));
    AnadirUnidad(motor, Bando::LUZ, Warhammer::THUNDERHAWK, sf::Vector2i(0, 3));
    AnadirUnidad(motor, Bando::LUZ, Warhammer::CAPTAIN, sf::Vector2i(0, 4)); //LIDER
    AnadirUnidad(motor, Bando::LUZ, Warhammer::LIBRARIAN, sf::Vector2i(0, 5));
    AnadirUnidad(motor, Bando::LUZ, Warhammer::PRIMARIS, sf::Vector2i(0, 6));
    AnadirUnidad(motor, Bando::LUZ, Warhammer::DREADNOUGHT, sf::Vector2i(0, 7));
    AnadirUnidad(motor, Bando::LUZ, Warhammer::ASSAULT_MARINE, sf::Vector2i(0, 8));
    AnadirUnidad(motor, Bando::LUZ, Warhammer::VINDICARE, sf::Vector2i(1, 0));
    AnadirUnidad(motor, Bando::LUZ, Warhammer::VINDICARE, sf::Vector2i(1, 8));

    for (int i = 1; i < 8; i++) {
        AnadirUnidad(motor, Bando::LUZ, Warhammer::INTERCESSOR, sf::Vector2i(1, i));
    }

    //BANDO OSCURIDAD DESPLIEGUE
    AnadirUnidad(motor, Bando::OSCURIDAD, Warhammer::HARPY, sf::Vector2i(8, 0));
    AnadirUnidad(motor, Bando::OSCURIDAD, Warhammer::CARNIFEX, sf::Vector2i(8, 1));
    AnadirUnidad(motor, Bando::OSCURIDAD, Warhammer::TOXICRENO, sf::Vector2i(8, 2));
    AnadirUnidad(motor, Bando::OSCURIDAD, Warhammer::TIRANOFEX, sf::Vector2i(8, 3));
    AnadirUnidad(motor, Bando::OSCURIDAD, Warhammer::HIVE_TYRANT, sf::Vector2i(8, 4));//LÍDER 
    AnadirUnidad(motor, Bando::OSCURIDAD, Warhammer::GENESTEALER, sf::Vector2i(8, 5));
    AnadirUnidad(motor, Bando::OSCURIDAD, Warhammer::TOXICRENO, sf::Vector2i(8, 6));
    AnadirUnidad(motor, Bando::OSCURIDAD, Warhammer::CARNIFEX, sf::Vector2i(8, 7));
    AnadirUnidad(motor, Bando::OSCURIDAD, Warhammer::HARPY, sf::Vector2i(8, 8));
    AnadirUnidad(motor, Bando::OSCURIDAD, Warhammer::TERMAGANT, sf::Vector2i(7, 0));
    AnadirUnidad(motor, Bando::OSCURIDAD, Warhammer::TERMAGANT, sf::Vector2i(7, 8));

    for (int i = 1; i < 8; i++) {
        AnadirUnidad(motor, Bando::OSCURIDAD, Warhammer::TERMAGANT, sf::Vector2i(7, i));
    }
}
