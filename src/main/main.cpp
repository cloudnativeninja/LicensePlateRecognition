#include "all.h"
#include "cmd-parse.hh"
#include "../algo/exec.hh"

int main(int ac, char **av)
{
  options opts;
  cv::Mat lImage;

  parse::cmdline(ac, av, opts);
  lImage = cv::imread(opts["img"].front(), CV_LOAD_IMAGE_UNCHANGED);
  Tools::showImage(lImage, "Original Image");
  algorithm::exec_all(opts["algorithms"], lImage);
  return 0;
}
