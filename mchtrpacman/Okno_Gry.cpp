#include "Okno_gry.h"
#include "Gracz.h"
#include "wrog.h"
#include "Przedmiot.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QFont>
#include <QSvgRenderer>
#include <QDir>
#include <QCoreApplication>
#include <QDebug>

static const int TILE = 40;
static const int COLS = 19;
static const int ROWS = 19;
static const int VIEW_W = COLS * TILE;
static const int VIEW_H = ROWS * TILE;

OknoGry::OknoGry(QWidget *parent) : QWidget(parent), m_animTick(0) {
    setWindowTitle("Student w Labiryncie");

    // HUD
    m_labelWynik = new QLabel("Wynik: 0", this);
    m_labelZycia = new QLabel("♥♥♥", this);
    m_labelInfo  = new QLabel("", this);

    QFont hudFont("Courier", 14, QFont::Bold);
    m_labelWynik->setFont(hudFont);
    m_labelZycia->setFont(hudFont);
    m_labelInfo->setFont(QFont("Courier", 18, QFont::Bold));
    m_labelInfo->setAlignment(Qt::AlignCenter);
    m_labelInfo->setStyleSheet("color: #FFD700; background: rgba(0,0,0,160); padding: 8px;");

    QHBoxLayout* hudLayout = new QHBoxLayout();
    hudLayout->addWidget(m_labelWynik);
    hudLayout->addStretch();
    hudLayout->addWidget(m_labelZycia);

    m_scena = new QGraphicsScene(this);
    m_widok = new QGraphicsView(m_scena, this);
    m_widok->setFixedSize(VIEW_W + 2, VIEW_H + 2);
    m_scena->setSceneRect(0, 0, VIEW_W, VIEW_H);
    m_widok->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_widok->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_widok->setStyleSheet("border: 2px solid #C98838; background: #1a0a00;");

    QVBoxLayout* main = new QVBoxLayout(this);
    main->addLayout(hudLayout);
    main->addWidget(m_widok);
    main->addWidget(m_labelInfo);
    setLayout(main);

    setStyleSheet("background-color: #1a0a00; color: #FFD700;");
    m_labelWynik->setStyleSheet("color: #FFD700;");
    m_labelZycia->setStyleSheet("color: #FF4444;");

    ladujTekstury();

    m_sesja.inicjalizujPoziom("poziom_1.txt");

    m_zegarKlatek = new QTimer(this);
    connect(m_zegarKlatek, &QTimer::timeout, this, &OknoGry::onKlatkaGry);
    m_zegarKlatek->start(33); // ~30fps

    setFocusPolicy(Qt::StrongFocus);
}

OknoGry::~OknoGry() {}

QPixmap OknoGry::renderujSVG(const QString& sciezka, int w, int h) {
    QSvgRenderer renderer(sciezka);
    if (!renderer.isValid()) {
        qWarning() << "Nie mozna zaladowac SVG:" << sciezka;
        QPixmap px(w, h);
        px.fill(Qt::magenta);
        return px;
    }
    QPixmap px(w, h);
    px.fill(Qt::transparent);
    QPainter p(&px);
    renderer.render(&p);
    return px;
}

void OknoGry::ladujTekstury() {
    // Szukaj SVG w katalogu aplikacji i biezacym
    QStringList dirs = {
        QCoreApplication::applicationDirPath(),
        QDir::currentPath(),
        "."
    };

    auto znajdzSVG = [&](const QString& nazwa) -> QString {
        for (auto& d : dirs) {
            QString p = d + "/" + nazwa;
            if (QFile::exists(p)) return p;
        }
        return "";
    };

    struct { QString klucz; QString plik; } svg[] = {
        {"sciana",       "filarnaroznikscianamchtr.svg"},
        {"okno",         "oknomchtr.svg"},
        {"klimatyzacja", "klimatyzacjamchtr.svg"},
        {"kebsik",       "kebsik.svg"},
        {"studentdebil", "studentdebil.svg"},
        {"skalniak",     "skalniak.svg"},
    };

    for (auto& s : svg) {
        QString path = znajdzSVG(s.plik);
        if (!path.isEmpty()) {
            m_tekstury[s.klucz] = renderujSVG(path, TILE, TILE);
            qDebug() << "Zaladowano:" << s.klucz << "z" << path;
        } else {
            qWarning() << "Brak pliku:" << s.plik;
            QPixmap fallback(TILE, TILE);
            fallback.fill(s.klucz == "sciana" ? QColor(100,70,30) : QColor(50,50,50));
            m_tekstury[s.klucz] = fallback;
        }
    }

    // Przestraszony wrog - przyciemniony skalniak
    QPixmap przestr = m_tekstury["skalniak"].copy();
    QPainter pp(&przestr);
    pp.setCompositionMode(QPainter::CompositionMode_SourceAtop);
    pp.fillRect(0, 0, TILE, TILE, QColor(0, 0, 180, 160));
    m_tekstury["skalniak_przestraszony"] = przestr;

    // Super kebsik - z poswiatą
    QPixmap superK(TILE, TILE);
    superK.fill(Qt::transparent);
    QPainter ps(&superK);
    ps.drawPixmap(0, 0, m_tekstury["kebsik"]);
    ps.setCompositionMode(QPainter::CompositionMode_SourceAtop);
    ps.fillRect(4, 4, TILE-8, TILE-8, QColor(255, 220, 0, 80));
    m_tekstury["kebsik_super"] = superK;
}

