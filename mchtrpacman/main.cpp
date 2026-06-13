#include <QApplication>
#include "Okno_gry.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    OknoGry okno;
    okno.setWindowTitle("Student w Labiryncie — PAC-MAN MECHATRO");
    okno.show();

    return app.exec();
}
