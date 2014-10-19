#ifndef LINE_HH_
# define LINE_HH_

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class line
{
public:
  line(cv::Point&, cv::Point&);
  line(cv::Vec2f&);
  line(const line&);
  line();
  virtual ~line();

  int is_horizontal();
  int is_vertical();
  int is_parallel(line&);
  void draw(cv::Mat& img);

  float get_slope();
  void dump(std::ostream&);

private:
  cv::Point p1_;
  cv::Point p2_;
  float rho_;
  float theta_;
  float slope_;
  int first_ = 1;
};

#endif /* !LINE_HH_ */
