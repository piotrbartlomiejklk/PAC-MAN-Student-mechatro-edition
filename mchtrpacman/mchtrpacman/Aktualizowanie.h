#ifndef AKTUALIZOWANIE_H
#define AKTUALIZOWANIE_H
using namespace std;

class IAktualizowanie {
public:
    virtual ~IAktualizowanie() = default;

    // = 0 oznacza, że ta metoda jest "czysto wirtualna".
    // To właśnie jest ten twardy wymóg kontraktu.
    virtual void aktualizuj() = 0;
};
#endif // AKTUALIZOWANIE_H
