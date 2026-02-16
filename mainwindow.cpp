#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QFontDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->statusbar->showMessage("Ready");

    connect(ui->plainTextEdit, &QPlainTextEdit::cursorPositionChanged,
            this, &MainWindow::updateCursorPosition);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered() {
    bool ok;
    QString name = QInputDialog::getText(this, "New Document",
                                         "Enter file name:", QLineEdit::Normal,
                                         "untitled.txt", &ok);
    if (ok && !name.isEmpty()) {
        ui->plainTextEdit->clear();
        ui->statusbar->showMessage("Created: " + name, 3000);
    }
}

void MainWindow::on_actionExit_triggered() {
    auto result = QMessageBox::question(this, "Exit", "Do you want to close QuickPad?",
                                        QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes) {
        this->close();
    }
}

void MainWindow::on_actionFont_triggered() {
    bool ok;
    QFont font = QFontDialog::getFont(&ok, ui->plainTextEdit->font(), this);
    if (ok) {
        ui->plainTextEdit->setFont(font);
    }
}

void MainWindow::on_actionWordWrap_triggered() {
    if (ui->actionWordWrap->isChecked()) {
        ui->plainTextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    } else {
        ui->plainTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
    }
}

void MainWindow::on_actionAbout_triggered() {
    QMessageBox::about(this, "About QuickPad",
                       "QuickPad v1.0\nPractical Work #4\nDeveloped with Qt 6.");
}

void MainWindow::updateCursorPosition() {
    int line = ui->plainTextEdit->textCursor().blockNumber() + 1;
    int col = ui->plainTextEdit->textCursor().columnNumber() + 1;
    ui->statusbar->showMessage(QString("Line %1, Col %2").arg(line).arg(col));
}

