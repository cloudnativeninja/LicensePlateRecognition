#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/operations.hpp>
#include <opencv2/core/types_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/core/core_c.h>
#include <opencv2/core/types_c.h>

#include <iostream>

namespace algorithm
{
  void gaussian(cv::Mat& img)
  {
    cv::GaussianBlur(img, img, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
  }

  void grayscale(cv::Mat& img)
  {
    cv::cvtColor(img, img, CV_RGB2GRAY);
  }

  void sobel(cv::Mat& img)
  {
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;


    cv::Sobel(img, grad_x, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT);
    cv::convertScaleAbs(grad_x, abs_grad_x);

    cv::Sobel(img, grad_y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT);
    cv::convertScaleAbs(grad_y, abs_grad_y);

    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, img);
  }
}
