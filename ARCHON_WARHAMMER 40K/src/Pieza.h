#pragma once
#include "Arena.h"
#include <vector>
#include <string>
#include "BarrasArena.h"
#include "EstadoJuego.h"
#include "Hitboxes.h"
#include "Animaciones.h"

class Coordinador;

// Enums:
enum class Bando { LUZ, OSCURIDAD };
enum class PatronMovimiento { Ortogonal, Diagonal, Ambos };
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
    int rol; //Define si es cazador o perseguidor para los minions IA

    //Variables encargadas de la gestión de proyectiles en la arena
    sf::Clock relojHitbox;//Reloj que avanza desde que se dispara
    sf::Clock relojProyectil;//para que cambie de fotograma al atacar
    sf::Clock relojHabilidad; //para Animacion hechizo

};

// Clase pieza:
class Pieza {
protected:
    
    // Atributos tablero:
    Bando bando;
    int rangoMovimiento;
    sf::Vector2i posicionTablero;
    sf::Vector2f posicionAbsoluta;
    bool seleccionado;
       
   // Atributos arena:
    sf::Vector2f ultimadireccion; //Memoria de la última dirección a la que miró la pieza, para "apuntar" los proyectiles
    BarrasArena barrasArena;
    std::unique_ptr<AnimadorSprites> animador; //Smart pointer, exclusivo de C++. Garantiza que la memoria del animador se destruya de la RAM en el momento que la pieza muera en combate y se elimine.
    sf::Texture texturaTablero;
    sf::Texture texturaArena;
    sf::Sprite spriteTablero;
    sf::Sprite spriteArena;

    // Atributos hechizos:
    sf::Clock relojAtaque;
    bool hechizoDisponible;//Hechizo sólo una vez por combate
    int hechizosRestantes = 3; //Para limitar los hechizos por ronda
    bool usadoEnEstaArena = false; //para limitar su uso a 1 vez por arena
   

    // Atributos estados particulares:
    bool inmovilizado = false; //Hechizo del basilisco
    bool invulnerable = false; //Hechizo del unicornio
    double temporizadorInmovilizacion = 0.0;
    double temporizadorInvulnerabilidad = 0.0;
    double tiempoRalentizado = 0.0; //Hechizo de la gárgola
    double multiplicadorVelocidadActual = 1.0;

    // Atributos de texto:
    sf::Text textoAviso;
    sf::Font fuente;

    // Atributos renderizado de los sprites:
    double piezaAlturaTablero = 60;
    double piezaAlturaArena = 40;
    int anchoFrame = 0;
    int altoFrame = 0;

    // Atributos Flash Daño:
    float temporizadorFlashDano = 0.f;
    
   // Amistades para que el Motor siga funcionando sin cambios pesados
    friend class Motor;
    friend class Generador;
    friend class InterfazHUD;
   
public:
    Stats stats;
    PatronMovimiento patronMovimiento;
    TipoMovimiento tipoMov;
    double multiplicadorArena;

    // Constructor y destructor:
    Pieza(Bando b, sf::Vector2i pos);
    virtual ~Pieza() = default;

    // Métodos Virtuales Puros: Obligan a las hijas a implementar su propia lógica
    virtual bool poderMover(sf::Vector2i destino, const std::vector<Pieza*>& otrasPiezas, bool esDestinoOcupado) = 0;
    virtual void procesarMovimientoArena(sf::Vector2f direccion, float dt, Arena& arena) = 0;
    virtual void dibujar(sf::RenderWindow& window, Estado estadoActual) = 0;
    virtual Pieza* clonar() const = 0;//para clonar la lista de piezas en los archivos de guardado

    // Métodos Comunes a todos los elementos:
    void mover(sf::Vector2i destino);
    void moverEnArena(float dx, float dy);
    void sincronizarPosicionTablero();
    bool detectarConflicto(const std::vector<Pieza*>& otrasPiezas);

