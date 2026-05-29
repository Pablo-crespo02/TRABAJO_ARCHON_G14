#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>

class GestorSonido {
private:
    std::unordered_map<std::string, sf::SoundBuffer> buffers;
    std::unordered_map<std::string, sf::Sound> sonidos;

public:
    bool cargar(const std::string& nombre, const std::string& ruta);
    void reproducir(const std::string& nombre);
    void detener(const std::string& nombre);
    void setVolumen(const std::string& nombre, float volumen);
};