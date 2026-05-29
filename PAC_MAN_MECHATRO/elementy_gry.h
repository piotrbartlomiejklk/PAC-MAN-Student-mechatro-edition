#ifndef ELEMENTY_GRY_H
#define ELEMENTY_GRY_H

#endif // ELEMENTY_GRY_H
class Elementygry{
protected:
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    int m_speed;

public:
    Elementygry(int x, int y, int width, int height, int speed);
    ~Elementygry() = default;
    void update();
};
