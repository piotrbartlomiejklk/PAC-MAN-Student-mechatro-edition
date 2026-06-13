#ifndef WROG_H
#define WROG_H

#include "elementy_gry.h"
#include "Mapa.h"
#include <vector>
#include <utility>

using namespace std;

class Wrog : public ElementyGry {
protected:
    const Mapa* m_mapa;
    int m_rozmiarKafelka;
    int m_dx;
    int m_dy;
    bool m_przestraszony;
    int m_przestraszonyTimer;
    int m_startX;
    int m_startY;
    int m_animFrame;

public:
    Wrog(int x, int y, int width, int height, int speed, const Mapa* mapa);

    bool czyPrzestraszon() const;
    void przestrasz(int czas);
    void resetujPozycje();
    int getAnimFrame() const;

protected:
    bool mozePrzejsc(int nx, int ny) const;
    pair<int,int> getKafelek() const;
    vector<pair<int,int>> getSasiedzPrzejezdni(int kx, int ky, int wyklDx, int wyklDy) const;
};

class WrogLosowy : public Wrog {
public:
    WrogLosowy(int x, int y, int width, int height, int speed, const Mapa* mapa);
    void aktualizuj() override;
    string getTypGrafiki() const override;
};

class WrogBFS : public Wrog {
private:
    int m_graczX;
    int m_graczY;

public:
    WrogBFS(int x, int y, int width, int height, int speed, const Mapa* mapa);
    void ustawCelGracza(int gx, int gy);
    void aktualizuj() override;
    string getTypGrafiki() const override;
};

#endif
