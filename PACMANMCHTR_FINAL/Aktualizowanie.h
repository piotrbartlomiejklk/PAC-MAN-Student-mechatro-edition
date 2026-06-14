#ifndef AKTUALIZOWANIE_H
#define AKTUALIZOWANIE_H
using namespace std;

// ============================================================
// INTERFEJS (CZYSTO WIRTUALNA KLASA BAZOWA) - POLIMORFIZM
// ============================================================
// IAktualizowanie to tzw. "interfejs" - klasa abstrakcyjna,
// która definiuje KONTRAKT: każda klasa dziedzicząca MUSI
// zaimplementować metodę aktualizuj().
//
// Dzięki temu możemy trzymać wskaźniki do IAktualizowanie
// i wołać aktualizuj() bez wiedzy, co tak naprawdę jest pod spodem
// (Gracz? Wrog? Przedmiot?) - to właśnie jest POLIMORFIZM.
// ============================================================
class IAktualizowanie {
public:
    // Wirtualny destruktor - WYMAGANY gdy używamy polimorfizmu!
    // Bez niego delete na wskaźniku bazowym nie wywoła destruktora klasy pochodnej.
    virtual ~IAktualizowanie() = default;

    // CZYSTO WIRTUALNA METODA (= 0) - wymusza implementację w klasach pochodnych.
    // Jeśli klasa pochodna jej nie zaimplementuje, staje się też abstrakcyjna
    // i nie można jej instancjonować.
    virtual void aktualizuj() = 0;
};

#endif // AKTUALIZOWANIE_H
