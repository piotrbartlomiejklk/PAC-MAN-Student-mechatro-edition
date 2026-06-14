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

enum class StanGry { GRAJ, GAME_OVER, WYGRANA };

// ============================================================
// Gra - zarządza logiką gry: mapą, elementami i kolizjami.
//
// POLIMORFIZM: m_elementy to vector<unique_ptr<ElementyGry>>,
// który przechowuje Gracz*, WrogLosowy*, WrogBFS* i Przedmiot*
// jako wskaźniki do wspólnej klasy bazowej ElementyGry.
// W aktualizujStanGry() wołamy e->aktualizuj() na każdym —
// kompilator sam wybiera właściwą wersję przez tablicę vtable.
// ============================================================
class Gra {
private:
    Mapa m_mapa;
    string m_sciezkaPoziom; // zapamiętana do restartu

    // Polimorficzny kontener WSZYSTKICH obiektów gry.
    // unique_ptr = automatyczne zwalnianie pamięci.
    vector<unique_ptr<ElementyGry>> m_elementy;

    // Pomocnicze wskaźniki dla szybkiego dostępu (nie są właścicielami pamięci)
    Gracz*             m_gracz;
    vector<Wrog*>      m_wrogowie;
    vector<Przedmiot*> m_przedmioty;

    StanGry m_stan;

public:
    Gra();

    // Wczytuje plik mapy i tworzy wszystkie elementy gry.
    // Wywołuj po każdym wyborze poziomu lub restarcie.
    bool inicjalizujPoziom(const string& sciezkaDoPliku);
    void restart();         // wczytuje plik mapy od nowa
    void aktualizujStanGry();

    const Mapa&                            getMapa()    const;
    const vector<unique_ptr<ElementyGry>>& getElementy() const;
    StanGry getStan()  const;
    int     getWynik() const;
    int     getZycia() const;

    void ustawKierunkGracza(Kierunek k);

private:
    void sprawdzKolizjeGraczPrzedmioty();
    void sprawdzKolizjeGraczWrogowie();
    void sprawdzWygrana();
    bool kolizja(ElementyGry* a, ElementyGry* b) const;
};

#endif
