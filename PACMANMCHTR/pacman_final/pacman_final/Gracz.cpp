#include "Gracz.h"

// Konstruktor wywołuje konstruktor klasy bazowej (ElementyGry) w liście inicjalizacyjnej.
// Parametr 'id' jest ignorowany — zachowany dla kompatybilności z sygnaturą.
Gracz::Gracz(int x, int y, int width, int height, int speed, int /*id*/, int lives, const Mapa* mapa)
    : ElementyGry(x, y, width, height, speed),
      m_lives(lives), m_score(0),
      m_kierunek(Kierunek::BRAK), m_nastepnyKierunek(Kierunek::BRAK),
      m_mapa(mapa), m_rozmiarKafelka(mapa->getRozmiarKafelka()),
      m_animFrame(0), m_umiera(false), m_umieraTimer(0),
      m_startX(x), m_startY(y)
{}

// Sprawdza 4 rogi prostokąta gracza — żaden nie może trafić w ścianę.
// margin sprawia, że hitbox jest nieco mniejszy niż kafelek (gracz "mieści się" w skrętach).
bool Gracz::mozePrzejsc(int nx, int ny) const {
    int T = m_rozmiarKafelka;
    int m = 2; // margines w pikselach
    return !m_mapa->czySciana((nx + m)     / T, (ny + m)     / T) &&
           !m_mapa->czySciana((nx + T-m-1) / T, (ny + m)     / T) &&
           !m_mapa->czySciana((nx + m)     / T, (ny + T-m-1) / T) &&
           !m_mapa->czySciana((nx + T-m-1) / T, (ny + T-m-1) / T);
}

// Przesuwa (nx, ny) o m_speed pikseli w kierunku k.
// Parametry przez referencję — zmiany widoczne w wywołującym.
void Gracz::probujPoruszycSie(Kierunek k, int& nx, int& ny) const {
    switch (k) {
        case Kierunek::GORE:  ny -= m_speed; break;
        case Kierunek::DOL:   ny += m_speed; break;
        case Kierunek::LEWO:  nx -= m_speed; break;
        case Kierunek::PRAWO: nx += m_speed; break;
        default: break;
    }
}

// POLIMORFIZM: wywołana przez Gra::aktualizujStanGry() przez wskaźnik ElementyGry*.
// Kompilator wybiera TĘ wersję (nie pustą z klasy bazowej) dzięki vtable.
void Gracz::aktualizuj() {
    m_animFrame++;

    // Animacja śmierci — czekaj 60 klatek (~2s), potem wróć do gry
    if (m_umiera) {
        m_umieraTimer++;
        if (m_umieraTimer > 60) { m_umiera = false; m_umieraTimer = 0; }
        return;
    }

    int nx = m_x, ny = m_y;

    // Bufor kierunku: gracz mógł wcisnąć klawisz przed zakrętem —
    // sprawdź czy teraz można już skręcić w ten kierunek.
    if (m_nastepnyKierunek != Kierunek::BRAK) {
        int tx = m_x, ty = m_y;
        probujPoruszycSie(m_nastepnyKierunek, tx, ty);
        if (mozePrzejsc(tx, ty)) {
            m_kierunek = m_nastepnyKierunek;
            m_nastepnyKierunek = Kierunek::BRAK;
        }
    }

    // Przesuń w aktualnym kierunku jeśli droga wolna
    probujPoruszycSie(m_kierunek, nx, ny);
    if (mozePrzejsc(nx, ny)) { m_x = nx; m_y = ny; }
}

// POLIMORFIZM: zwraca nazwę tekstury SVG.
// OknoGry wyszuka m_tekstury["studentdebil"] i narysuje gracza.
string Gracz::getTypGrafiki() const { return "studentdebil"; }

int      Gracz::getLives()    const { return m_lives; }
int      Gracz::getScore()    const { return m_score; }
Kierunek Gracz::getKierunek() const { return m_kierunek; }
int      Gracz::getAnimFrame() const { return m_animFrame; }
bool     Gracz::czyUmiera()   const { return m_umiera; }

void Gracz::addScore(int pts) { m_score += pts; }
void Gracz::ustawNastepnyKierunek(Kierunek k) { m_nastepnyKierunek = k; }

void Gracz::stracZycie() {
    m_lives--;
    m_umiera = true;
    m_umieraTimer = 0;
    m_kierunek = m_nastepnyKierunek = Kierunek::BRAK;
}

void Gracz::resetujPozycje() {
    m_x = m_startX; m_y = m_startY;
    m_kierunek = m_nastepnyKierunek = Kierunek::BRAK;
    m_umiera = false; m_umieraTimer = 0;
}
