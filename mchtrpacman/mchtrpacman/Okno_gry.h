#ifndef OKNO_GRY_H
#define OKNO_GRY_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QKeyEvent>
#include <QLabel>
#include <QMap>
#include <QPixmap>
#include <QSvgRenderer>
#include "Gra.h"

class OknoGry : public QWidget {
    Q_OBJECT

private:
    Gra m_sesja;
    QGraphicsScene* m_scena;
    QGraphicsView* m_widok;
    QTimer* m_zegarKlatek;
    QLabel* m_labelWynik;
    QLabel* m_labelZycia;
    QLabel* m_labelInfo;
    QMap<QString, QPixmap> m_tekstury;
    int m_animTick;

public:
    explicit OknoGry(QWidget *parent = nullptr);
    ~OknoGry();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onKlatkaGry();

private:
    void ladujTekstury();
    QPixmap renderujSVG(const QString& sciezka, int w, int h);
    void renderujGre();
    void rysujMape();
    void rysujElementy();
    void rysujHUD();
    void rysujOverlay();
    QPixmap getTekstura(const QString& nazwa);
};

#endif
