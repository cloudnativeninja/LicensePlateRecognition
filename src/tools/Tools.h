#ifndef TOOLS_H_
#define TOOLS_H_

#include "../main/all.h"
#include <QImage>

class Tools
{
  public:
    Tools();
    virtual ~Tools();

  public:
    static void showImage(cv::Mat iImage);
    static void showImage(cv::Mat iImage, std::string iName);
  public:
    static int convolutionXSobel(cv::Mat iImage, int iX, int iY);
    static int convolutionYSobel(cv::Mat iImage, int iX, int iY);
    static int convolutionXDetection(cv::Mat iImage, int iX, int iY);
    static int convolutionYDetection(cv::Mat iImage, int iX, int iY);

  public:
    static std::vector<int> verticalProjection(cv::Mat iVerticalImage);
    static std::vector<int> horizontalProjection(cv::Mat iHorizontalImage);
    static cv::Mat horizontalProjection(cv::Mat iImage, std::vector<std::pair<unsigned, unsigned> > xBornes);
    static void horizontalProjection(cv::Mat iImage, std::vector<std::pair<int, int> > iBands, std::vector<std::vector<int> > &iXProjections);

  public:
    static void linearizeVector(std::vector<int> iProjection, std::vector<int> &iResult, int iWidth);
    static void insertionSort(int iWindow[]);
    static QImage Mat2QImage(cv::Mat const& src);



};

#endif /* TOOLS_H_ */
