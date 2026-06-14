#ifndef ELEMENTY_GRY_H
#define ELEMENTY_GRY_H

#include "Aktualizowanie.h"
#include "Rysowanie.h"

// ============================================================
// WIELOKROTNE DZIEDZICZENIE (MULTIPLE INHERITANCE)
// ============================================================
// ElementyGry dziedziczy JEDNOCZEŚNIE z dwóch interfejsów:
//   - IAktualizowanie  → musi zaimplementować aktualizuj()
//   - IRysowanie       → musi zaimplementować getX(), getY(), getWidth(),
//                        getHeight(), getTypGrafiki()
//
// ElementyGry jest KLASĄ ABSTRAKCYJNĄ - implementuje metody pozycji
// (getX, getY itp.) z IRysowanie, ale NIE implementuje:
//   - aktualizuj()    (z IAktualizowanie) - zostaje czysto wirtualna
//   - getTypGrafiki() (z IRysowanie)      - zostaje czysto wirtualna
//
// Dlatego nie można stworzyć obiektu ElementyGry bezpośrednio -
// muszą to robić klasy pochodne: Gracz, Wrog, Przedmiot.
//
// Ten wzorzec nazywa się "Template Method" / "Non-Virtual Interface".
// ============================================================
class ElementyGry : public IAktualizowanie, public IRysowanie {
protected:
    // Pola chronione (protected) - dostępne dla klas pochodnych
    // (Gracz, Wrog, Przedmiot), ale nie z zewnątrz.
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    int m_speed;
    bool m_aktywny;

public:
    // Konstruktor klasy bazowej - wywoływany przez konstruktory klas pochodnych
    // za pomocą listy inicjalizacyjnej: Gracz::Gracz(...) : ElementyGry(...)
    ElementyGry(int x, int y, int width, int height, int speed);

    // Wirtualny destruktor - wymagany przy polimorfizmie
    virtual ~ElementyGry() = default;

    // IMPLEMENTACJE metod z IRysowanie - te metody są ZDEFINIOWANE tutaj,
    // więc klasy pochodne DZIEDZICZĄ gotową implementację.
    // Użycie słowa kluczowego 'override' potwierdza, że nadpisujemy metodę wirtualną z bazowej.
    int getX() const override;
    int getY() const override;
    int getWidth() const override;
    int getHeight() const override;

    void setX(int x);
    void setY(int y);
    int getSpeed() const;

    bool czyAktywny() const;
    void dezaktywuj();

    // METODY CZYSTO WIRTUALNE - NIE zaimplementowane tutaj, muszą być
    // zaimplementowane przez każdą klasę pochodną (Gracz, Wrog, Przedmiot):
    //   virtual void aktualizuj() = 0;      (odziedziczona z IAktualizowanie)
    //   virtual string getTypGrafiki() = 0; (odziedziczona z IRysowanie)
};

#endif // ELEMENTY_GRY_H
