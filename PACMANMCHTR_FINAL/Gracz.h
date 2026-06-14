#ifndef GRACZ_H
#define GRACZ_H

#include "elementy_gry.h"
#include "Mapa.h"

enum class Kierunek { BRAK, GORE, DOL, LEWO, PRAWO };

// ============================================================
// POLIMORFIZM: Gracz dziedziczy po ElementyGry.
// Nadpisuje (override) dwie metody wirtualne:
//   - aktualizuj()     → logika ruchu gracza
//   - getTypGrafiki()  → zwraca "studentdebil" (nazwa SVG)
//
// Gdy Gra woła e->aktualizuj() przez wskaźnik ElementyGry*,
// a obiekt jest Graczem — wykona się TA metoda (nie bazowa).
// To jest właśnie polimorfizm: właściwa metoda wybierana
// w czasie działania programu przez tablicę vtable.
// ============================================================
class Gracz : public ElementyGry {
private:
    int      m_lives;
    int      m_score;
    Kierunek m_kierunek;
    Kierunek m_nastepnyKierunek; // bufor: zapamiętaj klawisz przed zakrętem
    const Mapa* m_mapa;
    int      m_rozmiarKafelka;
    int      m_animFrame;
    bool     m_umiera;
    int      m_umieraTimer;
    int      m_startX, m_startY; // pozycja startowa do resetu

public:
    Gracz(int x, int y, int width, int height, int speed, int id, int lives, const Mapa* mapa);

    // POLIMORFIZM — override: nadpisujemy metodę wirtualną z klasy bazowej
    void   aktualizuj()    override;
    string getTypGrafiki() const override;

    int      getLives()    const;
    int      getScore()    const;
    Kierunek getKierunek() const;
    int      getAnimFrame() const;
    bool     czyUmiera()   const;

    void addScore(int pts);
    void ustawNastepnyKierunek(Kierunek k);
    void stracZycie();
    void resetujPozycje();

private:
    bool mozePrzejsc(int nx, int ny) const;
    void probujPoruszycSie(Kierunek k, int& nx, int& ny) const;
};

#endif
