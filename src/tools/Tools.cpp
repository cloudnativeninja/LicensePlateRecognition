#include "Tools.h"

Tools::Tools() {}

Tools::~Tools() {}

void
Tools::showImage(cv::Mat iImage)
{
  cv::namedWindow("Display Image", CV_WINDOW_AUTOSIZE);
  cv::imshow("Display Image", iImage);
  cv::waitKey(0);
}

void
Tools::showImage(cv::Mat iImage, std::string iName)
{
  cv::namedWindow(iName, CV_WINDOW_AUTOSIZE);
  cv::imshow(iName, iImage);
  cv::waitKey(0);
}
