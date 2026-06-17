#ifndef PRZEDMIOT_H
#define PRZEDMIOT_H

#include "elementy_gry.h"

// ============================================================
// DZIEDZICZENIE: Przedmiot -> ElementyGry -> IAktualizowanie + IRysowanie
// ============================================================
// Przedmiot to trzecia konkretna klasa pochodna od ElementyGry.
// Reprezentuje kebab (punkty) do zebrania przez gracza.
//
// Implementuje obowiązkowe metody wirtualne:
//   - aktualizuj() override  ← pusta, przedmiot jest statyczny
//   - getTypGrafiki() override ← "kebsik" lub "kebsik_super"
//
// Zwróć uwagę: Przedmiot NIE implementuje getX(), getY() itp. -
// DZIEDZICZY je gotowe z ElementyGry. To eliminacja powielania kodu.
//
// Wskaźnik ElementyGry* może wskazywać na Przedmiot - dzięki temu
// Gra może iterować po WSZYSTKICH elementach naraz i wołać aktualizuj().
// ============================================================
class Przedmiot : public ElementyGry {
private:
    int m_wartosc;       // Punkty zdobywane po zebraniu
    bool m_superKebab;   // Duży kebab (K na mapie) - przestrasza wrogów

public:
    // Konstruktor wywołuje ElementyGry(x, y, width, height, speed=0)
    // speed=0 bo przedmiot się nie rusza
    Przedmiot(int x, int y, int width, int height, int wartosc, bool superKebab = false);

    // Przesłonięcie aktualizuj() - puste, bo kebab stoi w miejscu.
    // MUSI być zaimplementowane, bo jest czysto wirtualne w IAktualizowanie.
    void aktualizuj() override;

    // Przesłonięcie getTypGrafiki() - zależnie od typu zwraca
    // "kebsik" lub "kebsik_super". OknoGry użyje tej nazwy do wyboru tekstury.
    string getTypGrafiki() const override;

    int getWartosc() const;
    bool czySuper() const;
};

#endif
