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
#include <baseapi.h>
#include "../tools/Tools.h"
#include "line.hh"
#include "../filter/Filter.hh"

namespace algorithm
{
  void blurgaussian(cv::Mat &img)
  {
    cv::GaussianBlur(img, img, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
  }

  void grayscale(cv::Mat &img)
  {
    cv::cvtColor(img, img, CV_BGR2GRAY);
  }

  void equalize(cv::Mat &img)
  {
    cv::equalizeHist(img, img);
  }

  void morph(cv::Mat &img)
  {
    img = img;
  }


  void open(cv::Mat &img)
  {
    int morph_elem = 0; //Element:\n 0: Rect - 1: Cross - 2: Ellipse
    int morph_size = 3; // Kernel size:\n 2n +1 (max 21)
    int morph_operator = 0; //Operator:\n 0: Opening - 1: Closing \n 2: Gradient - 3: Top Hat \n 4: Black Hat

    int operation = morph_operator + 2;
    cv::Mat element = getStructuringElement(morph_elem,
                                            cv::Size(2 * morph_size + 1, 2 * morph_size + 1),
                                            cv::Point(morph_size, morph_size));
    // cv::Mat element = (cv::Mat_<uchar>(1, 2) << 25, 25);

    cv::morphologyEx(img, img, operation, element);
  }


  // Morphological Transforms
  void morph2(cv::Mat &img, int oprtor)
  {
    // int morph_elem = 0; //Element:\n 0: Rect - 1: Cross - 2: Ellipse
    // int morph_size = 3; // Kernel size:\n 2n +1 (max 21)
    int morph_operator = oprtor; //Operator:\n 0: Opening - 1: Closing \n 2: Gradient - 3: Top Hat \n 4: Black Hat

    int operation = morph_operator + 2;
    // cv::Mat element = getStructuringElement(morph_elem,
    //                                         cv::Size(2 * morph_size + 1, 2 * morph_size + 1),
    //                                         cv::Point(morph_size, morph_size));
    cv::Mat element = (cv::Mat_<uchar>(1, 2) << 25, 25);

    cv::morphologyEx(img, img, operation, element);
  }


  void otsu(cv::Mat &img)
  {
    // cv::adaptiveThreshold(img, img, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 2);
    // cv::adaptiveThreshold(img, img, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 11, 2);
    // cv::adaptiveThreshold(img, img, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 11, 2);

    cv::threshold(img, img, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
  }

  void median(cv::Mat &img)
  {
    cv::medianBlur(img, img, 3);
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

  void preProcessingPlate(const cv::Mat src, cv::Mat &dst)
  {
    cv::Mat med = src;
    median(med);
    sobel(med);
    otsu(med);
    dst = med;
  }


  std::vector<std::pair<unsigned int, unsigned int>>
      gatherProjection(std::vector<std::pair<unsigned int, unsigned int>> iVector)
  {
    std::vector<std::pair<unsigned int, unsigned int>> lGatheredVector;
    int lEpsilon = 2;
    for (unsigned int i = 0; i < iVector.size() - 1; ++i)
    {
      int lBornInf1 = iVector[i].first;
      int lBornSup1 = iVector[i].second;
      int lBornInf2 = iVector[i + 1].first;
      int lBornSup2 = iVector[i + 1].second;
      if (lBornInf2 - lBornSup1 <= lEpsilon)
      {
        lGatheredVector.push_back(std::pair<unsigned int, unsigned int>(lBornInf1, lBornSup2));
      }
      else
      {
        lGatheredVector.push_back(std::pair<unsigned int, unsigned int>(lBornInf1, lBornSup1));
        if (i == iVector.size() - 2)
        {
          lGatheredVector.push_back(std::pair<unsigned int, unsigned int>(lBornInf2, lBornSup2));
        }
      }
    }

    if (iVector.size() == 1)
    {
      return iVector;
    }

    return lGatheredVector;
  }

  std::vector<std::pair<unsigned int, unsigned int>>
      applyThresholding(std::vector<int> projection, int threshold)
  {
    std::vector<std::pair<unsigned int, unsigned int>> lLimites;
    bool lIsContinuous = false;
    unsigned int lBornInf = 0;
    unsigned int lBornSup = 0;
    for (unsigned int i = 0; i < projection.size(); ++i)
    {
      if (projection[i] > threshold)
      {
        if (!lIsContinuous)
        {
          lBornInf = i;
          lIsContinuous = true;
          lBornSup = lBornInf;
        }
        if (i == lBornSup + 1)
        {
          lIsContinuous = true;
          lBornSup = i;
        }
      }
      else if (lIsContinuous)
      {
        lIsContinuous = false;
        lLimites.push_back(std::pair<unsigned int, unsigned int> (lBornInf, lBornSup));
      }
    }
    if (lLimites.empty())
      return lLimites;

    std::vector<std::pair<unsigned int, unsigned int>> gatheredBornes;
    gatheredBornes = gatherProjection(lLimites);
    for (unsigned int i = 1; i < lLimites.size(); ++i)
    {
      gatheredBornes = gatherProjection(gatheredBornes);
    }
    return gatheredBornes;
  }


  void
  selectBySegmentation(cv::Mat iImage, std::vector<std::pair<int, int>> &iBands, std::vector<std::pair<int, int>> &iPlates)
  {
    std::vector<std::pair<int, int>> lGoodBands;
    std::vector<std::pair<int, int>> lGoodPlates;
    for (unsigned int i = 0; i < iBands.size(); ++i)
    {
      std::pair<int, int> lBand = iBands[i];
      std::pair<int, int> lPlate = iPlates[i];
      int lWidth = lPlate.second - lPlate.first;
      int lHeight = lBand.second - lBand.first;

      cv::Mat lSubImage = cv::Mat(iImage, cv::Rect(lPlate.first, lBand.first, lWidth, lHeight));
      preProcessingPlate(lSubImage, lSubImage);
      int lThreshold = 68 * lSubImage.rows;
      std::vector<int> lXProjection = Tools::horizontalProjection(lSubImage);
      std::vector<std::pair<unsigned int, unsigned int>> lSegmentedProjection = applyThresholding(lXProjection, lThreshold);
      cv::Mat horizontalProjectionImage = Tools::horizontalProjection(lSubImage, lSegmentedProjection);

      unsigned int lLength = lSegmentedProjection.size();
      if (lLength >= 7 && lLength <= 18)
      {
        lGoodBands.push_back(lBand);
        lGoodPlates.push_back(lPlate);
      }
    }
    iBands = lGoodBands;
    iPlates = lGoodPlates;
  }


  double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
  {
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10);
  }

  /**  @function Erosion  */
  void erosion(cv::Mat &src)
  {
    int erosion_type = cv::MORPH_RECT;
    int erosion_elem = 0; // Element:\n 0: Rect \n 1: Cross \n 2: Ellipse
    // int erosion_size = 3; // Kernel size:\n 2n +1"
    if ( erosion_elem == 0 )
    {
      erosion_type = cv::MORPH_RECT;
    }
    else if ( erosion_elem == 1 )
    {
      erosion_type = cv::MORPH_CROSS;
    }
    else if ( erosion_elem == 2)
    {
      erosion_type = cv::MORPH_ELLIPSE;
    }

    // cv::Mat element = getStructuringElement( erosion_type,
    //                   cv::Size( 2 * erosion_size + 1, 2 * erosion_size + 1 ),
    //                   cv::Point( erosion_size, erosion_size ) );

    cv::Mat element = (cv::Mat_<uchar>(1, 3) << 1, 1, 1);

    /// Apply the erosion operation
    cv::Mat dst(src.size(), 0);
    cv::erode( src, dst, element );
    src = dst;
  }


  /** @function Dilation */
  void dilation(cv::Mat &img)
  {
    int dilation_elem = 0; //Element: \n 0: Rect \n 1: Cross \n 2: Ellipse
    int dilation_size = 3; //Kernel size: \n 2n + 1
    int dilation_type = cv::MORPH_RECT;

    if ( dilation_elem == 0 )
    {
      dilation_type = cv::MORPH_RECT;
    }
    else if ( dilation_elem == 1 )
    {
      dilation_type = cv::MORPH_CROSS;
    }
    else if ( dilation_elem == 2)
    {
      dilation_type = cv::MORPH_ELLIPSE;
    }

    cv::Mat element = getStructuringElement( dilation_type,
                      cv::Size( 2 * dilation_size + 1, 2 * dilation_size + 1 ),
                      cv::Point( dilation_size, dilation_size ) );

    /// Apply the dilation operation
    cv::Mat dst;
    cv::dilate(img, dst, element );
    img = dst;
  }

  /**
   * Replacement for Matlab's bwareaopen()
   * Input image must be 8 bits, 1 channel, black and white (objects)
   * with values 0 and 255 respectively
   */
  void removeSmallBlobs(cv::Mat &im, double size)
  {
    // Only accept CV_8UC1
    if (im.channels() != 1 || im.type() != CV_8U)
      return;

    // Find all contours
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(im.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    for (size_t i = 0; i < contours.size(); i++)
    {
      // Calculate contour area
      double area = cv::contourArea(contours[i]);

      // Remove small objects by drawing the contour with black color
      if (area > 0 && area <= size)
        cv::drawContours(im, contours, i, CV_RGB(0, 0, 0), -1);
    }
  }

  bool  sup14(int jumps)
  {
    return (jumps >= 14);
  }

  int avg(std::vector<std::pair<int, int>> linejumps)
  {
    int avg = 0;
    for (auto it = linejumps.begin(); it != linejumps.end(); it++)
      avg += it->second;
    avg /= linejumps.size();
    return avg;
  }



  void roberts(cv::Mat &img)
  {
    // hx = [+1 0; 0 - 1]; hy = [0 + 1; -1 0];
    // Gx = imfilter(img, hx, 'conv', 'same', 'replicate');
    // Gy = imfilter(img, hy, 'conv', 'same', 'replicate');
    // G = sqrt(Gx. ^ 2 + Gy. ^ 2);
    img = img;
  }

  void fillzone(cv::Mat &img, std::vector < std::vector<std::pair<int, int>>> regions)
  {
    cv::Mat zone(img.size(), 0);
    cv::Mat a(img.size(), 0);

    int regioncount = 1;
    for (auto region : regions)
    {
      for (auto row : region)
      {
        for (int x = 0; x < a.cols - 1; x++)
        {
          zone.at<uchar>(row.first, x) = img.at<uchar>(row.first, x);
        }
      }
      regioncount--;
      if (regioncount == 0)
        break;
    }
    img = zone;
  }

  std::vector < std::pair<int, int>> sumvertical(cv::Mat &img)
  {

    std::vector<std::pair<int, int>> jumpscount;

    for (int x = 0; x < img.cols; x++)
    {
      int tmp = 0;
      for (int y = 0; y < img.rows; y++)
      {
        tmp += img.at<uchar>(y, x);
      }
      jumpscount.push_back(std::make_pair(x, tmp));
    }
    for (auto it : jumpscount)
    {
      std::cout << "points: (" << it.first << ", " << it.second << ")" << std::endl;
    }

    return jumpscount;
  }

  std::vector < std::vector<std::pair<int, int>>>
  location(cv::Mat &img)
  {
    cv::Mat dest(img.size(), 0);
    cv::Mat a(img.size(), 0);
    cv::threshold(img, dest, 97, 1, 0);

    for (int x = 0; x < a.cols - 1; x++)
    {
      for (int y = 0; y < a.rows; y++)
      {
        a.at<uchar>(y, x) = abs(dest.at<uchar>(y, x) - dest.at<uchar>(y, x + 1));
      }
    }

    std::vector<std::pair<int, int>> jumpscount;

    for (int y = 0; y < a.rows; y++)
    {
      int tmp = 0;
      for (int x = 0; x < a.cols - 1; x++)
      {
        tmp += a.at<uchar>(y, x);
      }
      jumpscount.push_back(std::make_pair(y, tmp));
    }

    std::vector < std::vector<std::pair<int, int>>> regions;

    bool inregion = false;
    std::vector < std::pair<int, int>> region;
    for (auto it : jumpscount)
    {
      if (it.second >= 14 && !inregion)
      {
        inregion = true;
        region.push_back(it);
      }
      else if (it.second >= 14 && inregion)
      {
        inregion = true;
        region.push_back(it);
      }
      else if (it.second < 14 && !inregion)
      {
        inregion = false;
      }
      else
      {
        inregion = false;
        regions.push_back(region);
        region.clear();
      }
    }

    std::sort(regions.begin(), regions.end(), [](std::vector < std::pair<int, int>> region, std::vector < std::pair<int, int>> region2)
    {
      return (avg(region) > avg(region2));
    });

    fillzone(img, regions);
    // cv::threshold(img, img, 0.5, 255, 0);

    // for (auto region : regions)
    // {
    //   std::cout << "--------------------" << avg(region) << "------------------------" << std::endl;
    //   for (auto it : region)
    //   {
    //     std::cout << "points: (" << it.first << ", " << it.second << ")" << std::endl;
    //   }
    // }
    // cv::threshold(img, img, 0.5, 255, 0);

    return regions;

  }

  void removeSmallBlobs(cv::Mat &img)
  {
    removeSmallBlobs(img, 21.);
  }

  void algochinoi2(cv::Mat &img)
  {
    cv::Mat original = img;
    grayscale(img);
    cv::equalizeHist(img, img);
    median(img);
    otsu(img);
    morph(img);
    sobel(img);
    dilation(img);
    cv::Mat channel[3];
    cv::Mat redfilter = original;
    cv::split(original, channel);
    channel[0] = cv::Mat(original.size(), 0);
    channel[1] = cv::Mat(original.size(), 0);
    cv::merge(channel, 3, original);
    Tools::showImage(original, "redfilter");
  }

  void selectHorizontalPeakProjection(std::vector<std::vector<int> > iXProjectionConvolution,
                                      std::vector<std::pair<int, int> > &iPlates, float iCoefficient)
  {
    for (std::vector<int> lXProjection : iXProjectionConvolution)
    {
      std::vector<int>::iterator lIterator = std::max_element(lXProjection.begin(), lXProjection.end());
      int lXBm = std::find(lXProjection.begin(), lXProjection.end(), *lIterator) - lXProjection.begin();

      int lXB0 = lXBm;
      for (; lXProjection[lXB0] >= iCoefficient * lXProjection[lXBm]; lXB0--);

      int lXB1 = lXBm;
      for (; lXProjection[lXB1] >= iCoefficient * lXProjection[lXBm]; lXB1++);
      lXB1--;

      iPlates.push_back(std::make_pair(lXB0, lXB1));
    }
  }

  void selectVerticalPeakProjection(std::vector<int> &iYProjectionConvolution, std::vector<std::pair<int, int> > &iBands, float iCoefficient)
  {
    std::vector<int> lYProjectionCopy(iYProjectionConvolution);
    int lNbBandsToBeDetected = 3;
    while (lNbBandsToBeDetected > 0)
    {
      std::vector<int>::iterator lIterator = std::max_element(lYProjectionCopy.begin(), lYProjectionCopy.end());
      int lYBm = std::find(lYProjectionCopy.begin(), lYProjectionCopy.end(), *lIterator) - lYProjectionCopy.begin();
      int lYB0 = lYBm;
      for (; lYProjectionCopy[lYB0] >= iCoefficient * lYProjectionCopy[lYBm]; lYB0--);

      int lYB1 = lYBm;
      for (; lYProjectionCopy[lYB1] >= iCoefficient * lYProjectionCopy[lYBm]; lYB1++);
      lYB1--;

      if (lYProjectionCopy[lYB0] == 0)
      {
        for (unsigned i = 0; i < iBands.size(); ++i)
        {
          if (std::abs(lYB0 - iBands[i].second) <= 1)
          {
            iBands[i] = std::pair<int, int>(iBands[i].first, lYB1);
          }
        }
      }
      else if (lYProjectionCopy[lYB1] == 0)
      {
        for (unsigned i = 0; i < iBands.size(); ++i)
        {
          if (std::abs(lYB1 - iBands[i].first) <= 1)
          {
            iBands[i] = std::pair<int, int>(lYB0, iBands[i].second);
          }
        }
      }
      else
        iBands.push_back(std::make_pair(lYB0, lYB1));

      for (int i = lYB0; i <= lYB1; ++i)
        lYProjectionCopy[i] = 0;

      lNbBandsToBeDetected--;
    }
  }

  cv::Mat verticalProject(cv::Mat iImage, std::vector<std::pair<int, int> > iBands)
  {
    cv::Mat lResultImage(iImage.size(), 0);
    for (std::pair<int, int> lBand : iBands)
    {
      int lYB0 = lBand.first;
      int lYB1 = lBand.second;
      for (int y = lYB0; y <= lYB1; ++y)
      {
        for (int x = 0; x < iImage.cols; ++x)
        {
          lResultImage.at<uchar>(y, x) = iImage.at<uchar>(y, x);
        }
      }
    }
    return lResultImage;
  }


  cv::Mat
  printROIs(cv::Mat iImage, std::vector<std::pair<int, int> > iBands, std::vector<std::pair<int, int> > iPlates)
  {
    cv::Mat lResultImage(iImage.size(), 0);
    for (unsigned i = 0; i < iBands.size(); ++i)
    {
      std::pair<int, int> lBand = iBands[i];
      std::pair<int, int> lPlate = iPlates[i];
      for (int y = lBand.first; y <= lBand.second; ++y)
      {
        for (int x = lPlate.first; x <= lPlate.second; ++x)
        {
          lResultImage.at<uchar>(y, x) = iImage.at<uchar>(y, x);
        }
      }
    }
    // Tools::showImage(lResultImage, "ROIs");
    return lResultImage;
  }

  int
  selectByRatio(std::vector<std::pair<int, int>> iBands, std::vector<std::pair<int, int>> iPlates)
  {
    int lBestROIIndex = -1;
    float lBestRatio = 9000;
    for (unsigned int i = 0; i < iBands.size(); ++i)
    {
      std::pair<int, int> lBand = iBands[i];
      std::pair<int, int> lPlate = iPlates[i];
      float lRatio = (float)(lPlate.second - lPlate.first) / (float)(lBand.second - lBand.first);
      if (std::abs(lRatio - 3.14) <= lBestRatio)
      {
        lBestRatio = std::abs(lRatio - 3.14);
        lBestROIIndex = i;
      }
    }
    if (lBestRatio < 11)
      return lBestROIIndex;
    return -1;
  }

  template <typename U>
  std::ostream &operator<<(std::ostream &o, const std::vector<U> &v)
  {
    for (const U &elt : v)
      o << elt << std::endl;
    return o;
  }

  void reduce_noize(cv::Mat &img)
  {
    cv::Mat one;
    cv::threshold(img, one, 128, 1, 0);

    std::vector<int> v_proj = Tools::verticalProjection(one);
    std::vector<int> h_proj;
    int limit = 0;
    int upper_limit = -1;
    int lower_limit = -1;
    int left = -1;
    int right = -1;

    for (size_t i = 0; i < v_proj.size(); ++i)
    {
      if (v_proj[i] <= 14 && upper_limit == -1)
        limit = 1;
      if (v_proj[i] > 14 && limit)
      {
        upper_limit = i - 1 ? i - 1 : 0;
        limit = 0;
      }
      if (v_proj[i] <= 14 && upper_limit != -1)
        lower_limit = i;
      if (v_proj[i] <= 14)
        for (int j = 0; j < img.cols; ++j)
          img.at<uchar>(i, j) = 0;
    }

    if (upper_limit == -1)
      upper_limit = 0;
    if (lower_limit == -1)
      lower_limit = img.rows;

    cv::Rect h_roi(0, upper_limit, img.cols, lower_limit - upper_limit);
    cv::Mat h_cut(img, h_roi);
    h_proj = Tools::horizontalProjection(one);
    for (size_t i = 0, j = h_proj.size() - 1; i < h_proj.size(); ++i, --j)
    {
      if (h_proj[i] != 0 && left == -1)
        left = i - 1 ? i - 1 : 0;
      if (h_proj[j] != 0 && right == -1)
        right = j + 1 < h_proj.size() ? j + 1 : h_proj.size() - 1;
      if (right != -1 && left != -1)
        break;
    }

    cv::Rect v_roi(left, 0, right - left, h_cut.rows);
    cv::Mat v_cut(h_cut, v_roi);
    img = v_cut;
  }


  std::string chinese_char_get(cv::Mat& img)
  {
    int width = img.cols / 6.41;
    cv::Rect chinese_roi(0, 0, width, img.rows);
    cv::Mat chinese_char(img, chinese_roi);
    cv::Mat lInvertImage(chinese_char.size(), 0);
    tesseract::TessBaseAPI lTessBaseAPI;

    setlocale(LC_NUMERIC, "C");

    cv::bitwise_not(chinese_char, lInvertImage);
    lTessBaseAPI.Init(NULL, "chi_sim", tesseract::OEM_DEFAULT);
    lTessBaseAPI.SetVariable("tessedit_char_whitelist", "宁川藏青粤贵闽吉陕蒙晋甘桂鄂赣浙苏新鲁皖湘黑辽云豫冀渝津京");
    lTessBaseAPI.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
    lTessBaseAPI.SetImage((uchar*)lInvertImage.data, lInvertImage.cols, lInvertImage.rows, 1, lInvertImage.cols);

    return lTessBaseAPI.GetUTF8Text();
  }

  std::string alphanum_char_get(cv::Mat& img)
  {
    int width = img.cols / 6.41;
    cv::Rect roi(width, 0, img.cols - width, img.rows);
    cv::Mat alphanums(img, roi);
    cv::Mat lInvertImage(alphanums.size(), 0);
    tesseract::TessBaseAPI lTessBaseAPI;

    setlocale(LC_NUMERIC, "C");

    blurgaussian(alphanums);
    cv::bitwise_not(alphanums, lInvertImage);
    lTessBaseAPI.Init(NULL, "eng", tesseract::OEM_DEFAULT);
    lTessBaseAPI.SetVariable("tessedit_char_whitelist", "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    lTessBaseAPI.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
    lTessBaseAPI.SetImage((uchar*)lInvertImage.data, lInvertImage.cols, lInvertImage.rows, 1, lInvertImage.cols);

    return lTessBaseAPI.GetUTF8Text();
  }


  void character_segmentation(cv::Mat& img)
  {
    cv::Mat one;
    cv::threshold(img, one, 128, 1, 0);
    std::vector<int> h_proj = Tools::horizontalProjection(one);
    int change = !h_proj[0] ? 0 : 1;
    int left = 0;
    size_t i = 0;
    int min_width = img.cols / 10.0;

    std::vector<cv::Mat> characters;

    for (; i < h_proj.size(); ++i)
    {
      if (!change && h_proj[i])
      {
        change = 1;
        left = i;
      }
      if (change && !h_proj[i])
      {
	float diff = i - left;
	cv::Rect char_rect(left, 0, i - left, img.rows);
	characters.push_back(cv::Mat(img, char_rect));
	change = 0;
      }
    }

    cv::Rect char_rect(left, 0, i - left, img.rows);
    characters.push_back(cv::Mat(img, char_rect));
    std::string test = "";
    /*    for (int i = characters.size() - 1, j = 0; j < 6; i--)
    {
      std::cout << characters[i].cols << " " << min_width << std::endl;
      if (characters[i].cols >= min_width)
      {
	std::cout << alphanum_char_get(characters[i]);
	Tools::showImage(characters[i]);
	j++;
      }
      }*/
    std::string plate_text = chinese_char_get(img) + alphanum_char_get(img);
    std::cout << plate_text << std::endl;
  }

  void detect(cv::Mat &img)
  {
    cv::Mat lGrayScaleImage = img;
    cv::Mat lVerticalImage(lGrayScaleImage.size(), 0);
    Filter::verticalEdgeDetection(lGrayScaleImage, lVerticalImage);
    img = lVerticalImage;

    std::vector<int> lYProjection = Tools::verticalProjection(lVerticalImage);
    std::vector<int> lYProjectionConvolution(lYProjection);
    Tools::linearizeVector(lYProjection, lYProjectionConvolution, 4);
    std::vector<std::pair<int, int> > lBands;
    selectVerticalPeakProjection(lYProjectionConvolution, lBands, 0.55);

    cv::Mat lVerticalProjectionImage = verticalProject(lGrayScaleImage, lBands);
    img = lVerticalProjectionImage;

    cv::Mat lHorizontalImage(lGrayScaleImage.size(), 0);
    Filter::horizontalEdgeDetection(lGrayScaleImage, lHorizontalImage);

    std::vector<std::vector<int> > lXProjections;
    Tools::horizontalProjection(lHorizontalImage, lBands, lXProjections);
    std::vector<std::vector<int> > lXProjectionConvolution(lXProjections);
    for (unsigned i = 0; i < lXProjectionConvolution.size(); ++i)
      Tools::linearizeVector(lXProjections[i], lXProjectionConvolution[i], 40);

    std::vector<std::pair<int, int> > lPlates;
    selectHorizontalPeakProjection(lXProjectionConvolution, lPlates, 0.24);
    cv::Mat lROIImage = printROIs(lGrayScaleImage, lBands, lPlates);

    selectBySegmentation(lGrayScaleImage, lBands, lPlates);
    int lIndexGoodPlate = selectByRatio(lBands, lPlates);

    if (lIndexGoodPlate == -1)
      std::cout << "Plate not found..." << std::endl;
    else
    {
      std::pair<int, int> lBand = lBands[lIndexGoodPlate];
      std::pair<int, int> lPlate = lPlates[lIndexGoodPlate];
      cv::Rect roi(lPlate.first, lBand.first, lPlate.second - lPlate.first, lBand.second - lBand.first);
      cv::Mat lFinalImage(img, roi);

      img = lFinalImage;
      reduce_noize(img);
      character_segmentation(img);

      std::cout << "Potential plate found!" << std::endl;
    }
  }

  void test_blue(cv::Mat &img)
  {
    // for (int i = 0; i < img.rows; ++i)
    //   for (int j = 0; j < img.cols; ++j)
    //   {
    //  cv::Vec3b rgb = img.at<cv::Vec3b>(i, j);
    //  if (!(rgb[0] > 90 && rgb[1] > 90 && rgb[2] < 80))
    //    img.at<cv::Vec3b>(i, j) = cv::Vec3b(0,0,0);
    //   }
    cv::Scalar hsv_l(110, 60, 60);
    cv::Scalar hsv_h(130, 255, 255);
    cv::cvtColor(img, img, CV_BGR2HSV);
    cv::inRange(img, hsv_l, hsv_h, img);
  }

  void swt(cv::Mat &img)
  {
    // stroke width transform
    // bw8u : we want to calculate the SWT of this. NOTE: Its background pixels are 0 and forground pixels are 1 (not 255!)
    // cv::Mat bw32f, swt32f, kernel;
    // double  max;
    // int strokeRadius;
    // cv::threshold(img, img, 97, 1, 0);
    // img.convertTo(bw32f, CV_32F);  // format conversion for multiplication
    // distanceTransform(img, swt32f, CV_DIST_L2, 5); // distance transform
    // minMaxLoc(swt32f, NULL, &max);  // find max
    // strokeRadius = (int)ceil(max);  // half the max stroke width
    // kernel = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)); // 3x3 kernel used to select 8-connected neighbors

    // for (int j = 0; j < strokeRadius; j++)
    // {
    //   dilate(swt32f, swt32f, kernel); // assign the max in 3x3 neighborhood to each center pixel
    //   swt32f = swt32f.mul(bw32f); // apply mask to restore original shape and to avoid unnecessary max propogation
    // }
    // // swt32f : resulting SWT image
    // img = swt32f;
    //    detect(img);
    detect(img);
    //    detect(img);
  }
}
