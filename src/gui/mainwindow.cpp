#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include "../main/all.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Open_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                         tr("Open File"), QDir::currentPath());

    if (!fileName.isEmpty()) {
        cv::Mat img = cv::imread(fileName.toUtf8().constData(), CV_LOAD_IMAGE_UNCHANGED);

        ui->imageLabel->setPixmap(QPixmap::fromImage(Tools::Mat2QImage(img)));
    }
}