    // Métodos de gestión de proyectiles en la arena:
    bool puedeAtacar()const;   //Comprueba si ha pasado suficuente tiempo desde el disparo anterior
    void reiniciarRelojHitbox(); //Reinicia el reloj de disparo

    // Setters y getters de los estados particulares:
    bool getInmovilizado() const { return inmovilizado;}
    bool getInvulnerable() const { return invulnerable; }
    void aplicarInmovilizacion(double duracion);
    void aplicarInvulnerabilidad(double duracion);
    void aplicarRalentizacion(double factor, double duracion);
    void gestionarEstadosAlterados(double dt); //UNIFICAR ESTE Y EL SIGUIENTE!!!!!!!!!
    void actualizarEstadosAlterados(double dt);
    
    // Getters generales:
    Bando getBando() const { return bando; }
    sf::Vector2i getPosicionTablero() const { return posicionTablero; }
    sf::Vector2f getPosicionAbsoluta() const {return sf::Vector2f(posicionAbsoluta);}
    sf::Vector2f getultimadireccion()const { return sf::Vector2f(ultimadireccion); }
    sf::FloatRect getHitbox() const { return sf::FloatRect(posicionAbsoluta.x - 15.f, posicionAbsoluta.y - 15.f, 30.f, 30.f); }
    int getHechizosRestantes() const { return hechizosRestantes;}
    bool puedeLanzar() const { return hechizosRestantes > 0 && !usadoEnEstaArena; }//Limite de 3 hechizos por ronda cada pieza

    // Setters generales:
    void setSeleccionado(bool valor) {seleccionado = valor;}
    void setPosicionAbsoluta(sf::Vector2f nuevaPos) { posicionAbsoluta = nuevaPos;}
    void setTiempoRecarga(double tiemporecarga) { stats.velAtaque = tiemporecarga; }
    void setultimadireccion(sf::Vector2f nuevadireccion) { if (nuevadireccion.x != 0 || nuevadireccion.y != 0) ultimadireccion = nuevadireccion; }
   
    // Hechizos y cooldowns:
    void resetearUsoArena() { usadoEnEstaArena = false; }
    void resetearHechizos() { hechizosRestantes = 3; }
    void intentarUsarHechizo(std::vector<Hitbox>& Hitboxes, Pieza* enemigo);
    virtual void usarHechizo(std::vector<Hitbox>& contenedordeAtaques, Pieza* enemigo) {}
    virtual std::string getDescripcionHechizo() const { return "Hechizo basico de unidad"; }

    // Minions:
    virtual void actualizarMinions(float dt, Arena& arena, Pieza* enemigo,std::vector<Hitbox>& hitboxes) {}
    virtual void limpiarMinions() {}
    virtual std::vector<Pieza*>& getMinionsInvocados() {
        static std::vector<Pieza*> vacio;
        return vacio; // Las piezas normales devuelven una lista vacía
    }
    void gestionarBarraAtaqueMinion(float vidaActual, float vidaMax, float velAtaque, const sf::Vector2f& posicion, bool reiniciarReloj) {
        // Al estar DENTRO de Pieza, tenemos acceso total y directo a 'barrasArena'

        // Si el motor o el líder dicen que ha atacado, reiniciamos el reloj visual
        if (reiniciarReloj) {
            this->barrasArena.reiniciarRecarga();
        }
        // Llamamos a la función nativa de tu clase BarrasArena
        this->barrasArena.actualizar(vidaActual, vidaMax, velAtaque, posicion);
    }

    // Renderizado, hit-Flash, Animaciones:
    void cargarConfigurarSprites(const std::string& tipo);
    void actualizarAnimacion(double dt);
    virtual void Animar(float dt, sf::Vector2f direccion);
    void activarFlashDano() { temporizadorFlashDano = 0.15f; }; // Dura 0.15 segundos
    void actualizarFlash(float dt);
    void dibujarAnilloSeleccion(sf::RenderWindow& window);
};