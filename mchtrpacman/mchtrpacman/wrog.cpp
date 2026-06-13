#include "wrog.h"
#include <cstdlib>
#include <queue>
#include <map>
#include <cmath>

Wrog::Wrog(int x, int y, int width, int height, int speed, const Mapa* mapa)
    : ElementyGry(x, y, width, height, speed),
      m_mapa(mapa), m_rozmiarKafelka(mapa->getRozmiarKafelka()),
      m_dx(1), m_dy(0), m_przestraszony(false), m_przestraszonyTimer(0),
      m_startX(x), m_startY(y), m_animFrame(0) {}

bool Wrog::czyPrzestraszon() const { return m_przestraszony; }

void Wrog::przestrasz(int czas) {
    m_przestraszony = true;
    m_przestraszonyTimer = czas;
}

void Wrog::resetujPozycje() {
    m_x = m_startX;
    m_y = m_startY;
    m_dx = 1; m_dy = 0;
    m_przestraszony = false;
    m_przestraszonyTimer = 0;
}

int Wrog::getAnimFrame() const { return m_animFrame; }

bool Wrog::mozePrzejsc(int nx, int ny) const {
    int T = m_rozmiarKafelka;
    int margin = 3;
    return !m_mapa->czySciana((nx + margin) / T, (ny + margin) / T) &&
           !m_mapa->czySciana((nx + T - margin - 1) / T, (ny + margin) / T) &&
           !m_mapa->czySciana((nx + margin) / T, (ny + T - margin - 1) / T) &&
           !m_mapa->czySciana((nx + T - margin - 1) / T, (ny + T - margin - 1) / T);
}

pair<int,int> Wrog::getKafelek() const {
    int T = m_rozmiarKafelka;
    return {m_x / T, m_y / T};
}

vector<pair<int,int>> Wrog::getSasiedzPrzejezdni(int kx, int ky, int wyklDx, int wyklDy) const {
    vector<pair<int,int>> wynik;
    int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
    for (auto& d : dirs) {
        if (d[0] == -wyklDx && d[1] == -wyklDy) continue; // nie cofaj sie
        int nx = kx + d[0], ny = ky + d[1];
        if (m_mapa->czyPrzejezdne(nx, ny))
            wynik.push_back({nx, ny});
    }
    return wynik;
}

// ---- WrogLosowy ----
WrogLosowy::WrogLosowy(int x, int y, int w, int h, int spd, const Mapa* mapa)
    : Wrog(x, y, w, h, spd, mapa) {}

void WrogLosowy::aktualizuj() {
    m_animFrame++;
    if (m_przestraszony) {
        m_przestraszonyTimer--;
        if (m_przestraszonyTimer <= 0) m_przestraszony = false;
    }

    int T = m_rozmiarKafelka;
    // Poruszaj sie piks po pikselu
    int nx = m_x + m_dx * m_speed;
    int ny = m_y + m_dy * m_speed;

    if (mozePrzejsc(nx, ny)) {
        m_x = nx; m_y = ny;
    } else {
        // Stoj na srodku kafelka i wybierz nowy kierunek
        m_x = (m_x / T) * T;
        m_y = (m_y / T) * T;
        auto sasiedzi = getSasiedzPrzejezdni(m_x / T, m_y / T, m_dx, m_dy);
        if (!sasiedzi.empty()) {
            auto [skx, sky] = sasiedzi[rand() % sasiedzi.size()];
            m_dx = skx - m_x / T;
            m_dy = sky - m_y / T;
        } else {
            m_dx = -m_dx; m_dy = -m_dy;
        }
    }
}

string WrogLosowy::getTypGrafiki() const {
    return m_przestraszony ? "skalniak_przestraszony" : "skalniak";
}

// ---- WrogBFS ----
WrogBFS::WrogBFS(int x, int y, int w, int h, int spd, const Mapa* mapa)
    : Wrog(x, y, w, h, spd, mapa), m_graczX(0), m_graczY(0) {}

void WrogBFS::ustawCelGracza(int gx, int gy) {
    m_graczX = gx;
    m_graczY = gy;
}

void WrogBFS::aktualizuj() {
    m_animFrame++;
    if (m_przestraszony) {
        m_przestraszonyTimer--;
        if (m_przestraszonyTimer <= 0) m_przestraszony = false;
    }

    int T = m_rozmiarKafelka;
    // Aktualizuj tylko kiedy jestesmy "na siatce" (wyrownani do kafelka)
    bool naKafelku = ((m_x % T) == 0 && (m_y % T) == 0);

    int nx = m_x + m_dx * m_speed;
    int ny = m_y + m_dy * m_speed;

    if (mozePrzejsc(nx, ny) && !naKafelku) {
        m_x = nx; m_y = ny;
        return;
    }

    // Wyrownaj do kafelka
    m_x = (m_x / T) * T;
    m_y = (m_y / T) * T;

    int startKx = m_x / T, startKy = m_y / T;
    int celKx = m_graczX / T, celKy = m_graczY / T;

    if (m_przestraszony) {
        // Uciekaj od gracza - wybierz najdalsza opcje
        auto sasiedzi = getSasiedzPrzejezdni(startKx, startKy, 0, 0);
        if (!sasiedzi.empty()) {
            int maxDist = -1;
            pair<int,int> best = sasiedzi[0];
            for (auto [sx, sy] : sasiedzi) {
                int d = abs(sx - celKx) + abs(sy - celKy);
                if (d > maxDist) { maxDist = d; best = {sx, sy}; }
            }
            m_dx = best.first - startKx;
            m_dy = best.second - startKy;
        }
        m_x += m_dx * m_speed;
        m_y += m_dy * m_speed;
        return;
    }

    // BFS
    map<pair<int,int>, pair<int,int>> parent;
    queue<pair<int,int>> q;
    pair<int,int> start = {startKx, startKy};
    pair<int,int> cel = {celKx, celKy};

    q.push(start);
    parent[start] = {-1,-1};

    bool znaleziono = false;
    while (!q.empty() && !znaleziono) {
        auto [cx, cy] = q.front(); q.pop();
        if (cx == celKx && cy == celKy) { znaleziono = true; break; }

        int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
        for (auto& d : dirs) {
            pair<int,int> n = {cx + d[0], cy + d[1]};
            if (parent.find(n) == parent.end() && m_mapa->czyPrzejezdne(n.first, n.second)) {
                parent[n] = {cx, cy};
                q.push(n);
            }
        }
    }

    if (znaleziono) {
        // Odtworz sciezke i zrob pierwszy krok
        pair<int,int> cur = cel;
        while (parent[cur] != start && parent[cur].first != -1) {
            cur = parent[cur];
        }
        if (cur != start) {
            m_dx = cur.first - startKx;
            m_dy = cur.second - startKy;
        }
    } else {
        auto sasiedzi = getSasiedzPrzejezdni(startKx, startKy, 0, 0);
        if (!sasiedzi.empty()) {
            auto [sx, sy] = sasiedzi[rand() % sasiedzi.size()];
            m_dx = sx - startKx;
            m_dy = sy - startKy;
        }
    }

    nx = m_x + m_dx * m_speed;
    ny = m_y + m_dy * m_speed;
    if (mozePrzejsc(nx, ny)) {
        m_x = nx; m_y = ny;
    }
}

string WrogBFS::getTypGrafiki() const {
    return m_przestraszony ? "skalniak_przestraszony" : "skalniak";
}
