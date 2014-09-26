#ifndef TOOLS_H_
#define TOOLS_H_

#include "../main/all.h"


class Tools {
public:
	Tools();
	virtual ~Tools();

public:
	static void showImage(cv::Mat iImage);
	static void showImage(cv::Mat iImage, std::string iName);
};

#endif /* TOOLS_H_ */
