#ifndef OKNO_GRY_H
#define OKNO_GRY_H
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QKeyEvent>
#include <set>
#include "Gra.h"

using namespace std;

class OknoGry : public QWidget {
    Q_OBJECT

private:
    Gra m_sesja;
    QGraphicsScene* m_scena;
    QGraphicsView* m_widok;
    QTimer* m_zegarKlatek;
    set<int> m_wcisnieteKlawisze;

public:
    explicit OknoGry(QWidget *parent = nullptr);
    ~OknoGry();

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private slots:
    void onKlatkaGry();

private:
    void renderujGre();
};


#endif // OKNO_GRY_H
