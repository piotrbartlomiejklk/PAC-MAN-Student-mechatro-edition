#include "Okno_gry.h"
#include <QKeyEvent>

// Konstruktor
OknoGry::OknoGry(QWidget *parent) : QWidget(parent) {
    // Tutaj w przyszłości zainicjalizujemy m_scena, m_widok i m_zegarKlatek
}

// Destruktor
OknoGry::~OknoGry() {
    // Tutaj w przyszłości zwolnimy pamięć
}

// Rejestrowanie wciśnięcia klawisza
void OknoGry::keyPressEvent(QKeyEvent *event) {
    // Dodajemy kod wciśniętego klawisza do naszego zbioru
    m_wcisnieteKlawisze.insert(event->key());
}

// Rejestrowanie puszczenia klawisza
void OknoGry::keyReleaseEvent(QKeyEvent *event) {
    // Usuwamy kod puszczonego klawisza ze zbioru
    m_wcisnieteKlawisze.erase(event->key());
}

// Główna pętla gry (wywoływana przez timer)
void OknoGry::onKlatkaGry() {
    // Tutaj będziemy sprawdzać m_wcisnieteKlawisze, ruszać graczami
    // i odświeżać logikę m_sesja
}

// Rysowanie grafiki
void OknoGry::renderujGre() {
    // Tutaj będziemy układać tekstury na QGraphicsScene
}
