#ifndef ALGORITHM_HH_
# define ALGORITHM_HH_

#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>


namespace algorithm
{
  void edge_detect(cv::Mat &img);
  void gaussian(cv::Mat &img);
  void grayscale(cv::Mat &img);
  void sobel(cv::Mat &img);
  void binarize(cv::Mat &img);
  void swt(cv::Mat &img);
  void median(cv::Mat &img);

}

#endif /* !ALGORITHM_HH_ */
