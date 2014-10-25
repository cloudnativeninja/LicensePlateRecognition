#include "Tools.h"

Tools::Tools() {}

Tools::~Tools() {}

void
Tools::showImage(cv::Mat iImage)
{
  cv::namedWindow("Display Image", CV_WINDOW_AUTOSIZE);
  cv::imshow("Display Image", iImage);
  cv::waitKey(0);
}

void
Tools::showImage(cv::Mat iImage, std::string iName)
{
  cv::namedWindow(iName, CV_WINDOW_AUTOSIZE);
  cv::imshow(iName, iImage);
  cv::waitKey(0);
}

int
Tools::convolutionYSobel(cv::Mat iImage, int iX, int iY)
{
  return iImage.at<uchar>(iY - 1, iX - 1)
         + 2 * iImage.at<uchar>(iY, iX - 1)
         + iImage.at<uchar>(iY + 1, iX - 1)
         - iImage.at<uchar>(iY - 1, iX + 1)
         - 2 * iImage.at<uchar>(iY, iX + 1)
         - iImage.at<uchar>(iY + 1, iX + 1);
}

int Tools::convolutionXSobel(cv::Mat iImage, int iX, int iY)
{
  return iImage.at<uchar>(iY - 1, iX - 1)
         + 2 * iImage.at<uchar>(iY - 1, iX)
         + iImage.at<uchar>(iY - 1, iX + 1)
         - iImage.at<uchar>(iY + 1, iX - 1)
         - 2 * iImage.at<uchar>(iY + 1, iX)
         - iImage.at<uchar>(iY + 1, iX + 1);
}

int
Tools::convolutionXDetection(cv::Mat iImage, int iX, int iY)
{
  return (- iImage.at<uchar>(iY - 1 , iX - 1)
          - iImage.at<uchar>(iY - 1, iX)
          - iImage.at<uchar>(iY - 1, iX + 1)
          + iImage.at<uchar>(iY + 1, iX - 1)
          + iImage.at<uchar>(iY + 1, iX)
          + iImage.at<uchar>(iY + 1, iX + 1));
}

int
Tools::convolutionYDetection(cv::Mat iImage, int iX, int iY)
{
  return (- iImage.at<uchar>(iY - 1, iX - 1)
          - iImage.at<uchar>(iY, iX - 1)
          - iImage.at<uchar>(iY + 1, iX - 1)
          + iImage.at<uchar>(iY - 1, iX + 1)
          + iImage.at<uchar>(iY, iX + 1)
          + iImage.at<uchar>(iY + 1, iX + 1));
}

std::vector<int>
Tools::verticalProjection(cv::Mat iVerticalImage)
{
  std::vector<int> lYProjection;
  for (int y = 0; y < iVerticalImage.rows; ++y)
  {
    int lSum = 0;
    for (int x = 0; x < iVerticalImage.cols; x++)
    {
      lSum += iVerticalImage.at<uchar>(y, x);
    }
    lYProjection.push_back(lSum);
  }
  return lYProjection;
}

std::vector<int>
Tools::horizontalProjection(cv::Mat iHorizontalImage)
{
  std::vector<int> lXProjection;
  for (int x = 0; x < iHorizontalImage.cols; ++x)
  {
    int lSum = 0;
    for (int y = 0; y < iHorizontalImage.rows; y++)
    {
      lSum += iHorizontalImage.at<uchar>(y, x);
    }
    lXProjection.push_back(lSum);
  }
  return lXProjection;
}

cv::Mat
Tools::horizontalProjection(cv::Mat iImage, std::vector<std::pair<unsigned, unsigned> > iXLimits)
{
  cv::Mat lResultImage(iImage.size(), 0);
  for (unsigned i = 0; i < iXLimits.size(); ++i)
  {
    unsigned lBornInf = iXLimits[i].first;
    unsigned lBornSup = iXLimits[i].second;
    for (unsigned x = lBornInf; x < lBornSup + 1; ++x)
    {
      for (int y = 0; y < iImage.rows; ++y)
      {
        lResultImage.at<uchar>(y, x) = iImage.at<uchar>(y, x);
      }
    }
  }
  return lResultImage;
}

void
Tools::horizontalProjection(cv::Mat iImage, std::vector<std::pair<int, int> > iBands, std::vector<std::vector<int> > &iXProjections)
{
  for (std::pair<int, int> lBand : iBands)
  {
    int lY0 = lBand.first;
    int lY1 = lBand.second;
    std::vector<int> lXProjection;
    for (int x = 0; x < iImage.cols; ++x)
    {
      int lSum = 0;
      for (int y = lY0; y <= lY1; ++y)
      {
        lSum += iImage.at<uchar>(y, x);
      }
      lXProjection.push_back(lSum);
    }
    iXProjections.push_back(lXProjection);
  }
}

void
Tools::linearizeVector(std::vector<int> iProjection, std::vector<int> &iResult, int iWidth)
{
  for (unsigned i = iWidth; i <= iProjection.size() - iWidth; ++i)
  {
    for (int w = -iWidth / 2; w < iWidth / 2; ++w)
    {
      iResult[i] += iProjection[i + w];
    }
    iResult[i] /= iWidth;
  }
}

void
Tools::insertionSort(int iWindow[])
{
  int lTmp , j;
  for (int i = 0; i < 9; i++)
  {
    lTmp = iWindow[i];
    for (j = i - 1; j >= 0 && lTmp < iWindow[j]; j--)
    {
      iWindow[j + 1] = iWindow[j];
    }
    iWindow[j + 1] = lTmp;
  }
}

QImage Tools::Mat2QImage(cv::Mat const& src)
{
  QImage dest(src.cols, src.rows, QImage::Format_ARGB32);
  const float scale = 255.0;
  if (src.depth() == CV_8U) {
    if (src.channels() == 1) {
      for (int i = 0; i < src.rows; ++i) {
	for (int j = 0; j < src.cols; ++j) {
	  int level = src.at<quint8>(i, j);
	  dest.setPixel(j, i, qRgb(level, level, level));
	}
      }
    } else if (src.channels() == 3) {
      for (int i = 0; i < src.rows; ++i) {
	for (int j = 0; j < src.cols; ++j) {
	  cv::Vec3b bgr = src.at<cv::Vec3b>(i, j);
	  dest.setPixel(j, i, qRgb(bgr[2], bgr[1], bgr[0]));
	}
      }
    }
  } else if (src.depth() == CV_32F) {
    if (src.channels() == 1) {
      for (int i = 0; i < src.rows; ++i) {
	for (int j = 0; j < src.cols; ++j) {
	  int level = scale * src.at<float>(i, j);
	  dest.setPixel(j, i, qRgb(level, level, level));
	}
      }
    } else if (src.channels() == 3) {
      for (int i = 0; i < src.rows; ++i) {
	for (int j = 0; j < src.cols; ++j) {
	  cv::Vec3f bgr = scale * src.at<cv::Vec3f>(i, j);
	  dest.setPixel(j, i, qRgb(bgr[2], bgr[1], bgr[0]));
	}
      }
    }
  }
  return dest;
}
