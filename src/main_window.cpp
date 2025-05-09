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
#include <QStatusBar>
#include <QMessageBox>
#include <QIcon>
#include <QTimer>
#include <QClipboard>
#include <QMenu>
#include <QAction>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Kalkulator CRC-16 Modbus RTU - SCR Zadanie 2");
    setMinimumSize(600, 350);
    setWindowIcon(QIcon(":/icons/app_icon.ico"));

    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Informacja o zadaniu
    QLabel* titleLabel = new QLabel("Systemy Czasu Rzeczywistego - Zadanie nr 2");
    QFont titleFont = titleLabel->font();
    titleFont.setBold(true);
    titleFont.setPointSize(titleFont.pointSize() + 2);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    QGroupBox* inputGroupBox = new QGroupBox("Dane wejściowe");
    QFormLayout* inputLayout = new QFormLayout(inputGroupBox);

    frameInput = new QLineEdit();
    frameInput->setPlaceholderText("np. 01 10 00 11 00 03 06 1A C4 BA D0");

    QRegularExpression hexRegex("^[0-9A-Fa-f\\s]*$");
    QRegularExpressionValidator* hexValidator = new QRegularExpressionValidator(hexRegex, this);
    frameInput->setValidator(hexValidator);

    repetitionsInput = new QLineEdit("1");
    QIntValidator* intValidator = new QIntValidator(1, 1000000000, this);
    repetitionsInput->setValidator(intValidator);

    connect(frameInput, &QLineEdit::returnPressed, this, &MainWindow::calculateCRC);
    connect(repetitionsInput, &QLineEdit::returnPressed, this, &MainWindow::calculateCRC);

    inputLayout->addRow("Bajty ramki (max 256):", frameInput);
    inputLayout->addRow("Liczba powtórzeń (1..10^9):", repetitionsInput);

    QGroupBox* resultsGroupBox = new QGroupBox("Wyniki");
    QFormLayout* resultsLayout = new QFormLayout(resultsGroupBox);

    totalTimeLabel = new QLabel("0 ms");
    crcLabel = new QLabel("0000");

    // Utworzenie układu poziomego dla etykiety CRC i przycisku kopiowania
    QHBoxLayout* crcLayout = new QHBoxLayout();
    crcLayout->addWidget(crcLabel, 1); // Daj etykiecie więcej miejsca

    QPushButton* copyButton = new QPushButton(QIcon::fromTheme("edit-copy"), "");
    copyButton->setToolTip("Kopiuj do schowka");
    copyButton->setMaximumWidth(30);
    connect(copyButton, &QPushButton::clicked, this, &MainWindow::copyCRCToClipboard);
    crcLayout->addWidget(copyButton);

    resultsLayout->addRow("Łączny czas realizacji [ms]:", totalTimeLabel);
    resultsLayout->addRow("Wartość CRC-16 [hex]:", crcLayout);

    // Przyciski
    startButton = new QPushButton("OBLICZ CRC");
    startButton->setDefault(true);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::calculateCRC);

    QPushButton* clearButton = new QPushButton("Wyczyść");
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearInputs);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(clearButton);
    buttonLayout->addWidget(startButton);

    // Informacja o realizacji zadania
    QLabel* infoLabel = new QLabel("Realizacja zadania zgodnie ze specyfikacją protokołu MODBUS RTU");
    infoLabel->setAlignment(Qt::AlignCenter);

    // Dodanie wszystkich elementów do głównego układu
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(inputGroupBox);
    mainLayout->addWidget(resultsGroupBox);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(infoLabel);

    // Tworzenie paska statusu
    statusBar()->showMessage("Gotowy do obliczeń CRC");

    setCentralWidget(centralWidget);

    // Menu kontekstowe
    crcLabel->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(crcLabel, &QLabel::customContextMenuRequested, this, &MainWindow::showCRCContextMenu);

    frameInput->setFocus();
}

