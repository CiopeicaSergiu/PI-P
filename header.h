#pragma once
#include <opencv2/opencv.hpp>

cv::Mat* toGrayscale(cv::Mat& img);
cv::Mat* toBinary(cv::Mat* img);
template <class T>
void printMask(T** v, int n)
{
	std::cout << std::endl << std::endl;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			std::cout << v[i][j] << "  ";
		std::cout << std::endl;

	}
	std::cout << std::endl << std::endl;
}
int** getCopyMask(int m[][3] , int n);

template <class T>
void destroyMask(T** v, int n)
{
	for (int i = 0; i < n; i++)
		delete[] v[i];
	delete[] v;
}

cv::Mat* applyMask(cv::Mat* img, int** mask, int n);
double** createFilterNetezire(int n, double& nr);
cv::Mat* filtruNetezire(cv::Mat* img, int n);
cv::Mat* getDinamycalCopy(cv::Mat& m);
cv::Mat* filtruGaussian(cv::Mat* img, int n, double gamma);
cv::Mat* contrastImage(cv::Mat* img, int a, int b, int s_a, int s_b);
cv::Mat* combineOutlines(cv::Mat* sy, cv::Mat* sx);
int getMaxTeta(std::map<int, int>& v);
cv::Mat* getHoughPeaks(cv::Mat* img);
double getCircleEq(int x1, int y1, int x, int y);
int ecDreapta(double xA, double yA, double xB, double yB, double x);