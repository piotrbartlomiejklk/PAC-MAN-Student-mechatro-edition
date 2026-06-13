#ifndef ELEMENTY_GRY_H
#define ELEMENTY_GRY_H

#include "Aktualizowanie.h"
#include "Rysowanie.h"

class ElementyGry : public IAktualizowanie, public IRysowanie {
protected:
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    int m_speed;
    bool m_aktywny;

public:
    ElementyGry(int x, int y, int width, int height, int speed);
    virtual ~ElementyGry() = default;

    int getX() const override;
    int getY() const override;
    int getWidth() const override;
    int getHeight() const override;

    void setX(int x);
    void setY(int y);
    int getSpeed() const;

    bool czyAktywny() const;
    void dezaktywuj();
};

#endif // ELEMENTY_GRY_H
