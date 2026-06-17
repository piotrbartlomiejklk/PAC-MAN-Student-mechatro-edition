#include "elementy_gry.h"

// Konstruktor klasy bazowej ElementyGry.
// Klasy pochodne (Gracz, Wrog, Przedmiot) wywołują go ze swoich list inicjalizacyjnych,
// np.: Gracz::Gracz(...) : ElementyGry(x, y, width, height, speed), m_lives(...) { }
// Dzięki temu wspólne pola (pozycja, rozmiar, prędkość) są inicjowane JEDEN RAZ w jednym miejscu.
ElementyGry::ElementyGry(int x, int y, int width, int height, int speed)
    : m_x(x), m_y(y), m_width(width), m_height(height), m_speed(speed), m_aktywny(true) {}

// Implementacje metod z interfejsu IRysowanie.
// Wszystkie klasy pochodne DZIEDZICZĄ te implementacje - nie muszą ich przepisywać.
// To jeden z kluczowych benefitów dziedziczenia: unikamy powielania kodu.
int ElementyGry::getX() const { return m_x; }
int ElementyGry::getY() const { return m_y; }
int ElementyGry::getWidth() const { return m_width; }
int ElementyGry::getHeight() const { return m_height; }
void ElementyGry::setX(int x) { m_x = x; }
void ElementyGry::setY(int y) { m_y = y; }
int ElementyGry::getSpeed() const { return m_speed; }
bool ElementyGry::czyAktywny() const { return m_aktywny; }
void ElementyGry::dezaktywuj() { m_aktywny = false; }
