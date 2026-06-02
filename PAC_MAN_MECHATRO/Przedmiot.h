#ifndef PRZEDMIOT_H
#define PRZEDMIOT_H

#include "elementy_gry.h"

class Przedmiot : public ElementyGry {
private:
    int m_wartosc;

public:
    Przedmiot(int x, int y, int width, int height, int wartosc);


    void update();


    int getWartosc() const;
};

#endif // PRZEDMIOT_H
