#pragma once
#include "Arena.h"
#include <vector>
#include <string>
#include "BarrasArena.h"
#include "EstadoJuego.h"
#include "Hitboxes.h"
#include "Animaciones.h"

class Coordinador;
// Enums básicos para todo el juego
enum class Bando { LUZ, OSCURIDAD };

// Enum para leer los patrones de movimiento fácilmente
enum class PatronMovimiento { Ortogonal, Diagonal, Ambos };
//Enum para leer los tipos de movimiento fácilmente
enum class TipoMovimiento { Terrestre, Volador, Teletransporte };

// Estructura de estadísticas que heredará cada pieza
struct Stats {

    std::string nombre;
    float vida;
    float vidaMaxima;
    bool esRango;
    float ataque;
    float defensa;
    float velAtaque;
    //ROL VA A PERMITIR SABER SI ES CAZADOR O PERSEGUIDOR PARA LOS MINIONS CON IA
    int rol;

    //Variables encargadas de la gestión de proyectiles en la arena
    sf::Clock relojHitbox;//Reloj que avanza desde que se dispara
    sf::Clock relojProyectil;//para que cambie de fotograma al atacar
    sf::Clock relojHabilidad; //para Animacion hechizo

};


class Pieza {
protected:
    // Atributos protegidos: las clases hijas (Golem, Fénix) pueden usarlos directamente
    Bando bando;
    int rangoMovimiento;
    BarrasArena barrasArena;

    sf::Vector2f ultimadireccion; //Memoria de la última dirección a la que miró la pieza, para "apuntar" los proyectiles
    bool seleccionado;
    sf::Clock relojAtaque;
    sf::Vector2i posicionTablero;
    sf::Vector2f posicionAbsoluta;
   
    bool hechizoDisponible;//Hechizo sólo una vez por combate
    //Hechizo del basilisco, va en pieza y no en ClaseUnicornio porque la inmovilización puede afectar a cualquier pieza no a sí misma:
    bool inmovilizado = false;
    double temporizadorInmovilizacion = 0.0;
    //Hechizo del unicornio, se vuelve invulnerable:
    bool invulnerable = false;
    double temporizadorInvulnerabilidad = 0.0;
    // Hechizo Gárgola, ralentiza al contrario:
    double tiempoRalentizado = 0.0;
    double multiplicadorVelocidadActual = 1.0; //100% de su velocidad en la inicialización

    // Amistades para que el Renderizador y el Motor sigan funcionando sin cambios pesados
    friend class Motor;
    friend class Generador;
    friend class InterfazHUD;

    //Atributos para texturas, sprites, animación:
    sf::Texture texturaTablero;
    sf::Texture texturaArena;
    sf::Sprite spriteTablero;
    sf::Sprite spriteArena;

    //Texto arena sin hechizos
    sf::Text textoAviso;
    sf::Font fuente;

    //VARIABLE HIT FLASH
    float temporizadorFlashDano = 0.f;

    //Smart pointer, exclusivo de C++. Garantiza que la memoria del animador se destruya de la RAM en el momento que la pieza muera en combate y se elimine.
    std::unique_ptr<AnimadorSprites> animador; 

    double piezaAlturaTablero = 60;
    double piezaAlturaArena = 40; //NO SON UNIVERSALES, HAY PIEZAS MÁS GRANDES Y MÁS PEQUEÑAS

    int anchoFrame = 0;
    int altoFrame = 0;

    int hechizosRestantes = 3; //Para limitar los hechizos por ronda
    bool usadoEnEstaArena = false; //para limitar su uso a 1 vez por arena

public:
    Stats stats;
    PatronMovimiento patronMovimiento;
    TipoMovimiento tipoMov;
    double multiplicadorArena;
    // Constructor: Solo pide lo básico para ubicar la pieza
    Pieza(Bando b, sf::Vector2i pos);

    // Destructor Virtual: CRÍTICO al usar herencia para evitar fugas de memoria
    virtual ~Pieza() = default;