void MainWindow::calculateCRC() {
    QString frameHexString = frameInput->text().trimmed();

    if (frameHexString.isEmpty()) {
        crcLabel->setText("Błąd: Wprowadź bajty ramki");
        statusBar()->showMessage("Błąd: Wprowadź bajty ramki", 5000);
        return;
    }

    bool ok;
    int repetitions = repetitionsInput->text().toInt(&ok);
    if (!ok || repetitions < 1 || repetitions > 1000000000) {
        crcLabel->setText("Błąd: Nieprawidłowa liczba powtórzeń");
        statusBar()->showMessage("Błąd: Nieprawidłowa liczba powtórzeń (zakres: 1..10^9)", 5000);
        return;
    }

    std::vector<uint8_t> frameBytes = HexParser::parseHexString(frameHexString);
    if (frameBytes.empty()) {
        crcLabel->setText("Błąd: Nieprawidłowy format bajtów");
        statusBar()->showMessage("Błąd: Nieprawidłowy format bajtów", 5000);
        return;
    }

    if (frameBytes.size() > 256) {
        crcLabel->setText("Błąd: Przekroczono limit 256 bajtów");
        statusBar()->showMessage("Błąd: Przekroczono limit 256 bajtów", 5000);
        return;
    }

    startButton->setEnabled(false);
    frameInput->setEnabled(false);
    repetitionsInput->setEnabled(false);

    QApplication::setOverrideCursor(Qt::WaitCursor);
    totalTimeLabel->setText("Obliczanie...");
    crcLabel->setText("Obliczanie...");
    statusBar()->showMessage("Obliczanie sumy kontrolnej CRC-16...");
    QApplication::processEvents();

    auto [totalTimeMs, crcValue] = CRCCalculator::performTimedCalculation(frameBytes, repetitions);

    QString crcString = QString("%1").arg(crcValue, 4, 16, QChar('0')).toUpper();

    totalTimeLabel->setText(QString::number(totalTimeMs) + " ms");
    crcLabel->setText(crcString);

    QString statusMsg;
    if (repetitions == 1) {
        statusMsg = QString("CRC-16: %1 (czas: %2 ms)").arg(crcString).arg(totalTimeMs);
    } else {
        double iterationTime = static_cast<double>(totalTimeMs) / repetitions;
        statusMsg = QString("CRC-16: %1 (czas: %2 ms, %3 ms/iteracja)").arg(crcString)
                           .arg(totalTimeMs)
                           .arg(iterationTime, 0, 'f', 8);
    }
    statusBar()->showMessage(statusMsg, 10000);

    QApplication::restoreOverrideCursor();
    startButton->setEnabled(true);
    frameInput->setEnabled(true);
    repetitionsInput->setEnabled(true);
}

void MainWindow::clearInputs() {
    frameInput->clear();
    repetitionsInput->setText("1");
    totalTimeLabel->setText("0 ms");
    crcLabel->setText("0000");
    statusBar()->showMessage("Wyczyszczono pola", 5000);
    frameInput->setFocus();
}

void MainWindow::copyCRCToClipboard() {
    QString crcText = crcLabel->text();
    if (crcText != "0000" && !crcText.startsWith("Błąd") && !crcText.startsWith("Obliczanie")) {
        QApplication::clipboard()->setText(crcText);
        statusBar()->showMessage("Skopiowano do schowka: " + crcText, 5000);
    }
}

void MainWindow::showCRCContextMenu(const QPoint &pos) {
    QMenu contextMenu(tr("Menu kontekstowe"), this);

    QAction copyAction("Kopiuj wynik", this);
    connect(&copyAction, &QAction::triggered, this, &MainWindow::copyCRCToClipboard);

    QAction infoAction("Informacje o sumie kontrolnej", this);
    connect(&infoAction, &QAction::triggered, this, &MainWindow::showCRCInfo);

    contextMenu.addAction(&copyAction);
    contextMenu.addAction(&infoAction);
    contextMenu.exec(crcLabel->mapToGlobal(pos));
}

void MainWindow::showCRCInfo() {
    QMessageBox::information(this, "Informacje o CRC-16 Modbus",
        "Suma kontrolna CRC-16 Modbus RTU:\n\n"
        "- Wielomian: x^16 + x^15 + x^2 + 1 (0x8005)\n"
        "- Wartość początkowa: 0xFFFF\n"
        "- Kolejność bajtów w wyniku: little-endian\n"
        "- Wartość xorowana na końcu: 0x0000\n\n"
        "Algorytm optymalizowany pod kątem czasu wykonania zgodnie z\n"
        "wymogami zadania nr 2 z przedmiotu Systemy Czasu Rzeczywistego.",
        QMessageBox::Ok);
}