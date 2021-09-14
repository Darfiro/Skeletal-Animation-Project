#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QDir>
#include <QCloseEvent>
#include <QMessageBox>
#include <QFile>
#include "scene.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnInput_clicked();

    void on_chSkelet_stateChanged(int arg1);

    void on_chBody_stateChanged(int arg1);

    void on_btnAdd_clicked();

    void on_btnView_clicked();

    void on_cmbFrames_currentIndexChanged(int index);

    void on_btnDeleteCurr_clicked();

    void on_btnDeleteAll_clicked();

    void deleteFiles(QString name, const char *filename);

    void on_btnPlay_clicked();

    void on_Duration_valueChanged(int value);

    void on_btnStop_clicked();

    void on_Speed_valueChanged(int value);

    void show_Message(QString message);

    void on_pushButton_clicked();

    void saveResult(QString name, const char *filename);

    void on_btnSafe_clicked();

    void shut_Down(QCloseEvent *event);

    void on_btnShowExample_clicked();

private:
    Ui::MainWindow *ui;
    Scene *scene;
    int justAdded;
    bool isAnimated;
    bool showChanged;
};

#endif // MAINWINDOW_H
