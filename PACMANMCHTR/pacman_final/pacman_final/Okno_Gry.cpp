#include "Okno_gry.h"
#include "Gracz.h"
#include "wrog.h"
#include "Przedmiot.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QSvgRenderer>
#include <QDir>
#include <QCoreApplication>
#include <QFile>
#include <QFont>
#include <QDebug>
#include <QGraphicsPixmapItem>
static const int TILE   = 40;           // rozmiar jednego kafelka w pikselach
static const int COLS   = 19;           // kolumny mapy
static const int ROWS   = 19;           // wiersze mapy
static const int VIEW_W = COLS * TILE;  // szerokość obszaru gry
static const int VIEW_H = ROWS * TILE;  // wysokość obszaru gry
static const int PANEL  = 140;          // szerokość bocznych paneli "budynku"

// ============================================================
// Konstruktor — buduje cały interfejs ręcznie (zamiast pliku .ui).
// Układ: [panel-L] [widok gry] [panel-R] z HUD-em na górze.
// ============================================================
OknoGry::OknoGry(QWidget* parent)
    : QWidget(parent), m_stan(StanEkranu::WYBOR_POZIOMU),
      m_wyborKursor(0), m_animTick(0)
{
    setWindowTitle("Student w Labiryncie — PAC-MAN MECHATRO");
    setStyleSheet("background: #0d0600;");

    ladujTekstury();

    // --- HUD: wynik i życia ---
    m_labelWynik = new QLabel("Wynik: 0", this);
    m_labelZycia = new QLabel("♥♥♥",      this);
    m_labelInfo  = new QLabel("",          this);

    QFont fHUD("Courier", 13, QFont::Bold);
    m_labelWynik->setFont(fHUD);
    m_labelZycia->setFont(fHUD);
    m_labelInfo->setFont(QFont("Courier", 12, QFont::Bold));
    m_labelWynik->setStyleSheet("color:#FFD700;");
    m_labelZycia->setStyleSheet("color:#FF4444;");
    m_labelInfo->setStyleSheet("color:#FFD700; background:#0d0d0d; border-top:2px solid #C98838; padding:4px;");
    m_labelInfo->setAlignment(Qt::AlignCenter);

    // Pasek HUD
    QWidget* hudBar = new QWidget(this);
    hudBar->setFixedHeight(44);
    hudBar->setStyleSheet("background:#0d0d0d; border-bottom:2px solid #C98838;");
    QLabel* tytul = new QLabel("🎓 STUDENT W LABIRYNCIE", hudBar);
    tytul->setFont(QFont("Courier", 12, QFont::Bold));
    tytul->setStyleSheet("color:#FFD700;");
    QHBoxLayout* hudLay = new QHBoxLayout(hudBar);
    hudLay->addWidget(tytul);
    hudLay->addStretch();
    hudLay->addWidget(m_labelWynik);
    hudLay->addSpacing(20);
    hudLay->addWidget(m_labelZycia);

    // --- Scena gry ---
    m_scena = new QGraphicsScene(this);
    m_scena->setSceneRect(0, 0, VIEW_W, VIEW_H);
    m_widok = new QGraphicsView(m_scena, this);
    m_widok->setFixedSize(VIEW_W + 2, VIEW_H + 2);
    m_widok->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_widok->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_widok->setStyleSheet("border:2px solid #C98838; background:#1a0a00;");

    // Boczne panele "budynku" — narysowane w rysujTloBudynku()
    QLabel* panelL = new QLabel(this);
    QLabel* panelR = new QLabel(this);
    panelL->setFixedWidth(PANEL);
    panelR->setFixedWidth(PANEL);
    panelL->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    panelR->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    // Narysuj tło budynku (robimy to tutaj raz, bo panel ma stały rozmiar)
    auto rysujPanel = [&](QLabel* panel, bool lustro) {
        QPixmap buf(PANEL, VIEW_H + 44);
        buf.fill(QColor(15, 6, 0));
        QPainter p(&buf);
        // Cegły
        for (int y = 0; y < buf.height(); y += 20) {
            int off = (y/20 % 2) * 30;
            for (int x = -off; x < PANEL; x += 60)
                p.fillRect(x+1, y+1, 58, 18, QColor(55, 28, 8));
            p.fillRect(0, y, PANEL, 1, QColor(30, 12, 2));
        }
        // Tekstury SVG wzdłuż panelu
        QPixmap texOkno = getTekstura("okno");
        QPixmap texKlim = getTekstura("klimatyzacja");
        QPixmap texSc   = getTekstura("sciana").scaled(20, TILE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        // Filar przy krawędzi przy mapie
        int fX = lustro ? 0 : PANEL - 20;
        for (int y = 0; y < buf.height(); y += TILE)
            p.drawPixmap(fX, y, texSc);
        // Okna i klimatyzacja naprzemiennie
        int elX = lustro ? 28 : 28;
        for (int y = TILE; y < buf.height() - TILE; y += TILE * 2) {
            bool parzyste = (y / TILE) % 2 == 0;
            p.drawPixmap(elX, y, parzyste ? texOkno : texKlim);
        }
        // Napis na ścianie
        p.setFont(QFont("Courier", 7, QFont::Bold));
        p.setPen(QColor(80, 45, 15));
        p.save(); p.translate(PANEL/2, buf.height()/2); p.rotate(-90);
        p.drawText(-50, 0, "WYDZIAŁ MECHATRO");
        p.restore();
        p.end();
        panel->setPixmap(buf);
    };
    rysujPanel(panelL, false);
    rysujPanel(panelR, true);

    // Środkowy rząd
    QHBoxLayout* midLay = new QHBoxLayout();
    midLay->setContentsMargins(0,0,0,0);
    midLay->setSpacing(0);
    midLay->addWidget(panelL);
    midLay->addWidget(m_widok, 0, Qt::AlignCenter);
    midLay->addWidget(panelR);

    // Główny layout okna
    QVBoxLayout* mainLay = new QVBoxLayout(this);
    mainLay->setContentsMargins(0,0,0,0);
    mainLay->setSpacing(0);
    mainLay->addWidget(hudBar);
    mainLay->addLayout(midLay, 1);
    mainLay->addWidget(m_labelInfo);
    setLayout(mainLay);

    // --- Panel wyboru poziomu (nakładka na widok gry) ---
    m_panelWyboru = new QWidget(m_widok);
    m_panelWyboru->setGeometry(0, 0, VIEW_W + 2, VIEW_H + 2);
    m_panelWyboru->setStyleSheet("background: rgba(0,0,0,210);");

    m_wyborTytul  = new QLabel("🎓 STUDENT W LABIRYNCIE 🎓", m_panelWyboru);
    m_wyborOpcja1 = new QLabel("  [1]  Poziom 1 — Klasyczny", m_panelWyboru);
    m_wyborOpcja2 = new QLabel("  [2]  Poziom 2 — Trudny",    m_panelWyboru);
    QLabel* hint  = new QLabel("↑↓ wybór   •   Enter / 1 / 2 — start\n\nSterowanie: W A S D", m_panelWyboru);

    m_wyborTytul->setFont(QFont("Courier", 18, QFont::Bold));
    m_wyborTytul->setStyleSheet("color:#FFD700;");
    m_wyborTytul->setAlignment(Qt::AlignCenter);
    m_wyborOpcja1->setFont(QFont("Courier", 14, QFont::Bold));
    m_wyborOpcja2->setFont(QFont("Courier", 14, QFont::Bold));
    hint->setFont(QFont("Courier", 10));
    hint->setStyleSheet("color:#777; padding:6px;");
    hint->setAlignment(Qt::AlignCenter);

    QVBoxLayout* wLay = new QVBoxLayout(m_panelWyboru);
    wLay->setAlignment(Qt::AlignCenter);
    wLay->setSpacing(18);
    wLay->addStretch();
    wLay->addWidget(m_wyborTytul);
    wLay->addSpacing(20);
    wLay->addWidget(m_wyborOpcja1);
    wLay->addWidget(m_wyborOpcja2);
    wLay->addSpacing(16);
    wLay->addWidget(hint);
    wLay->addStretch();

    aktualizujPodswietlenieWyboru();
    pokazEkranWyboru();

    // Timer gry — co 33ms (~30fps)
    m_zegar = new QTimer(this);
    connect(m_zegar, &QTimer::timeout, this, &OknoGry::onKlatka);
    m_zegar->start(33);

    setMinimumSize(VIEW_W + 2*PANEL + 4, VIEW_H + 44 + 36);
    setFocusPolicy(Qt::StrongFocus);
}

// ============================================================
// Ładowanie tekstur SVG → QPixmap (wykonywane raz przy starcie)
// Szukamy pliku w katalogu obok .exe i w bieżącym katalogu.
// ============================================================
QPixmap OknoGry::renderujSVG(const QString& sciezka) {
    QSvgRenderer r(sciezka);
    if (!r.isValid()) {
        QPixmap px(TILE, TILE); px.fill(Qt::magenta); return px;
    }
    QPixmap px(TILE, TILE);
    px.fill(Qt::transparent);
    QPainter p(&px);
    r.render(&p);
    return px;
}

void OknoGry::ladujTekstury() {
    // Szukaj SVG obok .exe lub w bieżącym katalogu
    auto znajdz = [](const QString& plik) -> QString {
        for (const QString& dir : { QCoreApplication::applicationDirPath(), QDir::currentPath(), QString(".") }) {
            QString p = dir + "/" + plik;
            if (QFile::exists(p)) return p;
        }
        return {};
    };

    // Tabela: klucz słownika → nazwa pliku SVG
    struct { QString klucz, plik; } lista[] = {
        {"sciana",       "filarnaroznikscianamchtr.svg"},
        {"okno",         "oknomchtr.svg"},
        {"klimatyzacja", "klimatyzacjamchtr.svg"},
        {"kebsik",       "kebsik.svg"},
        {"studentdebil", "studentdebil.svg"},
        {"skalniak",     "skalniak.svg"},
    };
    for (auto& s : lista) {
        QString path = znajdz(s.plik);
        if (!path.isEmpty())
            m_tekstury[s.klucz] = renderujSVG(path);
        else {
            qWarning() << "Brak SVG:" << s.plik;
            QPixmap fb(TILE, TILE);
            fb.fill(QColor(80,40,10));
            m_tekstury[s.klucz] = fb;
        }
    }

    // Warianty generowane programowo (nakładki kolorów):

    // Przestraszony wróg — niebieska nakładka na skalniaku
    QPixmap przestr = m_tekstury["skalniak"].copy();
    QPainter pp(&przestr);
    pp.setCompositionMode(QPainter::CompositionMode_SourceAtop);
    pp.fillRect(0, 0, TILE, TILE, QColor(0, 0, 180, 160));
    m_tekstury["skalniak_przestraszony"] = przestr;

    // Super kebsik — żółta poświata
    QPixmap superK(TILE, TILE);
    superK.fill(Qt::transparent);
    QPainter ps(&superK);
    ps.drawPixmap(0, 0, m_tekstury["kebsik"]);
    ps.setCompositionMode(QPainter::CompositionMode_SourceAtop);
    ps.fillRect(4, 4, TILE-8, TILE-8, QColor(255, 220, 0, 70));
    m_tekstury["kebsik_super"] = superK;
}

QPixmap OknoGry::getTekstura(const QString& nazwa) {
    return m_tekstury.value(nazwa, QPixmap());
}

// ============================================================
// Ekran wyboru poziomu
// ============================================================
void OknoGry::pokazEkranWyboru() {
    m_stan = StanEkranu::WYBOR_POZIOMU;
    m_panelWyboru->show();
    m_panelWyboru->raise();
    m_labelInfo->setText("Wybierz poziom klawiszem 1 lub 2");
}

void OknoGry::aktualizujPodswietlenieWyboru() {
    auto styl = [](bool aktywny, const QString& kolor) {
        return aktywny
            ? QString("color:%1; background:rgba(255,255,0,20); padding:8px; border-radius:4px;").arg(kolor)
            : QString("color:#555; padding:8px;");
    };
    m_wyborOpcja1->setStyleSheet(styl(m_wyborKursor == 0, "#FFD700"));
    m_wyborOpcja2->setStyleSheet(styl(m_wyborKursor == 1, "#FF8844"));
}

void OknoGry::rozpocznijPoziom(int nr) {
    m_panelWyboru->hide();
    m_stan = StanEkranu::GRAMY;
    m_labelInfo->setText("");
    QString plik = (nr == 2) ? "poziom_2.txt" : "poziom_1.txt";
    m_sesja.inicjalizujPoziom(plik.toStdString());
}

// ============================================================
// Obsługa klawiatury
// ============================================================
void OknoGry::keyPressEvent(QKeyEvent* event) {
    // --- Ekran wyboru poziomu ---
    if (m_stan == StanEkranu::WYBOR_POZIOMU) {
        if (event->key() == Qt::Key_Up   || event->key() == Qt::Key_W)
            { m_wyborKursor = 0; aktualizujPodswietlenieWyboru(); }
        else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_S)
            { m_wyborKursor = 1; aktualizujPodswietlenieWyboru(); }
        else if (event->key() == Qt::Key_1) rozpocznijPoziom(1);
        else if (event->key() == Qt::Key_2) rozpocznijPoziom(2);
        else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Space)
            rozpocznijPoziom(m_wyborKursor + 1);
        return;
    }

    // --- Po zakończeniu gry ---
    StanGry stan = m_sesja.getStan();
    if (stan == StanGry::GAME_OVER || stan == StanGry::WYGRANA) {
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_R)
            m_sesja.restart();                  // zagraj ponownie ten sam poziom
        else if (event->key() == Qt::Key_M)
            pokazEkranWyboru();                 // wróć do wyboru poziomu
        return;
    }

    // --- Sterowanie: WASD ---
    switch (event->key()) {
        case Qt::Key_W: m_sesja.ustawKierunkGracza(Kierunek::GORE);  break;
        case Qt::Key_S: m_sesja.ustawKierunkGracza(Kierunek::DOL);   break;
        case Qt::Key_A: m_sesja.ustawKierunkGracza(Kierunek::LEWO);  break;
        case Qt::Key_D: m_sesja.ustawKierunkGracza(Kierunek::PRAWO); break;
        default: break;
    }
}

