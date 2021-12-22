#include "header.h"


int main()
{
	const int n = 3;

	std::string v[n];


	//std::string path1 = "Images/car1.jpeg";
	v[0] = "Images/car2.jpg";
	v[1] = "ContursV1";
	v[2] = "ContursV2";


	cv::Mat* p[n];

	p[0] = readImage(v[0]);

	auto rectanglesV1= imageContursV1(p[0], p[1], 300, 100000);
	auto rectanglesV2= imageContursV2(p[0], p[2], 300, 100000);


	for (int i = 0; i < n; i++)
	{
		cv::namedWindow(v[i]);
		cv::imshow(v[i], *p[i]);
		cv::waitKey(0);
		cv::destroyWindow(v[i]);
	}

	for (auto single_rect : rectanglesV1)
	{
		cv::namedWindow("RectV1");
		cv::imshow("RectV1", single_rect);
		cv::waitKey(0);
		cv::destroyWindow("RectV1");
	}


	for (auto single_rect : rectanglesV2)
	{
		cv::namedWindow("RectV2");
		cv::imshow("RectV2", single_rect);
		cv::waitKey(0);
		cv::destroyWindow("RectV2");
	}
	return 0;
}