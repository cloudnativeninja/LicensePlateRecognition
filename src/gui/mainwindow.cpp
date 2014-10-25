#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include "../main/all.h"
#include <stdio.h>
#include <sqlite3.h>
#include <QtSql>

#define q2c(string) string.toStdString()
#define q2b(string) string.toString()


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

  if (!fileName.isEmpty())
  {
    cv::Mat img = cv::imread(fileName.toUtf8().constData(), CV_LOAD_IMAGE_UNCHANGED);

    ui->image_label->setPixmap(QPixmap::fromImage(Tools::Mat2QImage(img)));
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

  // sqlite3 *db;
  // int rc;
  // rc = sqlite3_open("../db/images.db", &db);
  // char *zErrMsg = 0;
  // char *sql;
  // const char *data = "Callback function called";

  // if ( rc )
  // {
  //   fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
  //   exit(0);
  // }
  // else
  // {
  //   fprintf(stderr, "Opened database successfully\n");
  // }

  // sql = "SELECT * FROM image;";

  // rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);

  // if ( rc != SQLITE_OK )
  // {
  //   fprintf(stderr, "SQL error: %s\n", zErrMsg);
  //   sqlite3_free(zErrMsg);
  // }
  // else
  // {
  //   fprintf(stdout, "Operation done successfully\n");
  // }

  // sqlite3_close(db);
}
