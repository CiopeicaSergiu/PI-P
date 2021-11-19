#include "rectangle.h"
#include <iostream>
#include <string.h>

int main()
{
	cv::Mat image = cv::imread("Images/car.jpeg");
	//Sobel Masks
	int sMaskx[][3] = { -1, -2, -1,
						 0, 0, 0,
						 1, 2, 1 };

	int sMasky[][3] = {-1, 0, 1,
					  -2, 0, 2,
					   -1, 0, 1};
	//Quick Mask
	int quickM[][3] = { -1, 0, -1,
						0, 4, 0,
						-1, 0, -1 };
	

	int** sx= getCopyMask(sMaskx, 3);
	int** sy = getCopyMask(sMasky, 3);
	int** qmask= getCopyMask(quickM, 3);
	if (image.empty())
	{
		std::cout << "Could not open or find the image" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	

	cv::Mat* imgGray= toGrayscale(image);
	cv::imwrite("Images/carGray.jpg", *imgGray);

	cv::Mat* imgNet = filtruGaussian(imgGray,3,0.4);
	cv::imwrite("Images/carGauss.jpg", *imgNet);

	cv::Mat* imgQmask = applyMask(imgNet, qmask, 3);
	cv::Mat* imgOutSx = applyMask(imgNet, sx, 3);
	cv::imwrite("Images/carSx.jpg", *imgOutSx);
	cv::Mat* imgOutSy = applyMask(imgNet, sy, 3);
	cv::imwrite("Images/carSy.jpg", *imgOutSy);
	cv::Mat* imgSxy = combineOutlines(imgOutSy, imgOutSx);
	cv::imwrite("Images/carSxy.jpg", *imgSxy);

	cv::Mat* imgBynSxy = toBinary(imgSxy);
	cv::imwrite("Images/carSxyB.jpg", *imgBynSxy);
	///cv::Mat* img_my_rectangles = new cv::Mat(*imgSxy);
	
	
	getHoughPeaks(imgBynSxy);
	//rectangle r1(12.50, -9.47, 9.17, 78.63);
	//r1.printVarfuri();
	
	//int *rez=

	std::string windowName1 = "ImgFGauss"; //Name of the window
	std::string windowName2 = "ImgFGaussByn";

	cv::namedWindow(windowName1); // Create a window
	cv::namedWindow(windowName2);
	
	cv::imshow(windowName2, *imgNet); // Show our image inside the created window.
	cv::imshow(windowName1, *imgBynSxy);

	cv::waitKey(0); // Wait for any keystroke in the window

	cv::destroyWindow(windowName1); //destroy the created window
	cv::destroyWindow(windowName2);
	destroyMask(sx, 3);
	destroyMask(sy, 3);
	return 0;
}