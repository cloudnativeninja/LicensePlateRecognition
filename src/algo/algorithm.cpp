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
#include <vector>

namespace algorithm
{
  void gaussian(cv::Mat &img)
  {
    cv::GaussianBlur(img, img, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
  }

  void grayscale(cv::Mat &img)
  {
    cv::cvtColor(img, img, CV_RGB2GRAY);
  }

  void binarize(cv::Mat &img)
  {
    // cv::cvtColor(img, img, CV_RGB2GRAY);
    cv::threshold(img, img, 97, 255, 0);
  }

  void swt(cv::Mat &img)
  {
    // bw8u : we want to calculate the SWT of this. NOTE: Its background pixels are 0 and forground pixels are 1 (not 255!)
    cv::Mat bw32f, swt32f, kernel;
    double  max;
    int strokeRadius;
    cv::threshold(img, img, 97, 1, 0);
    img.convertTo(bw32f, CV_32F);  // format conversion for multiplication
    distanceTransform(img, swt32f, CV_DIST_L2, 5); // distance transform
    minMaxLoc(swt32f, NULL, &max);  // find max
    strokeRadius = (int)ceil(max);  // half the max stroke width
    kernel = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)); // 3x3 kernel used to select 8-connected neighbors

    for (int j = 0; j < strokeRadius; j++)
      {
        dilate(swt32f, swt32f, kernel); // assign the max in 3x3 neighborhood to each center pixel
        swt32f = swt32f.mul(bw32f); // apply mask to restore original shape and to avoid unnecessary max propogation
      }
    // swt32f : resulting SWT image
    img = swt32f;
  }


  void sobel(cv::Mat &img)
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
