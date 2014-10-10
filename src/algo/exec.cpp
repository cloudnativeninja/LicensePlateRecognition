#include "exec.hh"
#include "algorithm.hh"
#include <map>

namespace algorithm
{
  static std::map<std::string, std::function<void(cv::Mat&)>> funcs =
  {
    { "gaussian", gaussian }
    , { "grayscale", grayscale }
    , { "sobel", sobel }
  };

  void exec_all(std::list<std::string>& as, cv::Mat& img)
  {
    for (std::string& a : as)
      funcs[a](img);
  }
}

