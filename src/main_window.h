#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

    private slots:
        void calculateCRC();

private:
    QLineEdit* frameInput;
    QLineEdit* repetitionsInput;

    QLabel* totalTimeLabel;
    QLabel* iterationTimeLabel;
    QLabel* crcLabel;

    QPushButton* startButton;
};

#endif // MAIN_WINDOW_H