#ifndef RYSOWANIE_H
#define RYSOWANIE_H
#include <string>

using namespace std;

// ============================================================
// INTERFEJS RYSOWANIA - POLIMORFIZM
// ============================================================
// IRysowanie to drugi interfejs w projekcie. Definiuje kontrakt
// dla wszystkich obiektów, które można narysować na ekranie.
//
// Warstwa graficzna (OknoGry) operuje na wskaźnikach IRysowanie*
// i nie musi wiedzieć, czy rysuje Gracza, Wroga czy Przedmiot -
// wywołuje te same metody getX(), getY(), getTypGrafiki() itd.
// POLIMORFICZNIE. To oddziela logikę gry od warstwy wyświetlania.
// ============================================================
class IRysowanie {
public:
    virtual ~IRysowanie() = default;

    // Czysto wirtualne metody pozycji i rozmiaru -
    // każdy element gry musi umieć powiedzieć gdzie jest i jak duży jest.
    virtual int getX() const = 0;
    virtual int getY() const = 0;
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;

    // Zwraca nazwę grafiki do załadowania (np. "studentdebil", "skalniak", "kebsik").
    // Dzięki tej metodzie warstwa graficzna może wybrać odpowiedni plik SVG
    // bez sprawdzania typu obiektu przez dynamic_cast.
    virtual string getTypGrafiki() const = 0;
};

#endif // RYSOWANIE_H