void OknoGry::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    centrujWidok();
    if (m_panelWyboru)
        m_panelWyboru->setGeometry(0, 0, m_widok->width(), m_widok->height());
}

void OknoGry::centrujWidok() { update(); }

// ============================================================
// Pętla gry — wywoływana przez timer co ~33ms
// ============================================================
void OknoGry::onKlatka() {
    if (m_stan == StanEkranu::WYBOR_POZIOMU) return;
    m_animTick++;
    m_sesja.aktualizujStanGry();
    renderujGre();
}

// ============================================================
// Renderowanie — buduje scenę Qt co klatkę
// ============================================================
void OknoGry::renderujGre() {
    m_scena->clear();
    rysujMape();
    rysujElementy(); // polimorficzne przez getTypGrafiki()
    rysujHUD();
    rysujOverlay();
}

void OknoGry::rysujMape() {
    const Mapa& mapa = m_sesja.getMapa();
    for (int r = 0; r < mapa.getWysokosc(); r++) {
        for (int k = 0; k < mapa.getSzerokosc(); k++) {
            int px = k * TILE, py = r * TILE;
            char z = mapa.getZnak(k, r);
            if (z == '#') {
                // Krawędzie mapy = okna budynku, narożniki = klimatyzacja, środek = cegła
                bool kraw  = (r==0 || r==mapa.getWysokosc()-1 || k==0 || k==mapa.getSzerokosc()-1);
                bool narozn = (r==0||r==mapa.getWysokosc()-1) && (k==0||k==mapa.getSzerokosc()-1);
                QString tex = narozn ? "klimatyzacja" : (kraw ? "okno" : "sciana");
                m_scena->addPixmap(getTekstura(tex))->setPos(px, py);
            } else {
                // Podłoga korytarza
                m_scena->addRect(px, py, TILE, TILE, QPen(Qt::NoPen), QBrush(QColor(26,10,0)));
            }
        }
    }
}

