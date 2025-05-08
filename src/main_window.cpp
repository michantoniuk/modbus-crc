#include "main_window.h"
#include "crc_calculator.h"
#include "hex_parser.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QApplication>
#include <QIntValidator>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Modbus CRC-16 Calculator");
    setMinimumSize(600, 300);
    
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    
    // Input section
    QGroupBox* inputGroupBox = new QGroupBox("Dane wejściowe");
    QFormLayout* inputLayout = new QFormLayout(inputGroupBox);
    
    frameInput = new QLineEdit();
    frameInput->setPlaceholderText("np. 01 10 00 11 00 03 06 1A C4 BA D0");
    
    // Create validator for hex input
    QRegularExpression hexRegex("^[0-9A-Fa-f\\s]*$");
    QRegularExpressionValidator* hexValidator = new QRegularExpressionValidator(hexRegex, this);
    frameInput->setValidator(hexValidator);
    
    repetitionsInput = new QLineEdit("1");
    QIntValidator* intValidator = new QIntValidator(1, 1000000000, this);
    repetitionsInput->setValidator(intValidator);
    
    // Connect Enter key in input fields to start button
    connect(frameInput, &QLineEdit::returnPressed, this, &MainWindow::calculateCRC);
    connect(repetitionsInput, &QLineEdit::returnPressed, this, &MainWindow::calculateCRC);
    
    inputLayout->addRow("Bajty ramki:", frameInput);
    inputLayout->addRow("Liczba powtórzeń:", repetitionsInput);
    
    // Results section
    QGroupBox* resultsGroupBox = new QGroupBox("Informacje");
    QFormLayout* resultsLayout = new QFormLayout(resultsGroupBox);
    
    totalTimeLabel = new QLabel("0 ms");
    iterationTimeLabel = new QLabel("0.00000000 ms");
    crcLabel = new QLabel("0000");
    
    resultsLayout->addRow("Czas całkowity (ms):", totalTimeLabel);
    resultsLayout->addRow("Czas 1 iteracji (ms):", iterationTimeLabel);
    resultsLayout->addRow("Suma kontrolna:", crcLabel);
    
    // Start button
    startButton = new QPushButton("START");
    connect(startButton, &QPushButton::clicked, this, &MainWindow::calculateCRC);
    
    // Add widgets to main layout
    mainLayout->addWidget(inputGroupBox);
    mainLayout->addWidget(resultsGroupBox);
    mainLayout->addWidget(startButton);
    
    setCentralWidget(centralWidget);
    
    // Initial focus
    frameInput->setFocus();
}

void MainWindow::calculateCRC() {
    QString frameHexString = frameInput->text().trimmed();
    
    if (frameHexString.isEmpty()) {
        crcLabel->setText("Błąd: Wprowadź bajty ramki");
        return;
    }
    
    bool ok;
    int repetitions = repetitionsInput->text().toInt(&ok);
    if (!ok || repetitions < 1) {
        crcLabel->setText("Błąd: Nieprawidłowa liczba powtórzeń");
        return;
    }
    
    std::vector<uint8_t> frameBytes = HexParser::parseHexString(frameHexString);
    if (frameBytes.empty()) {
        crcLabel->setText("Błąd: Nieprawidłowy format bajtów");
        return;
    }
    
    if (frameBytes.size() > 256) {
        crcLabel->setText("Błąd: Przekroczono limit 256 bajtów");
        return;
    }
    
    // Disable UI during calculation
    startButton->setEnabled(false);
    frameInput->setEnabled(false);
    repetitionsInput->setEnabled(false);
    
    // Update UI to show processing state
    QApplication::setOverrideCursor(Qt::WaitCursor);
    totalTimeLabel->setText("Obliczanie...");
    iterationTimeLabel->setText("Obliczanie...");
    crcLabel->setText("Obliczanie...");
    QApplication::processEvents();
    
    // Use multi-threaded processing for large repetition counts
    auto [totalTimeMs, crcValue] = CRCCalculator::performTimedCalculation(frameBytes, repetitions);
    
    // Format CRC as hex string (uppercase)
    QString crcString = QString("%1").arg(crcValue, 0, 16).toUpper();
    
    // Calculate time per iteration
    double iterationTime = static_cast<double>(totalTimeMs) / repetitions;
    
    // Update UI
    totalTimeLabel->setText(QString::number(totalTimeMs) + " ms");
    iterationTimeLabel->setText(QString::number(iterationTime, 'f', 8) + " ms");
    crcLabel->setText(crcString);
    
    // Re-enable UI
    QApplication::restoreOverrideCursor();
    startButton->setEnabled(true);
    frameInput->setEnabled(true);
    repetitionsInput->setEnabled(true);
}