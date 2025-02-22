#include <QApplication>
#include "ContactManagerGUI.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    ContactManagerGUI window;
    window.show();
    return app.exec();
}
