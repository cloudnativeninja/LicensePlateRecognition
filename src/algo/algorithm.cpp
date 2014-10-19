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

#include "line.hh"

namespace algorithm
{
  void blurgaussian(cv::Mat &img)
  {
    cv::GaussianBlur(img, img, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
  }

  void grayscale(cv::Mat &img)
  {
    cv::cvtColor(img, img, CV_RGB2GRAY);
    cv::equalizeHist(img, img);
  }

  // Morphological Transforms
  void morph(cv::Mat &img)
  {
    int morph_elem = 0; //Element:\n 0: Rect - 1: Cross - 2: Ellipse
    int morph_size = 21; // Kernel size:\n 2n +1 (max 21)
    int morph_operator = 3; //Operator:\n 0: Opening - 1: Closing \n 2: Gradient - 3: Top Hat \n 4: Black Hat

    int operation = morph_operator + 2;
    cv::Mat element = getStructuringElement(morph_elem,
                                            cv::Size(2 * morph_size + 1, 2 * morph_size + 1),
                                            cv::Point(morph_size, morph_size));
    cv::morphologyEx(img, img, operation, element);
  }


  void otsu(cv::Mat &img)
  {
    cv::threshold(img, img, 128, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
  }

  void median(cv::Mat &img)
  {
    cv::medianBlur(img, img, 3);
  }

  void detect(cv::Mat &img)
  {
    std::cout << "begin detect" << std::endl;

    cv::threshold(img, img, 97, 1, 0);
    cv::Mat difference(img.size(), 0);
    std::cout << "before for 1" << std::endl;

    for (int y = 0; y < img.cols; y++)
    {
      for (int x = 0; x < img.rows; x++)
        difference.at<uchar>(x, y) = 0.0;
    }
    std::cout << "before for 2" << std::endl;

    for (int y = 0; y < img.cols; y++)
    {
      for (int x = 0; x < img.rows; x++)
        difference.at<uchar>(x, y) = std::abs(img.at<uchar>(x, y) - img.at<uchar>(x, y + 1));
    }

    std::cout << "before for 3" << std::endl;

    std::vector<int> sum;
    for (int x = 0; x < img.rows; x++)
    {
      int sumline = 0;
      for (int y = 0; y < img.cols; y++)
      {
        sumline += difference.at<uchar>(x, y);
      }
      sum.push_back(sumline);
    }
    std::cout << "before for 4" << std::endl;

    for (auto lol : sum)
      std::cout << lol << std::endl;

    std::cout << "end detect" << std::endl;

  }


  void swt(cv::Mat &img)
  {
    // stroke width transform
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
    detect(img);
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

  static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
  {
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10);
  }

  void edge_detect(cv::Mat &img)
  {
    cv::RNG rng(12345);

    std::vector<std::vector<cv::Point> > contours;
    //    cv::Canny(img, img, 100, 200, 3);
    cv::findContours(img.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    // The array for storing the approximation curve
    std::vector<cv::Point> approx;

    // We'll put the labels in this destination image
    cv::Mat dst = img.clone();

    for (size_t i = 0; i < contours.size(); i++)
    {
      // Approximate contour with accuracy proportional
      // to the contour perimeter
      cv::approxPolyDP(
        cv::Mat(contours[i]),
        approx,
        cv::arcLength(cv::Mat(contours[i]), true) * 0.05,
        true
      );
      /*      cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
      cv::drawContours( dst, contours, i, color, 2, 8, CV_RETR_EXTERNAL, 0, cv::Point() );
      */
      // Skip small or non-convex objects
      if (approx.size() == 4)
      {
        // Number of vertices of polygonal curve
        int vtc = approx.size();

        // Get the degree (in cosines) of all corners
        std::vector<double> cos;
        for (int j = 2; j < vtc + 1; j++)
          cos.push_back(angle(approx[j % vtc], approx[j - 2], approx[j - 1]));

        // Sort ascending the corner degree values
        std::sort(cos.begin(), cos.end());

        // Get the lowest and the highest degree
        double mincos = cos.front();
        double maxcos = cos.back();

        // Use the degrees obtained above and the number of vertices
        // to determine the shape of the contour
        if (vtc == 4 && mincos >= -0.1 && maxcos <= 0.3)
        {
          // Detect rectangle or square
          cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255) );
          cv::drawContours( dst, contours, i, color, 2, 8, CV_RETR_EXTERNAL, 0, cv::Point() );
        }
      }
    } // end of for() loop
    /*
    std::vector<cv::Vec2f> vecs;
    std::vector<line> lines;
    std::vector<std::pair<line, line>> parallels;
    cv::Mat dst = img.clone();

    //    cv::Canny(img.clone(), dst, 50, 200, 3);

    cv::HoughLines(dst, vecs, 1, CV_PI / 180, 100, 0, 0);

    for (size_t i = 0; i < vecs.size(); i++)
      lines.push_back(line(vecs[i]));

    //    for (line& l : lines)
    //l.draw(dst);

    for (size_t i = 0; i < lines.size(); i++)
      for (size_t j = 0; j < lines.size(); j++)
    if (i != j && lines[i].is_horizontal() && lines[j].is_horizontal()
      && lines[i].is_parallel(lines[j]))
    parallels.push_back(std::pair<line, line>(lines[i], lines[j]));

    std::cout << parallels.size() << std::endl;
    for (size_t i = 0; i < parallels.size(); i++)
    {
      parallels[i].first.draw(dst);
      parallels[i].second.draw(dst);
    }
    */
    img = dst;
  }
}
