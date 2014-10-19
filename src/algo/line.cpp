#include "line.hh"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

line::line(cv::Vec2f& vec)
{
  double a;
  double b;
  double x0;
  double y0;

  rho_ = vec[0];
  theta_ = vec[1];

  a = cos(theta_);
  b = sin(theta_);
  x0 = a*rho_;
  y0 = b*rho_;

  p1_.x = cvRound(x0 + 1000 * (-b));
  p1_.y = cvRound(y0 + 1000 * (a));
  p2_.x = cvRound(x0 - 1000 * (-b));
  p2_.y = cvRound(y0 - 1000 * (a));
}

line::line(const line& other)
  : p1_ (other.p1_)
  , p2_ (other.p2_)
  , rho_ (other.rho_)
  , theta_ (other.theta_)
  , slope_ (other.slope_)
  , first_ (other.first_)
{
}

line::line()
{
}

line::~line()
{
}

int line::is_horizontal()
{
  return theta_ > CV_PI / 180 * 80 && theta_ < CV_PI / 180 * 100;
}

int line::is_vertical()
{
  return !is_horizontal();
}

int line::is_parallel(line& other)
{
  float s1 = get_slope();
  float s2 = other.get_slope();

  //std::cout << "s1: " << s1 << ", s2: " << s2 << ", s2 - s1: " << std::fabs(s2 - s1) << std::endl;

  return !std::fabs(s2 - s1);
}

float line::get_slope()
{
  if (first_)
  {
    float diffy = p2_.y - p1_.y;
    float diffx = p2_.x - p1_.x;
    slope_ = diffy / diffx;
    first_ = 0;
  }

  return slope_;
}

void line::draw(cv::Mat& img)
{
  cv::line(img, p1_, p2_, cv::Scalar(192,192,255), 3, CV_AA);
}

void line::dump(std::ostream& o)
{
  o << "p1: " << p1_ << ", p2: " << p2_ << std::endl;
}
