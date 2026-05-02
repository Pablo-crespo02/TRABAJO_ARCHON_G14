#pragma once
#include <string>

namespace Warhammer {
    struct DatosPieza {
        std::string nombre;
        std::string rol_archon;
        int tipo_movimiento;    // 0: TERRESTRE, 1: VOLADOR
        int patron_movimiento;  // 0: ORTOGONAL, 1: DIAGONAL, 2: AMBOS
        int Rango_movimiento;
        float vida;
        float cooldown;
        bool esRango;
        float ataque;
        float defensa;
        float velAtaque;
    };

    const int MOV_TERRESTRE = 0;
    const int MOV_VOLADOR = 1;
    const int PATRON_ORTOGONAL = 0;
    const int PATRON_DIAGONAL = 1;
    const int PATRON_AMBOS = 2;

    // --- FACCIÓN: IMPERIUM (Luz) ---
    const DatosPieza CAPTAIN = { "Captain Terminator", "Wizard", MOV_VOLADOR, PATRON_AMBOS, 4, 9.0f, 5.0f, true, 9.0f, 10.0f, 1.2f };
    const DatosPieza LIBRARIAN = { "Librarian", "Phoenix", MOV_VOLADOR, PATRON_DIAGONAL, 3, 7.0f, 8.0f, false, 7.0f, 8.0f, 1.1f };
    const DatosPieza ASSAULT_MARINE = { "Assault Marine", "Valkyrie", MOV_VOLADOR, PATRON_AMBOS, 3, 6.0f, 6.0f, true, 6.0f, 5.0f, 1.4f };
    const DatosPieza DREADNOUGHT = { "Dreadnought", "Golem", MOV_TERRESTRE, PATRON_ORTOGONAL, 2, 10.0f, 7.0f, false, 4.0f, 9.0f, 1.6f };
    const DatosPieza INTERCESSOR = { "Intercessor", "Knight", MOV_TERRESTRE, PATRON_AMBOS, 2, 4.0f, 8.0f, false, 4.0f, 5.0f, 1.4f };
    const DatosPieza VINDICARE = { "Vindicare", "Archer", MOV_TERRESTRE, PATRON_ORTOGONAL, 2, 4.0f, 6.0f, true, 3.0f, 6.0f, 1.2f };
    const DatosPieza THUNDERHAWK = { "Thunderhawk", "Djinn", MOV_VOLADOR, PATRON_AMBOS, 3, 2.0f, 7.0f, true, 5.0f, 8.0f, 3.0f };
    const DatosPieza PRIMARIS = { "Primaris", "Unicornio", MOV_TERRESTRE, PATRON_AMBOS, 4, 2.0f, 7.0f, true, 5.0f, 8.0f, 3.0f };

    // --- FACCIÓN: TYRANIDS (Oscuridad) ---
    const DatosPieza HIVE_TYRANT = { "Hive Tyrant", "Sorceress", MOV_VOLADOR, PATRON_AMBOS, 4, 9.0f, 5.0f, true, 9.0f, 10.0f, 1.2f };
    const DatosPieza HARPY = { "Harpy", "Manticora", MOV_VOLADOR, PATRON_DIAGONAL, 3, 7.0f, 8.0f, false, 7.0f, 8.0f, 1.1f };
    const DatosPieza LICTOR = { "Lictor", "Banshee", MOV_VOLADOR, PATRON_AMBOS, 3, 6.0f, 6.0f, true, 6.0f, 5.0f, 1.4f };
    const DatosPieza CARNIFEX = { "Carnifex", "Troll", MOV_TERRESTRE, PATRON_ORTOGONAL, 2, 10.0f, 7.0f, false, 4.0f, 9.0f, 1.6f };
    const DatosPieza TERMAGANT = { "Termagant", "Goblin", MOV_TERRESTRE, PATRON_AMBOS, 2, 4.0f, 8.0f, false, 4.0f, 5.0f, 1.4f };
    const DatosPieza GENESTEALER = { "Genestealer", "Shapeshifter", MOV_VOLADOR, PATRON_AMBOS, 5, 4.0f, 8.0f, true, 4.0f, 5.0f, 1.4f };
    const DatosPieza TOXICRENO = { "Toxicreno", "Basilisco", MOV_VOLADOR, PATRON_ORTOGONAL, 3, 4.0f, 8.0f, true, 4.0f, 5.0f, 1.4f };
    const DatosPieza TIRANOFEX = { "Tiranofex", "Dragon", MOV_VOLADOR, PATRON_ORTOGONAL, 3, 4.0f, 8.0f, true, 4.0f, 5.0f, 1.4f };
}