#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QFontDialog>
#include <QInputDialog>
#include <QColorDialog>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), isModified(false)
{
    ui->setupUi(this);
    ui->plainTextEdit->setFocus();

    connect(ui->actionUndo, &QAction::triggered, ui->plainTextEdit, &QPlainTextEdit::undo);
    connect(ui->actionRedo, &QAction::triggered, ui->plainTextEdit, &QPlainTextEdit::redo);
    connect(ui->actionCut, &QAction::triggered, ui->plainTextEdit, &QPlainTextEdit::cut);
    connect(ui->actionCopy, &QAction::triggered, ui->plainTextEdit, &QPlainTextEdit::copy);
    connect(ui->actionPaste, &QAction::triggered, ui->plainTextEdit, &QPlainTextEdit::paste);
    connect(ui->actionSelectAll, &QAction::triggered, ui->plainTextEdit, &QPlainTextEdit::selectAll);

    connect(ui->plainTextEdit, &QPlainTextEdit::textChanged, this, [this](){ isModified = true; });
    connect(ui->plainTextEdit, &QPlainTextEdit::cursorPositionChanged, this, &MainWindow::updateCursorPosition);

    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    connect(ui->plainTextEdit, &QPlainTextEdit::copyAvailable, ui->actionCut, &QAction::setEnabled);
    connect(ui->plainTextEdit, &QPlainTextEdit::copyAvailable, ui->actionCopy, &QAction::setEnabled);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_actionFind_triggered() {
    bool ok;
    QString word = QInputDialog::getText(this, "Find", "Search word:", QLineEdit::Normal, "", &ok);
    if (ok && !word.isEmpty()) {
        if (!ui->plainTextEdit->find(word)) {
            ui->statusbar->showMessage("Not found. Restarting from top...", 2000);
            ui->plainTextEdit->moveCursor(QTextCursor::Start);
            ui->plainTextEdit->find(word);
        }
    }
}

void MainWindow::on_actionPreferences_triggered() {
    QColor color = QColorDialog::getColor(Qt::white, this, "Select Background Color");
    if (color.isValid()) {
        QPalette pal = ui->plainTextEdit->palette();
        pal.setColor(QPalette::Base, color);
        ui->plainTextEdit->setPalette(pal);
    }
}


void MainWindow::on_actionDarkTheme_triggered() {
    this->setStyleSheet(
        "QMainWindow { background-color: #2b2b2b; color: white; }"
        "QPlainTextEdit { background-color: #1e1e1e; color: #abb2bf; selection-background-color: #404859; }"
        "QMenuBar { background-color: #333333; color: white; }"
        "QMenuBar::item:selected { background-color: #444444; }"
        "QStatusBar { background-color: #333333; color: white; }"
        "QToolBar { background-color: #333333; border: none; }"
        );
    ui->statusbar->showMessage("Dark Theme Enabled", 2000);
}

void MainWindow::on_actionLightTheme_triggered() {

    this->setStyleSheet("");
    ui->statusbar->showMessage("Light Theme Enabled", 2000);
}

void MainWindow::on_actionHelp_triggered() {
    // Виклик діалогу довідки
    QMessageBox::information(this, "QuickPad Help",
                             "QuickPad Shortcuts:\n"
                             "F1 - Show this help\n"
                             "Ctrl+F - Find text\n"
                             "Ctrl+N - New document\n"
                             "Ctrl+S - Save file");
}

void MainWindow::on_actionAbout_triggered() {
    QMessageBox::about(this, "About QuickPad",
                       "QuickPad v2.0\n"
                       "Developed for Practical Work #5\n"
                       "Using Qt 6.5"
                       "QuickPad Shortcuts:\n"
                       "F1 - Show this help\n"
                       "Ctrl+F - Find text\n"
                       "Ctrl+N - New document\n"
                       "Ctrl+S - Save file");
}

void MainWindow::on_actionNew_triggered() {
    if (maybeSave()) { ui->plainTextEdit->clear(); setCurrentFile(""); }
}

void MainWindow::on_actionOpen_triggered() {
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this, "Open File");
        if (!fileName.isEmpty()) loadFile(fileName);
    }
}

bool MainWindow::on_actionSave_triggered() {
    if (curFile.isEmpty()) return on_actionSaveAs_triggered();
    saveFile(curFile);
    return true;
}

bool MainWindow::on_actionSaveAs_triggered() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save As");
    if (fileName.isEmpty()) return false;
    saveFile(fileName);
    return true;
}

void MainWindow::loadFile(const QString &fileName) {
    QFile file(fileName);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        ui->plainTextEdit->setPlainText(QTextStream(&file).readAll());
        setCurrentFile(fileName);
    }
}

void MainWindow::saveFile(const QString &fileName) {
    QFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream(&file) << ui->plainTextEdit->toPlainText();
        setCurrentFile(fileName);
    }
}

bool MainWindow::maybeSave() {
    if (!isModified) return true;
    auto ret = QMessageBox::warning(this, "QuickPad", "Save changes?", QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (ret == QMessageBox::Save) return on_actionSave_triggered();
    return (ret != QMessageBox::Cancel);
}

void MainWindow::setCurrentFile(const QString &fileName) {
    curFile = fileName;
    isModified = false;
    setWindowTitle((curFile.isEmpty() ? "untitled" : curFile) + " - QuickPad");
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (maybeSave()) event->accept(); else event->ignore();
}

void MainWindow::on_actionExit_triggered() { this->close(); }

void MainWindow::on_actionWordWrap_triggered() {
    ui->plainTextEdit->setLineWrapMode(ui->actionWordWrap->isChecked() ? QPlainTextEdit::WidgetWidth : QPlainTextEdit::NoWrap);
}

void MainWindow::on_actionFont_triggered() {
    bool ok;
    QFont font = QFontDialog::getFont(&ok, ui->plainTextEdit->font(), this);
    if (ok) ui->plainTextEdit->setFont(font);
}

void MainWindow::updateCursorPosition() {
    int line = ui->plainTextEdit->textCursor().blockNumber() + 1;
    int col = ui->plainTextEdit->textCursor().columnNumber() + 1;
    ui->statusbar->showMessage(QString("Line %1, Col %2").arg(line).arg(col));
}
