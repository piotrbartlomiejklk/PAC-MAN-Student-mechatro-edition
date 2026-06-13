#include "Gracz.h"

Gracz::Gracz(int x, int y, int width, int height, int speed, int id, int startingLives, const Mapa* mapa)
    : ElementyGry(x, y, width, height, speed),
      m_lives(startingLives), m_score(0), m_id(id),
      m_kierunek(Kierunek::BRAK), m_nastepnyKierunek(Kierunek::BRAK),
      m_mapa(mapa), m_rozmiarKafelka(mapa->getRozmiarKafelka()),
      m_animFrame(0), m_umiera(false), m_umieraTimer(0),
      m_startX(x), m_startY(y) {}

bool Gracz::mozePrzejsc(int nx, int ny) const {
    int T = m_rozmiarKafelka;
    // Sprawdz 4 rogi
    int margin = 2;
    return !m_mapa->czySciana((nx + margin) / T, (ny + margin) / T) &&
           !m_mapa->czySciana((nx + T - margin - 1) / T, (ny + margin) / T) &&
           !m_mapa->czySciana((nx + margin) / T, (ny + T - margin - 1) / T) &&
           !m_mapa->czySciana((nx + T - margin - 1) / T, (ny + T - margin - 1) / T);
}

void Gracz::probujPoruszycSie(Kierunek k, int& nx, int& ny) const {
    int spd = m_speed;
    switch (k) {
        case Kierunek::GORE: ny -= spd; break;
        case Kierunek::DOL:  ny += spd; break;
        case Kierunek::LEWO: nx -= spd; break;
        case Kierunek::PRAWO: nx += spd; break;
        default: break;
    }
}

void Gracz::aktualizuj() {
    if (m_umiera) {
        m_umieraTimer++;
        if (m_umieraTimer > 60) {
            m_umiera = false;
            m_umieraTimer = 0;
        }
        return;
    }

    m_animFrame++;

    int nx = m_x, ny = m_y;

    // Najpierw sprobuj nastepny kierunek (bufor)
    if (m_nastepnyKierunek != Kierunek::BRAK) {
        int tx = m_x, ty = m_y;
        probujPoruszycSie(m_nastepnyKierunek, tx, ty);
        if (mozePrzejsc(tx, ty)) {
            m_kierunek = m_nastepnyKierunek;
            m_nastepnyKierunek = Kierunek::BRAK;
        }
    }

    // Kontynuuj biezacy kierunek
    probujPoruszycSie(m_kierunek, nx, ny);
    if (mozePrzejsc(nx, ny)) {
        m_x = nx;
        m_y = ny;
    }

    // Teleport przez krawedzie (wiersze 0 i max)
    int maxX = m_mapa->getSzerokosc() * m_rozmiarKafelka;
    if (m_x < -m_width) m_x = maxX;
    if (m_x > maxX) m_x = -m_width;
}

string Gracz::getTypGrafiki() const { return "studentdebil"; }
int Gracz::getId() const { return m_id; }
int Gracz::getLives() const { return m_lives; }
int Gracz::getScore() const { return m_score; }

void Gracz::addScore(int pts) { m_score += pts; }

void Gracz::stracZycie() {
    m_lives--;
    m_umiera = true;
    m_umieraTimer = 0;
    m_kierunek = Kierunek::BRAK;
    m_nastepnyKierunek = Kierunek::BRAK;
}

bool Gracz::czyUmiera() const { return m_umiera; }

void Gracz::resetujPozycje() {
    m_x = m_startX;
    m_y = m_startY;
    m_kierunek = Kierunek::BRAK;
    m_nastepnyKierunek = Kierunek::BRAK;
    m_umiera = false;
    m_umieraTimer = 0;
}

void Gracz::ustawNastepnyKierunek(Kierunek k) { m_nastepnyKierunek = k; }
Kierunek Gracz::getKierunek() const { return m_kierunek; }
int Gracz::getAnimFrame() const { return m_animFrame; }
