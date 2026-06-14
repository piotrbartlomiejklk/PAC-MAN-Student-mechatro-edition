#ifndef OKNO_GRY_H
#define OKNO_GRY_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QKeyEvent>
#include <QLabel>
#include <QMap>
#include <QPixmap>
#include "Gra.h"

// Dwa stany okna: wybór poziomu lub rozgrywka
enum class StanEkranu { WYBOR_POZIOMU, GRAMY };

// ============================================================
// OknoGry dziedziczy z QWidget — polimorfizm Qt.
// Nadpisuje keyPressEvent() i resizeEvent() z QWidget.
//
// Zamiast pliku .ui (Qt Designer) budujemy interfejs kodem.
// Całość grafiki pochodzi z SVG w katalogu projektu (obok .exe).
// ============================================================
class OknoGry : public QWidget {
    Q_OBJECT

private:
    Gra            m_sesja;
    StanEkranu     m_stan;

    // --- Grafika gry ---
    QGraphicsScene* m_scena;
    QGraphicsView*  m_widok;
    QTimer*         m_zegar;

    // --- HUD ---
    QLabel* m_labelWynik;
    QLabel* m_labelZycia;
    QLabel* m_labelInfo;    // stopka: komunikaty i podpowiedzi

    // --- Ekran wyboru poziomu (nakładka na widok) ---
    QWidget* m_panelWyboru;
    QLabel*  m_wyborTytul;
    QLabel*  m_wyborOpcja1; // Poziom 1 — klasyczny
    QLabel*  m_wyborOpcja2; // Poziom 2 — trudny
    int      m_wyborKursor; // 0 lub 1 — podświetlona opcja

    // --- Tekstury SVG (ładowane raz, reużywane co klatkę) ---
    QMap<QString, QPixmap> m_tekstury;
    int m_animTick; // licznik klatek

public:
    explicit OknoGry(QWidget* parent = nullptr);

protected:
    // POLIMORFIZM Qt: nadpisujemy metody wirtualne z QWidget
    void keyPressEvent(QKeyEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void onKlatka();

private:
    void ladujTekstury();
    QPixmap renderujSVG(const QString& sciezka);
    QPixmap getTekstura(const QString& nazwa);

    void pokazEkranWyboru();
    void rozpocznijPoziom(int nrPoziomu);  // 1 lub 2
    void aktualizujPodswietlenieWyboru();

    void renderujGre();
    void rysujMape();
    void rysujElementy();   // polimorficzne — używa getTypGrafiki()
    void rysujHUD();
    void rysujOverlay();    // GAME OVER / WYGRANA
    void rysujTloBudynku(); // dekoracja boczna

    void centrujWidok();
};

#endif
