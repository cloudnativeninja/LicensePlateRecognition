#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../main/all.h"

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
    void on_action_Open_triggered();

    void on_detect_button_clicked();

private:
    cv::Mat car_img;
    Ui::MainWindow *ui;
    int img_treated;
};

#endif // MAINWINDOW_H
