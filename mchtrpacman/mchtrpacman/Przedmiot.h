#ifndef PRZEDMIOT_H
#define PRZEDMIOT_H

#include "elementy_gry.h"

class Przedmiot : public ElementyGry {
private:
    int m_wartosc;
    bool m_superKebab; // duzy punkt (K)

public:
    Przedmiot(int x, int y, int width, int height, int wartosc, bool superKebab = false);

    void aktualizuj() override;
    string getTypGrafiki() const override;

    int getWartosc() const;
    bool czySuper() const;
};

#endif
