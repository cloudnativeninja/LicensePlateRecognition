#include "all.h"
#include "cmd-parse.hh"
#include "../algo/exec.hh"
#include "../gui/mainwindow.h"
#include <QApplication>

int main(int ac, char **av)
{
  options opts;
  cv::Mat lImage;

  parse::cmdline(ac, av, opts);
  if (!opts["gui"].size())
  {
    lImage = cv::imread(opts["img"].front(), CV_LOAD_IMAGE_UNCHANGED);
    Tools::showImage(lImage, "Original Image");
    algorithm::exec_all(opts["algorithms"], lImage);
    return 0;
  }
  else
  {
    QApplication a(ac, av);
    MainWindow w;
    w.show();

    return a.exec();
  }
}
