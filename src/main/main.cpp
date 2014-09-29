#include "all.h"

int main(int argc, char** argv)
{
  std::string lDebugString = "--debug";
  std::string lOcrString = "--ocr";
  bool lDebugMode = false;
  bool lOcrMode = false;
  cv::Mat img_gray;
  cv::Mat grad;
  cv::Mat lImage;
  char* window_name = "Sobel Demo - Simple Edge Detector";
  int scale = 1;
  int delta = 0;
  int ddepth = CV_16S;

  if (argc == 4)
  {
    lImage = cv::imread(argv[3], CV_LOAD_IMAGE_UNCHANGED);
    if (argv[1] == lDebugString)
      lDebugMode = true;
    else if (argv[1] == lOcrString)
      lOcrMode = true;
    if (argv[2] == lDebugString)
      lDebugMode = true;
    else if (argv[2] == lOcrString)
      lOcrMode = true;
  } else if (argc == 3) {
    lImage = cv::imread(argv[2], CV_LOAD_IMAGE_UNCHANGED);
    if (argv[1] == lDebugString)
      lDebugMode = true;
    else if (argv[1] == lOcrString)
      lOcrMode = true;
  } else if (argc == 2) {
    lImage = cv::imread(argv[1], CV_LOAD_IMAGE_UNCHANGED);
  }

  if ((argc != 2 && argc != 3 && argc != 4) || !lImage.data) {
    std::cout << "No image data" << std::endl;
    return -1;
  }
  
  cv::GaussianBlur(lImage, lImage, cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT );

  /// Convert it to gray
  cv::cvtColor( lImage, img_gray, CV_RGB2GRAY );

  /// Generate grad_x and grad_y
  cv::Mat grad_x, grad_y;
  cv::Mat abs_grad_x, abs_grad_y;
  
  /// Gradient X
  //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
  cv::Sobel( img_gray, grad_x, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT );
  cv::convertScaleAbs( grad_x, abs_grad_x );

  /// Gradient Y
  //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
  cv::Sobel( img_gray, grad_y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT );
  cv::convertScaleAbs( grad_y, abs_grad_y );
  
  /// Total Gradient (approximate)
  cv::addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );

  Tools::showImage(grad, "Original Image");

  return 0;
}
