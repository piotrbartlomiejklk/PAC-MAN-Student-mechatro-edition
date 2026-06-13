#ifndef RYSOWANIE_H
#define RYSOWANIE_H
#include <string>

using namespace std;

class IRysowanie {
public:
    virtual ~IRysowanie() = default;

    // Wymagane funkcje dla grafiki
    virtual int getX() const = 0;
    virtual int getY() const = 0;
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;

    // Zwraca np. "student" albo "wrog" - żeby Qt wiedziało jaki plik SVG załadować
    virtual string getTypGrafiki() const = 0;
};
#endif // RYSOWANIE_H
