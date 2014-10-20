#include "Filter.hh"

Filter::Filter() {}

Filter::~Filter() {}

void
Filter::sobelFilter(const cv::Mat &src, cv::Mat &dst)
{
  for (int y = 0; y < src.rows; y++)
  {
    for (int x = 0; x < src.cols; x++)
    {
      dst.at<uchar>(y, x) = 0.0;
    }
  }

  for (int y = 1; y < src.rows - 1; y++)
  {
    for (int x = 1; x < src.cols - 1; x++)
    {
      int gx = Tools::convolutionXSobel(src, x, y);
      int gy = Tools::convolutionYSobel(src, x, y);
      int sum = abs(gx) + abs(gy);
      sum = sum > 255 ? 255 : sum;
      sum = sum < 0 ? 0 : sum;
      dst.at<uchar>(y, x) = sum;
    }
  }
}

void
Filter::horizontalEdgeDetection(const cv::Mat &src, cv::Mat &dst)
{
  for (int y = 0; y < src.rows; y++)
  {
    for (int x = 0; x < src.cols; x++)
      dst.at<uchar>(y, x) = 0.0;
  }

  for (int y = 1; y < src.rows - 1; y++)
  {
    for (int x = 1; x < src.cols - 1; x++)
    {
      int h = Tools::convolutionXDetection(src, x, y);
      int sum = abs(h);
      sum = sum > 255 ? 255 : sum;
      sum = sum < 0 ? 0 : sum;
      dst.at<uchar>(y, x) = sum;
    }
  }
}

void
Filter::verticalEdgeDetection(const cv::Mat &src, cv::Mat &dst)
{
  for (int y = 0; y < src.rows; y++)
  {
    for (int x = 0; x < src.cols; x++)
      dst.at<uchar>(y, x) = 0.0;
  }

  for (int y = 1; y < src.rows - 1; y++)
  {
    for (int x = 1; x < src.cols - 1; x++)
    {
      int v = Tools::convolutionYDetection(src, x, y);
      int sum = abs(v);
      sum = sum > 255 ? 255 : sum;
      sum = sum < 0 ? 0 : sum;
      dst.at<uchar>(y, x) = sum;
    }
  }
}

void
Filter::medianFilter(const cv::Mat &src, cv::Mat &dst)
{
  int window[9];

  dst = src.clone();
  for (int y = 0; y < src.rows; y++)
  {
    for (int x = 0; x < src.cols; x++)
    {
      dst.at<uchar>(y, x) = 0.0;
    }
  }

  for (int y = 1; y < src.rows - 1; y++)
  {
    for (int x = 1; x < src.cols - 1; x++)
    {
      window[0] = src.at<uchar>(y - 1, x - 1);
      window[1] = src.at<uchar>(y, x - 1);
      window[2] = src.at<uchar>(y + 1, x - 1);
      window[3] = src.at<uchar>(y - 1, x);
      window[4] = src.at<uchar>(y, x);
      window[5] = src.at<uchar>(y + 1, x);
      window[6] = src.at<uchar>(y - 1, x + 1);
      window[7] = src.at<uchar>(y, x + 1);
      window[8] = src.at<uchar>(y + 1, x + 1);

      Tools::insertionSort(window);
      dst.at<uchar>(y, x) = window[4];
    }
  }
}

void
Filter::otsuFilter(const cv::Mat &src, cv::Mat &dst, int threshold)
{
  for (int y = 0; y < src.rows; ++y)
  {
    for (int x = 0; x < src.cols; ++x)
    {
      dst.at<uchar>(y, x) = src.at<uchar>(y, x) < threshold ? 0 : 255;
    }
  }
}

void
Filter::grayScaleFilter(const cv::Mat &src, cv::Mat &dst)
{
  for (int y = 0; y < src.rows; ++y)
  {
    for (int x = 0; x < src.cols; ++x)
    {
      int blue = src.at<cv::Vec3b>(y, x)[0];
      int green = src.at<cv::Vec3b>(y, x)[1];
      int red = src.at<cv::Vec3b>(y, x)[2];
      int gray = (red * 299 + green * 587 + blue * 114) / 1000;
      dst.at<uchar>(y, x) = gray;
    }
  }
}
