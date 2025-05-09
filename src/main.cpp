#include "main_window.h"
#include <QApplication>
#include <QIcon>

#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char *argv[]) {
#ifdef _WIN32
    // Set process priority to high for better benchmark performance
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
#endif

    // Initialize application
    QApplication app(argc, argv);

    // Set application metadata
    app.setApplicationName("ModbusCrc");
    app.setApplicationDisplayName("Kalkulator CRC-16 Modbus");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("ModbusCrc");

    // Set application icon
    app.setWindowIcon(QIcon(":/icons/app_icon.svg"));

    // Create and show main window
    MainWindow window;
    window.show();

    // Run application event loop
    return app.exec();
}