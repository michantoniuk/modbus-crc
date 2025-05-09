#include "main_window.h"
#include <QApplication>
#include <QIcon>
#include <QLocale>
#include <QTranslator>

#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char *argv[]) {
#ifdef _WIN32
    // Ustawienie priorytetu procesu na wysoki dla lepszej wydajności testów
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
#endif

    // Inicjalizacja aplikacji
    QApplication app(argc, argv);

    // Ustawienie metadanych aplikacji
    app.setApplicationName("ModbusCrc");
    app.setApplicationDisplayName("Kalkulator CRC-16 Modbus");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Projekt Studencki");

    // Ustawienie lokalizacji na polską
    QLocale::setDefault(QLocale(QLocale::Polish, QLocale::Poland));
    QLocale locale = QLocale::system();

    // Ustawienie ikony aplikacji z zasobów
    app.setWindowIcon(QIcon(":/icons/app_icon.ico"));

    // Utworzenie i wyświetlenie głównego okna
    MainWindow window;
    window.show();

    // Uruchomienie pętli zdarzeń aplikacji
    return app.exec();
}