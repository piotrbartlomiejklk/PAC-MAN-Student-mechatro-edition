#include <QApplication>
#include <QScreen>
#include "Okno_gry.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    OknoGry okno;
    okno.setWindowTitle("Student w Labiryncie — PAC-MAN MECHATRO");

    // Wyśrodkuj okno na ekranie przy starcie
    QScreen* ekran = QApplication::primaryScreen();
    if (ekran) {
        QRect geometriaEkranu = ekran->availableGeometry();
        QSize rozmiarOkna = okno.sizeHint();
        int x = (geometriaEkranu.width() - rozmiarOkna.width()) / 2;
        int y = (geometriaEkranu.height() - rozmiarOkna.height()) / 2;
        okno.move(geometriaEkranu.left() + x, geometriaEkranu.top() + y);
    }

    okno.show();
    return app.exec();
}
