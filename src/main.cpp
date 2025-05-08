#include "main_window.h"
#include <QApplication>

#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char *argv[]) {
    // Set high priority for this process on Windows
#ifdef _WIN32
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
#endif

    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}