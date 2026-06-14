#ifndef WROG_H
#define WROG_H

#include "elementy_gry.h"
#include "Mapa.h"
#include <vector>
#include <utility>

using namespace std;

// ============================================================
// DZIEDZICZENIE: Wrog -> ElementyGry -> IAktualizowanie + IRysowanie
// ============================================================
// Wrog to ABSTRAKCYJNA klasa bazowa dla wszystkich typów wrogów.
// Sama NIE implementuje aktualizuj() ani getTypGrafiki() - zostawia
// je klasom pochodnym (WrogLosowy, WrogBFS).
//
// Hierarchy:
//   ElementyGry (abstrakcyjna)
//       └─ Wrog (abstrakcyjna - wspólny stan i metody pomocnicze)
//             ├─ WrogLosowy (konkretna - losowy ruch)
//             └─ WrogBFS    (konkretna - inteligentne śledzenie)
//
// Dzięki temu Gra::m_wrogowie to vector<Wrog*> i może zawierać
// oba typy wrogów - wywołanie w->aktualizuj() wywoła WŁAŚCIWĄ
// implementację dzięki polimorfizmowi (late binding / vtable).
// ============================================================
class Wrog : public ElementyGry {
protected:
    // Pola chronione - dostępne w WrogLosowy i WrogBFS
    const Mapa* m_mapa;
    int m_rozmiarKafelka;
    int m_dx;   // Aktualny kierunek X (-1, 0, 1)
    int m_dy;   // Aktualny kierunek Y (-1, 0, 1)
    bool m_przestraszony;       // Tryb ucieczki po zjedzeniu super kebaba
    int m_przestraszonyTimer;
    int m_startX;
    int m_startY;
    int m_animFrame;

public:
    // Konstruktor klasy bazowej Wrog - wywoływany przez klasy pochodne
    Wrog(int x, int y, int width, int height, int speed, const Mapa* mapa);

    bool czyPrzestraszon() const;
    void przestrasz(int czas);
    void resetujPozycje();
    int getAnimFrame() const;

protected:
    // Metody pomocnicze współdzielone między WrogLosowy i WrogBFS.
    // Umieszczone w klasie bazowej żeby uniknąć powielania kodu.
    bool mozePrzejsc(int nx, int ny) const;
    pair<int,int> getKafelek() const;
    vector<pair<int,int>> getSasiedzPrzejezdni(int kx, int ky, int wyklDx, int wyklDy) const;
};

// ============================================================
// DZIEDZICZENIE: WrogLosowy -> Wrog -> ElementyGry
// ============================================================
// WrogLosowy to KONKRETNA klasa pochodna - implementuje:
//   - aktualizuj() override  ← losowy ruch po korytarzach
//   - getTypGrafiki() override ← "skalniak" lub "skalniak_przestraszony"
//
// POLIMORFIZM w akcji:
//   Wrog* w = new WrogLosowy(...);
//   w->aktualizuj();   // wywoła WrogLosowy::aktualizuj() !
// ============================================================
class WrogLosowy : public Wrog {
public:
    WrogLosowy(int x, int y, int width, int height, int speed, const Mapa* mapa);

    // override - przesłonięcie metod abstrakcyjnych z klas bazowych
    void aktualizuj() override;
    string getTypGrafiki() const override;
};

// ============================================================
// DZIEDZICZENIE: WrogBFS -> Wrog -> ElementyGry
// ============================================================
// WrogBFS to drugi konkretny typ wroga - używa algorytmu BFS
// (przeszukiwania wszerz) do śledzenia gracza.
//
// Ciekawy przykład polimorfizmu: OknoGry robi dynamic_cast<WrogBFS*>
// żeby narysować pomarańczową ramkę - w ten sposób odróżnia oba typy
// wrogów MIMO że oba są trzymane jako Wrog* w wektorze.
// ============================================================
class WrogBFS : public Wrog {
private:
    // WrogBFS potrzebuje znać cel (pozycję gracza) do BFS
    int m_graczX;
    int m_graczY;

public:
    WrogBFS(int x, int y, int width, int height, int speed, const Mapa* mapa);

    // Metoda specyficzna dla WrogBFS - nie istnieje w klasie bazowej Wrog.
    // Gra::aktualizujStanGry() musi zrobić dynamic_cast żeby ją wywołać.
    void ustawCelGracza(int gx, int gy);

    // override tych samych metod co WrogLosowy - inna implementacja!
    void aktualizuj() override;
    string getTypGrafiki() const override;
};

#endif
