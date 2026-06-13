#include "Gra.h"
#include <cstdlib>
#include <ctime>

Gra::Gra() : m_stan(StanGry::MENU), m_poziom(1), m_gracz(nullptr),
             m_punktyPoziom(0), m_calkowityWynik(0) {
    srand((unsigned)time(nullptr));
}

bool Gra::inicjalizujPoziom(const string& sciezkaDoPliku) {
    m_sciezkaPoziom = sciezkaDoPliku;
    if (!m_mapa.wczytaj_z_Pliku(sciezkaDoPliku)) return false;

    m_elementy.clear();
    m_wrogowie.clear();
    m_przedmioty.clear();
    m_gracz = nullptr;
    m_punktyPoziom = 0;

    int T = m_mapa.getRozmiarKafelka();

    for (int wiersz = 0; wiersz < m_mapa.getWysokosc(); wiersz++) {
        for (int kol = 0; kol < m_mapa.getSzerokosc(); kol++) {
            char z = m_mapa.getZnak(kol, wiersz);
            int x = kol * T, y = wiersz * T;

            if (z == 'S') {
                auto g = make_unique<Gracz>(x, y, T, T, 3, 1, 3, &m_mapa);
                m_gracz = g.get();
                m_elementy.push_back(move(g));
                m_mapa.setZnak(kol, wiersz, '.');
            } else if (z == 'L') {
                auto w = make_unique<WrogLosowy>(x, y, T, T, 2, &m_mapa);
                m_wrogowie.push_back(w.get());
                m_elementy.push_back(move(w));
                m_mapa.setZnak(kol, wiersz, '.');
            } else if (z == 'B') {
                auto w = make_unique<WrogBFS>(x, y, T, T, 2, &m_mapa);
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
        }
    }

    m_stan = StanGry::GRAJ;
    return true;
}

void Gra::aktualizujStanGry() {
    if (m_stan != StanGry::GRAJ) return;
    if (!m_gracz) return;

    // Aktualizuj wrogowie BFS - daj im pozycje gracza
    for (auto* w : m_wrogowie) {
        auto* bfs = dynamic_cast<WrogBFS*>(w);
        if (bfs) bfs->ustawCelGracza(m_gracz->getX(), m_gracz->getY());
    }

    // Aktualizuj wszystkie elementy
    for (auto& e : m_elementy) {
        if (e->czyAktywny()) e->aktualizuj();
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
            int pts = p->getWartosc();
            m_gracz->addScore(pts);
            m_calkowityWynik += pts;
            m_punktyPoziom++;
            p->dezaktywuj();

            // Super kebab - przestrasz wrogow
            if (p->czySuper()) {
                for (auto* w : m_wrogowie) w->przestrasz(300);
            }
        }
    }
}

void Gra::sprawdzKolizjeGraczWrogowie() {
    for (auto* w : m_wrogowie) {
        if (!w->czyAktywny()) continue;
        if (kolizja(m_gracz, w)) {
            if (w->czyPrzestraszon()) {
                // Gracz zje wroga
                m_gracz->addScore(200);
                m_calkowityWynik += 200;
                w->resetujPozycje();
            } else {
                m_gracz->stracZycie();
                if (m_gracz->getLives() <= 0) {
                    m_stan = StanGry::GAME_OVER;
                } else {
                    // Reset pozycji wszystkich wrogow
                    for (auto* wr : m_wrogowie) wr->resetujPozycje();
                }
                return;
            }
        }
    }
}

void Gra::sprawdzWygrana() {
    bool pozostaly = false;
    for (auto* p : m_przedmioty) {
        if (p->czyAktywny()) { pozostaly = true; break; }
    }
    if (!pozostaly) m_stan = StanGry::WYGRANA;
}

bool Gra::kolizja(ElementyGry* a, ElementyGry* b) const {
    int margin = 8;
    return a->getX() + margin < b->getX() + b->getWidth() - margin &&
           a->getX() + a->getWidth() - margin > b->getX() + margin &&
           a->getY() + margin < b->getY() + b->getHeight() - margin &&
           a->getY() + a->getHeight() - margin > b->getY() + margin;
}

const Mapa& Gra::getMapa() const { return m_mapa; }
const vector<unique_ptr<ElementyGry>>& Gra::getElementy() const { return m_elementy; }
StanGry Gra::getStan() const { return m_stan; }

void Gra::ustawKierunkGracza(Kierunek k) {
    if (m_gracz) m_gracz->ustawNastepnyKierunek(k);
}

int Gra::getWynik() const { return m_gracz ? m_gracz->getScore() : 0; }
int Gra::getZycia() const { return m_gracz ? m_gracz->getLives() : 0; }
int Gra::getPoziom() const { return m_poziom; }

void Gra::restart() {
    m_calkowityWynik = 0;
    inicjalizujPoziom(m_sciezkaPoziom);
}
