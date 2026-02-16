#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    bool on_actionSave_triggered();
    bool on_actionSaveAs_triggered();
    void on_actionExit_triggered();

    void on_actionFind_triggered();
    void on_actionFont_triggered();
    void on_actionWordWrap_triggered();
    void on_actionPreferences_triggered();
    void on_actionHelp_triggered();
    void on_actionAbout_triggered();
    void on_actionDarkTheme_triggered();
    void on_actionLightTheme_triggered();

    void updateCursorPosition();

private:
    Ui::MainWindow *ui;
    QString curFile;
    bool isModified;

    bool maybeSave();
    void loadFile(const QString &fileName);
    void saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
};
#endif // MAINWINDOW_H