    // Métodos Virtuales Puros: Obligan a las hijas a implementar su propia lógica
    virtual bool poderMover(sf::Vector2i destino, const std::vector<Pieza*>& otrasPiezas, bool esDestinoOcupado) = 0;
    // La pieza recibe la dirección deseada, el tiempo transcurrido y la referencia a la arena para validar
    virtual void procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) = 0;
    // La pieza recibe la ventana y el estado actual para saber cómo mostrarse
    virtual void dibujar(sf::RenderWindow& window, Estado estadoActual) = 0;

    virtual Pieza* clonar() const = 0;//para clonar la listapiezas en los archivos de guardado

    // Métodos Comunes: Lógica que es igual para todos (implementada en Pieza.cpp)
    void mover(sf::Vector2i destino);
    void moverEnArena(float dx, float dy);
    void sincronizarPosicionTablero();
    bool detectarConflicto(const std::vector<Pieza*>& otrasPiezas);

    //Métodos de gestión de proyectiles en la arena:
    bool puedeAtacar()const;   //Comprueba si ha pasado suficuente tiempo desde el disparo anterior
    void reiniciarRelojHitbox(); //Reinicia el reloj de disparo

    //Gestión de la inmovilización del basilisco:
    bool getInmovilizado() const { return inmovilizado; }
    void aplicarInmovilizacion(double duracion);
    void gestionarEstadosAlterados(double dt);

    //Invulnerabilidad del unicornio:
    bool getInvulnerable() const { return invulnerable; }
    void aplicarInvulnerabilidad(double duracion); 

    //Ralentización de la Gárgola:
    void aplicarRalentizacion(double factor, double duracion); //Recibe el Hechizo
    void actualizarEstadosAlterados(double dt); //Temporizador para la gárgola

    //Getters Públicos: Para que otras piezas puedan consultarse entre sí sin errores de acceso
    sf::Vector2i getPosicionTablero() const { return posicionTablero; }

    Bando getBando() const { return bando; }

    sf::FloatRect getHitbox() const {
        return sf::FloatRect(posicionAbsoluta.x - 15.f, posicionAbsoluta.y - 15.f, 30.f, 30.f);
    }

    sf::Vector2f getPosicionAbsoluta() const {
        return sf::Vector2f(posicionAbsoluta);
    }

    sf::Vector2f getultimadireccion()const {
        return sf::Vector2f(ultimadireccion);
    }

    //SETTERS PÚBLICOS:
    // Setter para la selección (quita el borde amarillo)
    void setSeleccionado(bool valor) {
        seleccionado = valor;
    }

    // Setter para la posición en la arena
    void setPosicionAbsoluta(sf::Vector2f nuevaPos) {
        posicionAbsoluta = nuevaPos;
    }

    //Setter para establecer tiempos de recarga diferentes para cada pieza:
    void setTiempoRecarga(double tiemporecarga) {
        stats.velAtaque = tiemporecarga;
    };

    //Setter para establecer la última dirección de mirada de la pieza, para apuntar los proyectiles:
    void setultimadireccion(sf::Vector2f nuevadireccion) {
        if (nuevadireccion.x != 0 || nuevadireccion.y != 0) {
            ultimadireccion = nuevadireccion;
        }
    }

    int getHechizosRestantes() const {
        return hechizosRestantes;
    }
    void resetearUsoArena() { usadoEnEstaArena = false; }

    bool puedeLanzar() const { return hechizosRestantes > 0 && !usadoEnEstaArena; }
    //Limite de 3 hechizos por ronda cada pieza
    virtual void usarHechizo(std::vector<Hitbox>& contenedordeAtaques, Pieza* enemigo) {}

    void resetearHechizos() { hechizosRestantes = 3; }

    void intentarUsarHechizo(std::vector<Hitbox>& Hitboxes, Pieza* enemigo);
       
    virtual std::string getDescripcionHechizo() const { return "Hechizo basico de unidad"; }

    virtual void actualizarMinions(float dt, Arena& arena, Pieza* enemigo,std::vector<Hitbox>& hitboxes) {}
    virtual void limpiarMinions() {}
    virtual std::vector<Pieza*>& getMinionsInvocados() {
        static std::vector<Pieza*> vacio;
        return vacio; // Las piezas normales devuelven una lista vacía
    }

    sf::Vector2f getUltimaDireccion() const { return ultimadireccion; }

    // Método público para actualizar las barras de la arena de los minions 
    void gestionarBarraAtaqueMinion(float vidaActual, float vidaMax, float velAtaque, const sf::Vector2f& posicion, bool reiniciarReloj) {
        // Al estar DENTRO de Pieza, tenemos acceso total y directo a 'barrasArena'

        // Si el motor o el líder dicen que ha atacado, reiniciamos el reloj visual
        if (reiniciarReloj) {
            this->barrasArena.reiniciarRecarga();
        }

        // Llamamos a la función nativa de tu clase BarrasArena
        this->barrasArena.actualizar(vidaActual, vidaMax, velAtaque, posicion);
    }

    //CONFIGURCIÓN CENTRALIZADA DE LOS SPRITES:

    void cargarConfigurarSprites(const std::string& tipo);

    void actualizarAnimacion(double dt);

    //´MÉTODP DE ANIMAR GENÉRICO PARA TODAS LAS PIEZAS:
    void Animar(float dt, sf::Vector2f direccion);

    //FUNCIONES HIT FLASH
    void activarFlashDano() { temporizadorFlashDano = 0.15f; } // Dura 0.15 segundos
    void actualizarFlash(float dt);
};