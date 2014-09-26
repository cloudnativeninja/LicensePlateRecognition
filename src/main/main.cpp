#include "all.h"

int main(int argc, char** argv) {
	std::string lDebugString = "--debug";
	std::string lOcrString = "--ocr";
	bool lDebugMode = false;
	bool lOcrMode = false;
	cv::Mat lImage;

	if (argc == 4) {
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

	Tools::showImage(lImage, "Original Image");

    return 0;
}