QPixmap OknoGry::getTekstura(const QString& nazwa) {
    if (m_tekstury.contains(nazwa)) return m_tekstury[nazwa];
    QPixmap fb(TILE, TILE);
    fb.fill(Qt::darkGray);
    return fb;
}

void OknoGry::keyPressEvent(QKeyEvent *event) {
    StanGry stan = m_sesja.getStan();

    if (stan == StanGry::GAME_OVER || stan == StanGry::WYGRANA) {
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Space || event->key() == Qt::Key_R) {
            m_sesja.restart();
        }
        return;
    }

    switch (event->key()) {
        case Qt::Key_Up:    case Qt::Key_W: m_sesja.ustawKierunkGracza(Kierunek::GORE); break;
        case Qt::Key_Down:  case Qt::Key_S: m_sesja.ustawKierunkGracza(Kierunek::DOL); break;
        case Qt::Key_Left:  case Qt::Key_A: m_sesja.ustawKierunkGracza(Kierunek::LEWO); break;
        case Qt::Key_Right: case Qt::Key_D: m_sesja.ustawKierunkGracza(Kierunek::PRAWO); break;
        default: break;
    }
}

void OknoGry::onKlatkaGry() {
    m_animTick++;
    m_sesja.aktualizujStanGry();
    renderujGre();
}

void OknoGry::renderujGre() {
    m_scena->clear();

    rysujMape();
    rysujElementy();
    rysujHUD();
    rysujOverlay();
}

void OknoGry::rysujMape() {
    const Mapa& mapa = m_sesja.getMapa();
    for (int r = 0; r < mapa.getWysokosc(); r++) {
        for (int k = 0; k < mapa.getSzerokosc(); k++) {
            char z = mapa.getZnak(k, r);
            int px = k * TILE, py = r * TILE;

            QString tex;
            if (z == '#') {
                // Roznicuj teksture: krawedzie = okno, wewnetrzne = sciana
                bool krawedz = (r == 0 || r == mapa.getWysokosc()-1 || k == 0 || k == mapa.getSzerokosc()-1);
                tex = krawedz ? "okno" : "sciana";
            } else {
                // Podloga - ciemny prostokat
                auto* rect = m_scena->addRect(px, py, TILE, TILE, QPen(Qt::NoPen), QBrush(QColor(26, 10, 0)));
                Q_UNUSED(rect);
                continue;
            }
            auto* item = m_scena->addPixmap(getTekstura(tex));
            item->setPos(px, py);
        }
    }
}

