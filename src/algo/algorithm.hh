#ifndef ALGORITHM_HH_
# define ALGORITHM_HH_

#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>


namespace algorithm
{
  void blurgaussian(cv::Mat &img);
  void grayscale(cv::Mat &img);
  void sobel(cv::Mat &img);
  void otsu(cv::Mat &img);
  void removeSmallBlobs(cv::Mat &img);
  void median(cv::Mat &img);
  void morph(cv::Mat &img);
  void equalize(cv::Mat &img);
  std::vector < std::vector<std::pair<int, int>>>
  location(cv::Mat &img);
  void roberts(cv::Mat &img);
  void swt(cv::Mat &img);
  void fillzone(cv::Mat &img, std::vector < std::vector<std::pair<int, int>>> regions);
  void erosion(cv::Mat &src);
  void dilation(cv::Mat &img);
  void morph2(cv::Mat &img, int oprtor);
  void open(cv::Mat &img);
  std::vector < std::pair<int, int>> sumvertical(cv::Mat &img);
}

#endif /* !ALGORITHM_HH_ */
