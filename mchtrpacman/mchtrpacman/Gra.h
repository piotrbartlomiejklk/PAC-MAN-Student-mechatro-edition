#ifndef GRA_H
#define GRA_H

#include <vector>
#include <memory>
#include <string>
#include "Mapa.h"
#include "elementy_gry.h"
#include "Gracz.h"
#include "wrog.h"
#include "Przedmiot.h"

using namespace std;

enum class StanGry { MENU, GRAJ, PAUSE, GAME_OVER, WYGRANA };

class Gra {
private:
    Mapa m_mapa;
    vector<unique_ptr<ElementyGry>> m_elementy;
    StanGry m_stan;
    string m_sciezkaPoziom;
    int m_poziom;

    Gracz* m_gracz;
    vector<Wrog*> m_wrogowie;
    vector<Przedmiot*> m_przedmioty;
    int m_punktyPoziom;
    int m_calkowityWynik;

public:
    Gra();
    bool inicjalizujPoziom(const string& sciezkaDoPliku);
    void aktualizujStanGry();
    void restart();

    const Mapa& getMapa() const;
    const vector<unique_ptr<ElementyGry>>& getElementy() const;

    StanGry getStan() const;
    void ustawKierunkGracza(Kierunek k);

    int getWynik() const;
    int getZycia() const;
    int getPoziom() const;

private:
    void sprawdzKolizjeGraczPrzedmioty();
    void sprawdzKolizjeGraczWrogowie();
    void sprawdzWygrana();
    bool kolizja(ElementyGry* a, ElementyGry* b) const;
};

#endif
