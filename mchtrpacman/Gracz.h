#ifndef GRACZ_H
#define GRACZ_H

#include "elementy_gry.h"
#include "Mapa.h"

enum class Kierunek { BRAK, GORE, DOL, LEWO, PRAWO };

class Gracz : public ElementyGry {
private:
    int m_lives;
    int m_score;
    int m_id;
    Kierunek m_kierunek;
    Kierunek m_nastepnyKierunek;
    const Mapa* m_mapa;
    int m_rozmiarKafelka;
    int m_animFrame;
    bool m_umiera;
    int m_umieraTimer;
    int m_startX;
    int m_startY;

public:
    Gracz(int x, int y, int width, int height, int speed, int id, int startingLives, const Mapa* mapa);

    void aktualizuj() override;
    string getTypGrafiki() const override;

    int getId() const;
    int getLives() const;
    int getScore() const;
    void addScore(int pts);
    void stracZycie();
    bool czyUmiera() const;
    void resetujPozycje();

    void ustawNastepnyKierunek(Kierunek k);
    Kierunek getKierunek() const;
    int getAnimFrame() const;

private:
    bool mozePrzejsc(int nx, int ny) const;
    void probujPoruszycSie(Kierunek k, int& nx, int& ny) const;
};

#endif