// Rysuje wszystkie aktywne elementy gry.
// POLIMORFIZM: e->getTypGrafiki() zwraca inną wartość dla każdego typu
// (Przedmiot/Wrog/Gracz) — OknoGry nie sprawdza typu if-ami.
// Wyjątek: dynamic_cast jest używany gdy potrzebujemy cech specyficznych
// dla podklasy (obracanie gracza, ramka BFS, miganie śmierci).
void OknoGry::rysujElementy() {
    const auto& elementy = m_sesja.getElementy();

    // Kolejność rysowania: Przedmioty → Wrogowie → Gracz (gracz na wierzchu)

    // --- Przedmioty ---
    for (const auto& e : elementy) {
        if (!e->czyAktywny()) continue;
        if (!dynamic_cast<Przedmiot*>(e.get())) continue;

        // POLIMORFIZM: getTypGrafiki() zwraca "kebsik" lub "kebsik_super"
        QString typ = QString::fromStdString(e->getTypGrafiki());

        // Super kebsik — pełny rozmiar, z poświatą (bez pulsowania)
        // Zwykły kebsik — zmniejszony do 40%, wycentrowany
        if (typ == "kebsik_super") {
            m_scena->addPixmap(getTekstura("kebsik_super"))->setPos(e->getX(), e->getY());
        } else {
            int sz = TILE * 2 / 5, off = (TILE - sz) / 2;
            m_scena->addPixmap(
                getTekstura("kebsik").scaled(sz, sz, Qt::KeepAspectRatio, Qt::SmoothTransformation)
            )->setPos(e->getX() + off, e->getY() + off);
        }
    }

    // --- Wrogowie ---
    for (const auto& e : elementy) {
        if (!e->czyAktywny()) continue;
        auto* w = dynamic_cast<Wrog*>(e.get());
        if (!w) continue;

        // POLIMORFIZM: "skalniak" lub "skalniak_przestraszony"
        m_scena->addPixmap(getTekstura(
            QString::fromStdString(e->getTypGrafiki()))
        )->setPos(e->getX(), e->getY());

        // Pomarańczowa ramka dla WrogBFS — odróżnia go od losowego
        if (dynamic_cast<WrogBFS*>(w))
            m_scena->addRect(e->getX()+1, e->getY()+1, TILE-2, TILE-2,
                             QPen(QColor(255,120,0,200), 2), QBrush(Qt::NoBrush));
    }

    // --- Gracz ---
    for (const auto& e : elementy) {
        if (!e->czyAktywny()) continue;
        auto* g = dynamic_cast<Gracz*>(e.get());
        if (!g) continue;

        // Miganie podczas animacji śmierci (co 4 klatki gracz znika)
        if (g->czyUmiera() && (m_animTick / 4) % 2 == 0) continue;

        // POLIMORFIZM: "studentdebil"
        QPixmap px = getTekstura(QString::fromStdString(g->getTypGrafiki()));

        // Obróć teksturę w kierunku ruchu
        switch (g->getKierunek()) {
            case Kierunek::PRAWO:
                px = px.transformed(QTransform().scale(-1, 1)); break;
            case Kierunek::GORE:
                px = px.transformed(QTransform().rotate(-90))
                       .scaled(TILE, TILE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation); break;
            case Kierunek::DOL:
                px = px.transformed(QTransform().rotate(90))
                       .scaled(TILE, TILE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation); break;
            default: break;
        }

        auto* item = m_scena->addPixmap(px);
        item->setPos(e->getX(), e->getY());
        item->setZValue(10); // gracz zawsze na wierzchu
    }
}

