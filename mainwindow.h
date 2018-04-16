#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <nzerointerface.h>
#include <QJsonDocument>

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

    void on_btnSend_clicked();

    void OnZeroRcv(QString msg);

private:
    Ui::MainWindow *ui;
    NZeroInterface zero;
};

#endif // MAINWINDOW_H
