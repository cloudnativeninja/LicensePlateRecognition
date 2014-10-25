#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include "../main/all.h"
#include <QtSql>

#define q2c(string) string.toStdString()
#define q2b(string) string.toString()

#include "../algo/algorithm.hh"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  img_treated = 0;
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_action_Open_triggered()
{
  QString fileName = QFileDialog::getOpenFileName(this,
                     tr("Open File"), QDir::currentPath());

  if (!fileName.isEmpty())
  {
    car_img = cv::imread(fileName.toUtf8().constData(), CV_LOAD_IMAGE_UNCHANGED);

    ui->image_label->setPixmap(QPixmap::fromImage(Tools::Mat2QImage(car_img)));
    img_treated = 0;
    ui->located_lp->clear();
    ui->plate_number_txt->clear();


  }
}


void MainWindow::on_actionSave_triggered()
{
  QSqlDatabase db;
  db = QSqlDatabase::addDatabase("QSQLITE");
  db.setHostName("localhost");
  db.setUserName("root");
  db.setPassword("");
  db.setDatabaseName("images");

  if (db.open())
  {
    std::cout << "Vous êtes maintenant connecté à " << q2c(db.hostName()) << std::endl;


    QSqlQuery query;
    if (query.exec("SELECT * FROM image"))
    {
      while (query.next())
      {
        std::cout << "    Nouvelle entrée" << std::endl;
        for (int x = 0; x < query.record().count(); ++x)
        {
          // std::cout << "        " << q2c(query.record().fieldName(x)) << " = " << query.value(x) << std::endl;
        }
      }
    }
    db.close();
  }
  else
  {
    std::cout << "La connexion a échouée, désolé" << std::endl << q2c(db.lastError().text()) << std::endl;;
  }

}

void MainWindow::on_detect_button_clicked()
{
  if (img_treated)
    return;
  img_treated = 1;
  if (car_img.rows <= 0 || car_img.cols <= 0)
    return;

  algorithm::grayscale(car_img);
  if (algorithm::detect(car_img))
  {
    ui->error_label->setText("Could not find licence plate");
    return;
  }
  algorithm::reduce_noize(car_img);
  ui->located_lp->setPixmap(QPixmap::fromImage(Tools::Mat2QImage(car_img)));
  std::string plate_no = algorithm::character_segmentation(car_img);
  if (plate_no.size() < 6)
  {
    ui->error_label->setText("Could not find licence plate");
    return;
  }
  ui->plate_number_txt->setText(QString::fromUtf8(plate_no.c_str()));
}
