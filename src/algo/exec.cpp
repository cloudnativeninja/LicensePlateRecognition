#include "exec.hh"
#include "algorithm.hh"
#include "../tools/Tools.h"

#include <map>

namespace algorithm
{
  static std::map<std::string, std::function<void(cv::Mat &)>> funcs =
  {
    { "gaussian", gaussian },
    { "grayscale", grayscale },
    { "sobel", sobel },
    { "binarize", binarize },
    { "swt", swt },
    { "edge_detect", edge_detect}
  };

  void exec_all(std::list<std::string> &as, cv::Mat &img)
  {
    for (std::string &a : as)
      {
        funcs[a](img);
        Tools::showImage(img, a);
      }

  }
}

