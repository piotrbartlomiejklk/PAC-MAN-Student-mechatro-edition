#ifndef MAPA_H
#define MAPA_H

#include <vector>
#include <string>

using namespace std;
enum class TypKafelka {
    SCIANA,
    SCIEZKA
};

class Mapa {
private:
    vector<vector<TypKafelka>> m_siatka;
    int m_szerokosc;
    int m_wysokosc;

public:
    Mapa();


    bool wczytaj_z_Pliku(const string& sciezka);


    TypKafelka getKafelek(int x, int y) const;


    bool czyKolizja(int x, int y) const;

    int getSzerokosc() const;
    int getWysokosc() const;
};

#endif // MAPA_H
