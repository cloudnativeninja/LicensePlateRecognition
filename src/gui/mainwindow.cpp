#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include "../main/all.h"
#include <QtSql>

#define q2c(string) string.toStdString()
#define q2b(string) string.toUtf8().constData()

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

  currentFileName = fileName;

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
  QString dbPath = QCoreApplication::applicationDirPath() + "/../db/images.db";
  std::cout << q2c(dbPath) << std::endl;
  db.setDatabaseName(dbPath);
  if (db.open())
  {
    std::cout << "You are now connected to " << q2c(db.hostName()) << " database " << std::endl;
    id++;
    QFile file(currentFileName);
    if (!file.open(QIODevice::ReadOnly)) return;
    QByteArray byteArray = file.readAll();

    QSqlQuery query;
    QString id_string = id == -1 ? "NULL" : QString::number (id);
    query.prepare("REPLACE INTO imagetable (id, imagepath, imagedata) VALUES (:id, :imagepath, :imagedata)");
    query.bindValue (":id", id_string);
    query.bindValue (":imagepath", currentFileName);
    query.bindValue (":imagedata", QString (byteArray.toBase64()));
    if (!query.exec())
      std::cout << "Fail to add image " << q2c(currentFileName) << " to database of host " << q2c(db.hostName()) << std::endl;
    else
      std::cout << "Adding image " << q2c(currentFileName) << " to database of host " << q2c(db.hostName()) << std::endl;



    db.close();
  }
  else
  {
    std::cout << "Connexion failed, sorry. " << std::endl << q2c(db.lastError().text()) << std::endl;;
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

void MainWindow::on_actionLoad_Database_triggered()
{
  QSqlDatabase db;
  db = QSqlDatabase::addDatabase("QSQLITE");
  QString dbPath = QCoreApplication::applicationDirPath() + "/../db/images.db";
  std::cout << q2c(dbPath) << std::endl;
  db.setDatabaseName(dbPath);
  if (db.open())
  {
    std::cout << "You are now connected to " << q2c(db.hostName()) << " database " << std::endl;

    QSqlQuery query;
    QString id_string = id == -1 ? "NULL" : QString::number (id);

    query.prepare("SELECT id, imagepath, imagedata FROM imagetable WHERE id=1;");
    if (!query.exec())
    {
      qDebug() << query.lastError();
      std::cout << " Fail to load image " << q2c(currentFileName) << " to database of host " << q2c(db.hostName()) << std::endl;
    }
    else   std::cout << "Loading image " << q2c(currentFileName) << " to database of host " <<
                       q2c(db.hostName()) << std::endl;

    query.next();
    int iid = query.value(0).toInt();
    currentFileName = query.value(1).toString();
    QByteArray array = QByteArray::fromBase64(query.value(2).toByteArray());
    // Creating a QPixmap from QByteArray :
    std::cout << query.isSelect() << std::endl;
    QPixmap pixmap = QPixmap();
    pixmap.loadFromData(array);
    ui->image_label->setPixmap(pixmap);

  }


}
