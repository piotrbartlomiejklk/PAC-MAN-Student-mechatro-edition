#ifndef GRA_H
#define GRA_H

#include <vector>
#include <memory>
#include <string>

#include "Mapa.h"
#include "Gracz.h"
#include "Wrog.h"
#include "Przedmiot.h"

using namespace std;

class Gra {
private:
    Mapa m_mapa;
    vector<unique_ptr<Gracz>> m_gracze;
    vector<unique_ptr<Wrog>> m_wrogowie;
    vector<unique_ptr<Przedmiot>> m_przedmioty;
    bool m_graTrwa;

public:
    Gra();
    bool inicjalizujPoziom(const string& sciezkaDoPliku);
    void aktualizujStanGry();
    const Mapa& getMapa() const;
    const vector<unique_ptr<Gracz>>& getGracze() const;
    const vector<unique_ptr<Wrog>>& getWrogowie() const;
    const vector<unique_ptr<Przedmiot>>& getPrzedmioty() const;
    bool czyGraTrwa() const;
};

#endif //
