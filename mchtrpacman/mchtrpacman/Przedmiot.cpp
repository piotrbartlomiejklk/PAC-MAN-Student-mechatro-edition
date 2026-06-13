#include "Przedmiot.h"

Przedmiot::Przedmiot(int x, int y, int width, int height, int wartosc, bool superKebab)
    : ElementyGry(x, y, width, height, 0), m_wartosc(wartosc), m_superKebab(superKebab) {}

void Przedmiot::aktualizuj() {}

string Przedmiot::getTypGrafiki() const {
    return m_superKebab ? "kebsik_super" : "kebsik";
}

int Przedmiot::getWartosc() const { return m_wartosc; }
bool Przedmiot::czySuper() const { return m_superKebab; }
