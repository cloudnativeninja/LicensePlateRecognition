#ifndef ALGORITHM_HH_
# define ALGORITHM_HH_

#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>


namespace algorithm
{
  int blurgaussian(cv::Mat &img);
  int grayscale(cv::Mat &img);
  int sobel(cv::Mat &img);
  int otsu(cv::Mat &img);
  int removeSmallBlobs(cv::Mat &img);
  int median(cv::Mat &img);
  int morph(cv::Mat &img);
  int equalize(cv::Mat &img);
  std::vector < std::vector<std::pair<int, int>>>
  location(cv::Mat &img);
  int roberts(cv::Mat &img);
  int detect(cv::Mat &img);
  int reduce_noize(cv::Mat &img);
  std::string character_segmentation(cv::Mat &img);
  int fillzone(cv::Mat &img, std::vector < std::vector<std::pair<int, int>>> regions);
  int erosion(cv::Mat &src);
  int dilation(cv::Mat &img);
  int morph2(cv::Mat &img, int oprtor);
  int open(cv::Mat &img);
  std::vector < std::pair<int, int>> sumvertical(cv::Mat &img);
}

#endif /* !ALGORITHM_HH_ */
