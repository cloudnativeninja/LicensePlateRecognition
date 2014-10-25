#include "exec.hh"
#include "algorithm.hh"
#include "../tools/Tools.h"

#include <map>

namespace algorithm
{
  static std::map<std::string, std::function<void(cv::Mat &)>> funcs =
  {
    { "blur", blurgaussian },
    { "grayscale", grayscale },
    { "sobel", sobel },
    { "otsu", otsu },
    { "swt", removeSmallBlobs },
    { "median", median },
    { "morph", morph },
    { "hist", equalize},
    { "robert", roberts}
    // { "loc", location}

  };

  void precise_loc(cv::Mat &originalimage, std::vector < std::vector<std::pair<int, int>>> regions)
  {
    cv::Mat gray = originalimage;
    grayscale(gray);
    fillzone(gray, regions);
    originalimage = gray;
  }

  void exec_all(std::list<std::string> &as, cv::Mat &img)
  {
    cv::Mat originalimage = img;
    int i = 0;
    for (std::string &a : as)
    {
      funcs[a](img);
      std::cout << i << "-----------Before-------------" << a << std::endl;
      Tools::showImage(img, a);
      std::cout << i << "----------After--------------" << a << std::endl;
      i++;
    }
    std::vector < std::vector<std::pair<int, int>>> regions;
    regions = location(img);
    precise_loc(originalimage, regions);
    Tools::showImage(originalimage, "Grey Coarse Location");
    sobel(originalimage);
    // cv::Canny(originalimage, originalimage, 100, 200, 3);
    Tools::showImage(originalimage, "Sobel");
    erosion(originalimage);
    Tools::showImage(originalimage, "Erosion");
    // dilation(originalimage);
    // Tools::showImage(originalimage, "Dilation");
    morph2(originalimage, 1); // Closing Operation
    Tools::showImage(originalimage, "Closing");
    removeSmallBlobs(originalimage);
    Tools::showImage(originalimage, "bwareopen");
    sumvertical(originalimage);
  }

}

