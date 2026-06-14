#include "Mapa.h"
#include <fstream>
#include <algorithm>

Mapa::Mapa() {
    m_szerokosc = 0;
    m_wysokosc = 0;
    m_rozmiarKafelka = 40;
}

bool Mapa::wczytaj_z_Pliku(const string& sciezka) {
    ifstream plik(sciezka);
    if (!plik.is_open()) return false;

    m_siatka.clear();
    string linia;
    while (getline(plik, linia)) {
        // usun \r jesli Windows
        if (!linia.empty() && linia.back() == '\r') linia.pop_back();
        if (!linia.empty()) m_siatka.push_back(linia);
    }
    plik.close();

    if (m_siatka.empty()) return false;

    m_wysokosc = (int)m_siatka.size();
    m_szerokosc = (int)m_siatka[0].size();
    return true;
}

int Mapa::getSzerokosc() const { return m_szerokosc; }
int Mapa::getWysokosc() const { return m_wysokosc; }
int Mapa::getRozmiarKafelka() const { return m_rozmiarKafelka; }

char Mapa::getZnak(int kolumna, int wiersz) const {
    if (wiersz < 0 || wiersz >= m_wysokosc || kolumna < 0 || kolumna >= m_szerokosc)
        return '#';
    return m_siatka[wiersz][kolumna];
}

void Mapa::setZnak(int kolumna, int wiersz, char znak) {
    if (wiersz >= 0 && wiersz < m_wysokosc && kolumna >= 0 && kolumna < m_szerokosc)
        m_siatka[wiersz][kolumna] = znak;
}

bool Mapa::czySciana(int kolumna, int wiersz) const {
    return getZnak(kolumna, wiersz) == '#';
}

bool Mapa::czyPrzejezdne(int kolumna, int wiersz) const {
    char z = getZnak(kolumna, wiersz);
    return z != '#';
}

int Mapa::liczPunkty() const {
    int n = 0;
    for (auto& r : m_siatka)
        for (char c : r)
            if (c == '.' || c == 'K') n++;
    return n;
}
