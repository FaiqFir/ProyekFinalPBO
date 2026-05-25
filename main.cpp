#include <QApplication>
#include "views/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Style global aplikasi
    app.setStyle("Fusion");
    app.setStyleSheet(
        "QWidget { font-family: 'Segoe UI', Arial, sans-serif; }"
        "QScrollBar:vertical {"
        "  border:none; background:#F1F5F9; width:8px; border-radius:4px; }"
        "QScrollBar::handle:vertical {"
        "  background:#CBD5E1; border-radius:4px; min-height:24px; }"
        "QScrollBar::handle:vertical:hover { background:#94A3B8; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height:0; }"
    );

    MainWindow w;
    w.show();

    return app.exec();
}
