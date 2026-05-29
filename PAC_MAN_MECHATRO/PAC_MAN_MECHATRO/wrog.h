#ifndef WROG_H
#define WROG_H

#include "elementy_gry.h"


enum class TypBota {
    LOSOWY,
    BFS
};

class Wrog : public ElementyGry {
private:
    TypBota m_typBota;

public:
    Wrog(int x, int y, int width, int height, int speed, TypBota typ);

    void update();
};

#endif // WROG_H