void OknoGry::rysujElementy() {
    const auto& elementy = m_sesja.getElementy();

    // Najpierw przedmioty, potem postaci
    // Runda 1: przedmioty
    for (const auto& e : elementy) {
        if (!e->czyAktywny()) continue;
        auto* p = dynamic_cast<Przedmiot*>(e.get());
        if (!p) continue;

        QString typ = QString::fromStdString(e->getTypGrafiki());

        if (typ == "kebsik_super") {
            // Pulsowanie
            double scale = 0.7 + 0.3 * (sin(m_animTick * 0.15) * 0.5 + 0.5);
            QPixmap px = getTekstura("kebsik_super");
            int sw = (int)(TILE * scale), sh = (int)(TILE * scale);
            int ox = (TILE - sw) / 2, oy = (TILE - sh) / 2;
            auto* item = m_scena->addPixmap(px.scaled(sw, sh, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            item->setPos(e->getX() + ox, e->getY() + oy);
        } else {
            // Maly kebsik - 40% rozmiaru, wycentrowany
            int sz = TILE * 2 / 5;
            int off = (TILE - sz) / 2;
            QPixmap px = getTekstura("kebsik").scaled(sz, sz, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            auto* item = m_scena->addPixmap(px);
            item->setPos(e->getX() + off, e->getY() + off);
        }
    }

    // Runda 2: wrogowie
    for (const auto& e : elementy) {
        if (!e->czyAktywny()) continue;
        auto* w = dynamic_cast<Wrog*>(e.get());
        if (!w) continue;

        QString typ = QString::fromStdString(e->getTypGrafiki());
        auto* item = m_scena->addPixmap(getTekstura(typ));
        item->setPos(e->getX(), e->getY());

        // Ramka dla BFS wroga
        if (auto* bfs = dynamic_cast<WrogBFS*>(w)) {
            Q_UNUSED(bfs);
            m_scena->addRect(e->getX()+1, e->getY()+1, TILE-2, TILE-2,
                             QPen(QColor(255,100,0,180), 2), QBrush(Qt::NoBrush));
        }
    }

    // Runda 3: gracz
    for (const auto& e : elementy) {
        if (!e->czyAktywny()) continue;
        auto* g = dynamic_cast<Gracz*>(e.get());
        if (!g) continue;

        QPixmap px = getTekstura("studentdebil");

        // Obrot w zaleznosci od kierunku
        if (g->getKierunek() != Kierunek::BRAK) {
            double angle = 0;
            bool flip = false;
            switch (g->getKierunek()) {
                case Kierunek::PRAWO: flip = true; break;
                case Kierunek::GORE: angle = -90; break;
                case Kierunek::DOL: angle = 90; break;
                default: break;
            }
            if (angle != 0) {
                QTransform t;
                t.rotate(angle);
                px = px.transformed(t, Qt::SmoothTransformation);
                px = px.scaled(TILE, TILE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            }
            if (flip) {
                px = px.transformed(QTransform().scale(-1, 1));
            }
        }

        // Miganie przy smierci
        if (g->czyUmiera() && (m_animTick / 4) % 2 == 0) continue;

        auto* item = m_scena->addPixmap(px);
        item->setPos(e->getX(), e->getY());
        item->setZValue(10);
    }
}

void OknoGry::rysujHUD() {
    m_labelWynik->setText(QString("Wynik: %1").arg(m_sesja.getWynik()));

    QString zycia = "";
    for (int i = 0; i < m_sesja.getZycia(); i++) zycia += "♥";
    m_labelZycia->setText(zycia.isEmpty() ? "✗" : zycia);
}

void OknoGry::rysujOverlay() {
    StanGry stan = m_sesja.getStan();
    QString msg = "";

    if (stan == StanGry::GAME_OVER) {
        msg = "OBLAŁEŚ!\nWynik: " + QString::number(m_sesja.getWynik()) +
              "\n\nNaciśnij R / Enter by spróbować ponownie";
        // Ciemna nakładka
        auto* tlo = m_scena->addRect(0, 0, VIEW_W, VIEW_H,
                                     QPen(Qt::NoPen), QBrush(QColor(0,0,0,160)));
        tlo->setZValue(20);
    } else if (stan == StanGry::WYGRANA) {
        msg = "ZALICZONO!\nWynik: " + QString::number(m_sesja.getWynik()) +
              "\n\nNaciśnij R / Enter by zagrać ponownie";
        auto* tlo = m_scena->addRect(0, 0, VIEW_W, VIEW_H,
                                     QPen(Qt::NoPen), QBrush(QColor(0,30,0,160)));
        tlo->setZValue(20);
    }

    m_labelInfo->setText(msg);
    m_labelInfo->setVisible(!msg.isEmpty());

    if (!msg.isEmpty()) {
        auto* txt = m_scena->addText(msg, QFont("Courier", 20, QFont::Bold));
        txt->setDefaultTextColor(QColor(255,215,0));
        txt->setTextWidth(VIEW_W - 40);
        txt->setPos(20, VIEW_H/2 - 80);
        txt->setZValue(21);
    }
}
