#include "Gra.h"
#include <cstdlib>
#include <ctime>

Gra::Gra()
    : m_gracz(nullptr), m_stan(StanGry::GRAJ)
{
    srand((unsigned)time(nullptr));
}

bool Gra::inicjalizujPoziom(const string& sciezkaDoPliku) {
    m_sciezkaPoziom = sciezkaDoPliku;

    // Mapa wczytywana Z PLIKU przy każdym wywołaniu —
    // edycja poziom_1.txt lub poziom_2.txt jest widoczna po restarcie.
    if (!m_mapa.wczytaj_z_Pliku(sciezkaDoPliku)) return false;

    m_elementy.clear();
    m_wrogowie.clear();
    m_przedmioty.clear();
    m_gracz  = nullptr;
    m_stan   = StanGry::GRAJ;

    int T = m_mapa.getRozmiarKafelka();

    // Skanuj mapę i twórz obiekty dla każdego znaku specjalnego.
    // POLIMORFIZM TWORZENIA: tworzymy WrogLosowy / WrogBFS, ale
    // przechowujemy jako unique_ptr<ElementyGry> — klasa bazowa.
    for (int wiersz = 0; wiersz < m_mapa.getWysokosc(); wiersz++) {
        for (int kol = 0; kol < m_mapa.getSzerokosc(); kol++) {
            char z = m_mapa.getZnak(kol, wiersz);
            int  x = kol * T, y = wiersz * T;

            if (z == 'S') {
                auto g = make_unique<Gracz>(x, y, T, T, 6, 1, 3, &m_mapa); //5 to predkosc graca do chuja
                m_gracz = g.get();               // zapamiętaj surowy wskaźnik
                m_elementy.push_back(move(g));   // przenieś własność do wektora
                m_mapa.setZnak(kol, wiersz, '.'); // zostaw kebsika w miejscu startu

            } else if (z == 'L') {
                auto w = make_unique<WrogLosowy>(x, y, T, T, 4, &m_mapa); //4 predkosc tego chuja co cie goni ale nie leci do cb
                m_wrogowie.push_back(w.get());
                m_elementy.push_back(move(w));
                m_mapa.setZnak(kol, wiersz, '.');

            } else if (z == 'B') {
                auto w = make_unique<WrogBFS>(x, y, T, T, 5, &m_mapa); //5 to predkosc wroga ktory wie gdzie jestes do chuja
                m_wrogowie.push_back(w.get());
                m_elementy.push_back(move(w));
                m_mapa.setZnak(kol, wiersz, '.');

            } else if (z == 'K') {
                auto p = make_unique<Przedmiot>(x, y, T, T, 50, true);
                m_przedmioty.push_back(p.get());
                m_elementy.push_back(move(p));

            } else if (z == '.') {
                auto p = make_unique<Przedmiot>(x, y, T, T, 10, false);
                m_przedmioty.push_back(p.get());
                m_elementy.push_back(move(p));
            }
            // '#' = ściana — rysujemy z mapy, nie tworzymy obiektu
        }
    }
    return true;
}

void Gra::restart() {
    inicjalizujPoziom(m_sciezkaPoziom);
}

void Gra::aktualizujStanGry() {
    if (m_stan != StanGry::GRAJ || !m_gracz) return;

    // Podaj wrogiom BFS aktualną pozycję gracza.
    // dynamic_cast sprawdza w czasie wykonania czy wróg jest WrogBFS.
    // Zwraca nullptr dla WrogLosowy — pomijamy go (if bfs).
    for (auto* w : m_wrogowie) {
        auto* bfs = dynamic_cast<WrogBFS*>(w);
        if (bfs) bfs->ustawCelGracza(m_gracz->getX(), m_gracz->getY());
    }

    // POLIMORFIZM: jedna pętla wywołuje właściwą wersję aktualizuj()
    // dla każdego obiektu — Gracz, WrogLosowy, WrogBFS lub Przedmiot.
    for (auto& e : m_elementy) {
        if (e->czyAktywny()) e->aktualizuj(); // vtable wybiera właściwą metodę
    }

    if (!m_gracz->czyUmiera()) {
        sprawdzKolizjeGraczPrzedmioty();
        sprawdzKolizjeGraczWrogowie();
    }
    sprawdzWygrana();
}

void Gra::sprawdzKolizjeGraczPrzedmioty() {
    for (auto* p : m_przedmioty) {
        if (!p->czyAktywny()) continue;
        if (kolizja(m_gracz, p)) {
            m_gracz->addScore(p->getWartosc());
            p->dezaktywuj();
            if (p->czySuper())
                for (auto* w : m_wrogowie) w->przestrasz(300);
        }
    }
}

void Gra::sprawdzKolizjeGraczWrogowie() {
    for (auto* w : m_wrogowie) {
        if (!w->czyAktywny()) continue;
        if (kolizja(m_gracz, w)) {
            if (w->czyPrzestraszon()) {
                m_gracz->addScore(200);
                w->resetujPozycje();
            } else {
                m_gracz->stracZycie();
                if (m_gracz->getLives() <= 0)
                    m_stan = StanGry::GAME_OVER;
                else
                    for (auto* wr : m_wrogowie) wr->resetujPozycje();
                return;
            }
        }
    }
}

void Gra::sprawdzWygrana() {
    for (auto* p : m_przedmioty)
        if (p->czyAktywny()) return;
    m_stan = StanGry::WYGRANA;
}

// Kolizja AABB (oś-wyrównane prostokąty) z marginesem —
// margin sprawia że zderzenie nie jest co do piksela, lecz "dość blisko".
bool Gra::kolizja(ElementyGry* a, ElementyGry* b) const {
    int margin = 8;
    return a->getX() + margin         < b->getX() + b->getWidth()  - margin &&
           a->getX() + a->getWidth()  - margin > b->getX() + margin &&
           a->getY() + margin         < b->getY() + b->getHeight() - margin &&
           a->getY() + a->getHeight() - margin > b->getY() + margin;
}

const Mapa& Gra::getMapa() const { return m_mapa; }
const vector<unique_ptr<ElementyGry>>& Gra::getElementy() const { return m_elementy; }
StanGry Gra::getStan()  const { return m_stan; }
int Gra::getWynik() const { return m_gracz ? m_gracz->getScore() : 0; }
int Gra::getZycia() const { return m_gracz ? m_gracz->getLives() : 0; }
void Gra::ustawKierunkGracza(Kierunek k) { if (m_gracz) m_gracz->ustawNastepnyKierunek(k); }