void OknoGry::rysujHUD() {
    m_labelWynik->setText(QString("Wynik: %1").arg(m_sesja.getWynik()));
    QString serca;
    for (int i = 0; i < m_sesja.getZycia(); i++) serca += "♥";
    m_labelZycia->setText(serca.isEmpty() ? "✗" : serca);
}

void OknoGry::rysujOverlay() {
    StanGry stan = m_sesja.getStan();
    if (stan == StanGry::GRAJ) { m_labelInfo->setText(""); return; }

    // Przyciemnij planszę
    auto* tlo = m_scena->addRect(0, 0, VIEW_W, VIEW_H,
        QPen(Qt::NoPen), QBrush(stan == StanGry::GAME_OVER
            ? QColor(0,0,0,170) : QColor(0,30,0,170)));
    tlo->setZValue(20);

    QString msg = (stan == StanGry::GAME_OVER)
        ? QString("OBLAŁEŚ!\nWynik: %1\n\nR / Enter = ponów   M = wybór poziomu")
                  .arg(m_sesja.getWynik())
        : QString("ZALICZONO! 🎉\nWynik: %1\n\nR / Enter = ponów   M = wybór poziomu")
                  .arg(m_sesja.getWynik());

    auto* txt = m_scena->addText(msg, QFont("Courier", 17, QFont::Bold));
    txt->setDefaultTextColor(QColor(255, 215, 0));
    txt->setTextWidth(VIEW_W - 60);
    txt->setPos(30, VIEW_H / 2 - 80);
    txt->setZValue(21);

    m_labelInfo->setText(stan == StanGry::GAME_OVER ? "GAME OVER" : "WYGRANA!");
}

void OknoGry::rysujTloBudynku() {}
