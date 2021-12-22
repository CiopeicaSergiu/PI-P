#pragma once

#define _USE_MATH_DEFINES

#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <vector>




cv::Mat* readImage(std::string path);
int getDistance(cv::Point p1, cv::Point p2);
std::vector<cv::Mat> imageContursV1(cv::Mat* img, cv::Mat*& rez, int minArea, int maxArea);
double angle(cv::Point A, cv::Point B, cv::Point C);
std::vector<cv::Mat> imageContursV2(cv::Mat* img, cv::Mat*& rez, int minArea, int maxArea);
int calculatePoint(cv::Point p1, cv::Point p2, int x);