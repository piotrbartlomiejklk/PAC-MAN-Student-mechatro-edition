#ifndef MAPA_H
#define MAPA_H

#include <string>
#include <vector>

using namespace std;

class Mapa {
private:
    vector<string> m_siatka;
    int m_szerokosc;
    int m_wysokosc;
    int m_rozmiarKafelka;

public:
    Mapa();
    bool wczytaj_z_Pliku(const string& sciezka);

    int getSzerokosc() const;
    int getWysokosc() const;
    int getRozmiarKafelka() const;

    char getZnak(int kolumna, int wiersz) const;
    void setZnak(int kolumna, int wiersz, char znak);

    bool czySciana(int kolumna, int wiersz) const;
    bool czyPrzejezdne(int kolumna, int wiersz) const;

    int liczPunkty() const;
};

#endif // MAPA_H
