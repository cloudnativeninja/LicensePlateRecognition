#ifndef FILTER_H_
#define FILTER_H_

#include "../main/all.h"

class Filter
{
  public:
    Filter();
    virtual ~Filter();
    static void sobelFilter(const cv::Mat &src, cv::Mat &dst);
    static void horizontalEdgeDetection(const cv::Mat &src, cv::Mat &dst);
    static void verticalEdgeDetection(const cv::Mat &src, cv::Mat &dst);
    static void medianFilter(const cv::Mat &src, cv::Mat &dst);
    static void otsuFilter(const cv::Mat &src, cv::Mat &dst, int threshold);
    static void grayScaleFilter(const cv::Mat &src, cv::Mat &dst);
};

#endif /* FILTER_H_ */
