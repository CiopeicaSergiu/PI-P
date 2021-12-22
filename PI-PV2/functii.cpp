#include "header.h"

cv::Mat* readImage(std::string path)
{
	cv::Mat img = cv::imread(path, CV_8UC1);
	if (img.empty())
	{
		std::cout << "Could not open or find the image" << std::endl;
		exit(EXIT_FAILURE);
	}

	return new cv::Mat(img);
}


int getDistance(cv::Point p1, cv::Point p2)
{
	return round(sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y)));
}

int calculatePoint(cv::Point p1, cv::Point p2, int x)
{
	return (int)round((double)(p2.y - p1.y) / (p2.x - p1.x) * (x - p1.x) + p1.y);
}

//Nefunctional momentan
std::vector<cv::Mat> imageContursV1(cv::Mat* img, cv::Mat*& rez, int minArea, int maxArea)
{
	cv::RNG rng(time(NULL));
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;

	double tresh_max = 255;
	double tresh_min = tresh_max / 3;

	cv::Mat aux(img->size(), CV_8UC1);

	std::vector<std::vector<cv::Point>> squares;

	cv::GaussianBlur(*img, aux, cv::Size(3, 3), 3);
	cv::Canny(aux, aux, tresh_min, tresh_max);
	cv::findContours(aux, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

	rez = new cv::Mat(img->size(), CV_8UC3, cv::Scalar(0, 0, 0));



	cv::Point P1;
	cv::Point P2;


	unsigned char* data = (unsigned char*)rez->data;
	int ch = rez->channels();
	int h = rez->rows;
	int w = rez->cols;


	std::vector<cv::Point> vertices;
	for (int i = 0; i < contours.size(); i++)
	{
		if (contours[i].size() >= 4)
		{

			P1 = contours[i][0];
			P2 = contours[i][1];

			vertices.push_back(P1);
			int size = contours[i].size();
			for (int j = 2; j < size; j++)
			{
				cv::Point P3 = contours[i][j];
				if (abs(calculatePoint(P1, P2, P3.x) - P3.y) > 20)
				{
					P1 = P3;
					P2 = contours[i][(j + 1) % contours[i].size()];
					vertices.push_back(P3);
				}
			}
			vertices.push_back(contours[i][contours[i].size() - 1]);
			cv::Scalar color = cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));

			double my_area = cv::contourArea(vertices);
			if (my_area >= minArea && my_area <= maxArea && vertices.size() >= 4)
			{


				bool duplicate = false;
				double maxAngle = 0;

				for (int j = 0; j < vertices.size() - 2; j++)
				{
					double Angle = angle(vertices[j], vertices[(j + 1) % vertices.size()], vertices[(j + 2) % vertices.size()]);

					std::cout << "\n" << Angle;
					if (maxAngle < Angle && fabs(Angle-180)>10)
						maxAngle = Angle;


				}

				std::cout << "////////////////";
				if (fabs(maxAngle - 90) < 10)
				{

					if (vertices.size() > 0)
					{
						auto square = squares[squares.size() - 1];
						for (int k = 0; k < vertices.size(); k++)
						{
							auto itr = std::find(square.begin(), square.end(), vertices[k]);
							if (itr != square.end())
							{
								squares.erase(std::find(squares.begin(), squares.end(), square));
								squares.push_back(vertices);
								duplicate = true;
								break;
							}
						}

					}

					if (!duplicate)
						squares.push_back(vertices);

				}


			}

		}

	}

	std::vector<cv::Mat> segmenations;

	for (auto square : squares)
	{

		cv::Scalar color = cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		for (int j = 0; j < square.size(); j++)
		{
			cv::line(*rez, square[j], square[(j + 1) % square.size()], color, cv::LINE_8);
		}

		segmenations.push_back(cv::Mat(*img, cv::Rect(square[0], square[2])));

	}

	return segmenations;

}


double angle(cv::Point A, cv::Point B, cv::Point C)
{
	cv::Point AB = B - A;
	cv::Point BC = C - B;
	int dotP = AB.x * BC.x + AB.y * BC.y;
	double AB_d = sqrt(AB.x*AB.x+AB.y*AB.y);
	double BC_d = sqrt(BC.x*BC.x+BC.y*BC.y);
	return acos((double)dotP/(AB_d*BC_d))*180/M_PI;
}

std::vector<cv::Mat> imageContursV2(cv::Mat* img, cv::Mat*& rez, int minArea, int maxArea)
{
	cv::RNG rng(time(NULL));
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;

	cv::Mat aux(img->size(), CV_8UC1);
	

	double tresh_max =255;
	double tresh_min =tresh_max/3;

	cv::GaussianBlur(*img, aux, cv::Size(3, 3), 3);
	
	//adaptiveThreshold(aux, aux, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 13, 0);
	cv::Canny(aux, aux, tresh_min, tresh_max);
	//cv::threshold(aux, aux, 0, 255, cv::THRESH_OTSU | cv::THRESH_BINARY);
	
	cv::findContours(aux, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
	
	rez = new cv::Mat(img->size(), CV_8UC3, cv::Scalar(0, 0, 0));

	std::vector<cv::Point> polygon;
	std::vector<std::vector<cv::Point>> squares;

	for (int i = 0; i < contours.size(); i++)
	{
		cv::approxPolyDP(cv::Mat(contours[i]), polygon, cv::arcLength(cv::Mat(contours[i]), true) * 0.02, true);
		double my_area = cv::contourArea(polygon);
		if (my_area >= minArea && my_area <= maxArea && polygon.size()==4)
		{
			bool duplicate = false;
			double maxAngle = 0;

			for (int j = 0; j < polygon.size()-2; j++)
			{
				double Angle = angle(polygon[j], polygon[(j+1)%polygon.size()], polygon[(j+2)%polygon.size()]);
				
				
				if (maxAngle<Angle)
					maxAngle = Angle;
				
				
			}
			if (fabs(maxAngle - 90) <10)
			{
			
				if(squares.size()>0)
				{
					auto square = squares[squares.size() - 1];
					for (int k = 0; k < polygon.size(); k++)
					{
						auto itr = std::find(square.begin(), square.end(), polygon[k]);
						if (itr!= square.end())
						{
							squares.erase(std::find(squares.begin(), squares.end(), square));
							squares.push_back(polygon);
							duplicate = true;
							break;
						}
					}

				}
				
				if(!duplicate)
					squares.push_back(polygon);
				

			}
		}
		
	}
	
	std::vector<cv::Mat> segmenations;
	std::cout << "Squares: "<<squares.size()<<"\n";
	for (auto square : squares)
	{
		std::cout << "\n" << square;
		cv::Scalar color = cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		for (int j = 0; j < square.size(); j++)
		{
			cv::line(*rez, square[j], square[(j+1)%square.size()], color, cv::LINE_8);
		}
		
		segmenations.push_back(cv::Mat(*img, cv::Rect(square[0], square[2])));

	}

	return segmenations;

}

