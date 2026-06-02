#ifndef ELEMENTY_GRY_H
#define ELEMENTY_GRY_H

class ElementyGry {
private:
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    int m_speed;

public:
    ElementyGry(int x, int y, int width, int height, int speed);

    virtual ~ElementyGry() = default;

    virtual void update() = 0;
};

#endif // ELEMENTY_GRY_H
