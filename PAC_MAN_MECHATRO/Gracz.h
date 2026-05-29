#ifndef GRACZ_H
#define GRACZ_H

#include "elementy_gry.h"

using namespace std;

class Gracz : public ElementyGry {
private:
    int m_lives;
    int m_score;
    int m_id;

public:
    Gracz(int x, int y, int width, int height, int speed, int id, int startingLives = 3);
    void update();
    int getId() const;
};

#endif
