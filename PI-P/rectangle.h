#pragma once
#include "header.h"

int* getIntLines(double p1, double teta1, double p2, double teta2);

class point
{
private:
	friend class rectangle;
	int x, y;
};

class rectangle
{
private:
	point v1;
	point v2;
	point v3;
	point v4;
public:
	rectangle(double p1, double teta1, double p2, double teta2)
	{
		int* varf1 = getIntLines(-p1, teta1, -p2, teta2);
		int* varf2 = getIntLines(-p1, teta1, p2,teta2);
		int* varf3 = getIntLines(p1, teta1, p2, teta2);
		int* varf4 = getIntLines(p1, teta1, -p2, teta2);
		
		v1.y = varf1[0];
		v1.x = varf1[1];

		v2.y = varf2[0];
		v2.x = varf2[1];

		v3.y = varf3[0];
		v3.x = varf3[1];

		v4.y = varf4[0];
		v4.x = varf4[1];

		delete[] varf1;
		delete[] varf2;
		delete[] varf3;
		delete[] varf4;
	}

	void printVarfuri()
	{
		std::cout << "\nRectangle: ";
		std::cout << "\nv1: (y: " << v1.y << ",x: " << v1.x<<")";
		std::cout << "\nv2: (y: " << v2.y << ",x: " << v2.x << ")";
		std::cout << "\nv3: (y: " << v3.y << ",x: " << v3.x << ")";
		std::cout << "\nv4: (y: " << v4.y << ",x: " << v4.x << ")";
		std::cout << std::endl << std::endl;
	}

	void drawIt(cv::Mat* img)
	{
		int h = img->rows;
		int w = img->cols;
		unsigned char* imgData = (unsigned char*)img->data;
		for (int x = v1.x; x < v2.x; x++)
		{
			imgData[w * v1.y + x] = 100;
			imgData[w * v4.y + x] = 100;
		}

		for (int y = v1.y; y <= v4.y; y++)
		{
			imgData[w * y + v1.x] = 100;
			imgData[w * y + v2.x] = 100;

		}
	}

};



