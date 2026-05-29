#ifndef GRACZ_H
#define GRACZ_H
#include "elementy_gry.h"
#endif // GRACZ_H

class Gracz : Elementygry{
private:
    int m_lives;
    int m_score;

public:
    Gracz(int x, int y, int width, int height, int speed, int startingLives = 3);
    void update();

};
