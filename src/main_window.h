#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

// Main application window for the Modbus CRC-16 Calculator
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

    private slots:
        // Slot for calculating CRC when Start button is pressed or Enter key is hit
        void calculateCRC();

private:
    // Input fields
    QLineEdit* frameInput;
    QLineEdit* repetitionsInput;

    // Output labels
    QLabel* totalTimeLabel;
    QLabel* iterationTimeLabel;
    QLabel* crcLabel;

    // Start button
    QPushButton* startButton;
};

#endif // MAIN_WINDOW_H