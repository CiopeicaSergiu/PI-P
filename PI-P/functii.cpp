#define _USE_MATH_DEFINES
#include "rectangle.h"
#include <math.h>
#include <cstdlib>

cv::Mat* toGrayscale(cv::Mat& img)
{
	int nr=img.channels();

	int h = img.rows;
	int w = img.cols;
	
	cv::Mat* rez = new cv::Mat(h, w, CV_8UC1);
	
	unsigned char* my_dataImg = (unsigned char*)img.data;
	unsigned char* my_dataRez = (unsigned  char*)rez->data;
	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++)
			my_dataRez[y*w+x] = round(((unsigned int)my_dataImg[(y*w+x)*nr]+ (unsigned int)my_dataImg[(y * w + x) * nr+1]+ (unsigned int)my_dataImg[(y * w + x) * nr + 2])/(double)nr);
			//my_dataRez[y * w + x] = 0.299 * (double)my_dataImg[(y * w + x) * nr] + 0.587 * (double)my_dataImg[(y * w + x) * nr] + 0.114 * (double)my_dataImg[(y * w + x) * nr];
	return rez;
}

unsigned char getTreshHold(cv::Mat* img)
{
	if (img->channels()>1)
	{
		std::cout << "\nChannel-ul este mai mare de 1!(getTreshHold)";
		exit(EXIT_FAILURE);
	}

	int h = img->rows;
	int w = img->cols;

	unsigned char rez;
	double aux=0;
	unsigned char* my_pixels = (unsigned char*)img->data;

	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++)
			aux += my_pixels[y * w + x];
	aux /= ((double)w *(double) h);
	rez = round(aux);
	return rez;
}

cv::Mat* toBinary(cv::Mat* img)
{
	int h = img->rows;
	int w = img->cols;

	cv::Mat* rez = new cv::Mat(h, w, CV_8UC1);

	unsigned char aux = getTreshHold(rez);
	unsigned char* imgData = (unsigned char*)img->data;
	unsigned char* rezData = (unsigned char*)rez->data;
	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++)
			if (imgData[y*w+x] <= aux)
				rezData[y * w + x] = 0;
			else
				rezData[y * w + x] = 255;

	return rez;
}

cv::Mat* applyMask(cv::Mat* img, int** mask, int n)
{
	int h = img->rows;
	int w = img->cols;

	cv::Mat* rez = new cv::Mat(h, w, CV_8UC1);

	unsigned char* imgData = (unsigned char*)img->data;
	unsigned char* rezData = (unsigned char*)rez->data;
	int aux;
	int a = (n - 1) / 2;

	for (int y =a ; y < h-a; y++)
		for (int x = a; x < w-a; x++)
		{
			aux = 0;
			for (int t = -a; t <= a; t++)
				for (int s = -a; s <= a; s++)
						aux += imgData[w*(y+t)+x+s]*mask[t+a][s+a];
			if (aux < 0)
				aux = 0;
			if (aux > 255)
				aux = 255;
			rezData[y * w + x] = aux;
		}


	return rez;
}



int** getCopyMask(int m[][3], int n)
{

	int** v = new int* [n];
	for (int i = 0; i < n; i++)
	{
		v[i] = new int[n];
		for (int j = 0; j < n; j++)
			v[i][j] = m[i][j];
	}
	return v;
}




double** createFilterNetezire(int n, double& nr)
{

	double** v = new double* [n];
	int a = floor(n / 2), b = a;
	bool inc = true;
	nr = 0;
	for (int i = 0; i < n; i++)
	{
		v[i] = new double[n] {};
		nr += b - a + 1;
		for (int j = a; j <= b; j++)
			v[i][j] = 1;
		if (inc)
		{
			--a;
			++b;
		}
		else
		{
			++a;
			--b;
		}
		if (a<0 && b >n - 1)
		{
			a += 2;
			b -= 2;
			inc = false;
		}

	}

	return v;
}

cv::Mat* filtruNetezire(cv::Mat* img, int n)
{
	int h = img->rows;
	int w = img->cols;

	if (!n % 2)
	{
		std::cout << "\nFiltrul are dimnesiuni pare!(filtruNetezire)";
		return img;
	}

	if (n == 1)
		return img;

	double nr, aux;

	double** filter = createFilterNetezire(n, nr);
	int i, j;

	cv::Mat* rez = new cv::Mat(h, w, CV_8UC1);

	unsigned char* imgData = (unsigned char*)img->data;
	unsigned char* rezData = (unsigned char*)rez->data;

	int a = floor((n - 1) / 2), b = a;

	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++)
		{
			aux = 0;
			for (int s = -a; s <= a; s++)
				for (int t = -b; t <= b; t++)
				{
					if (y + s < 0)
						i = y + s + h;
					else
						if (y + s >= h)
							i = y + s - h;
						else
							i = y + s;

					if (x + t < 0)
						j = x + t + w;
					else
						if (x + t >= w)
							j = x + t - w;
						else
							j = x + t;

					aux += filter[s + a][t + b] * (double)imgData[i * w + j];
				}
			rezData[y * w + x] = aux / nr;
		}
	destroyMask(filter, n);
	return rez;
}

cv::Mat* getDinamycalCopy(cv::Mat& m)
{
	return new cv::Mat(m);
}


cv::Mat* filtruGaussian(cv::Mat* img, int n, double gamma)
{
	int w = img->cols;
	int h = img->rows;

	if (w <= 0 || h <= 0 || n <= 0)
	{
		std::cout << "\nValori imposibile!(filtruGaussian)";
		return img;
	}

	if (!n % 2)
	{
		std::cout << "\nFiltrul are dimnesiuni pare!(filtruGaussian)";
		return img;
	}

	if (n == 1)
		return img;

	cv::Mat* rez = new cv::Mat(h, w, CV_8UC1);

	unsigned char* imgData = (unsigned char*)img->data;
	unsigned char* rezData = (unsigned char*)rez->data;

	int a = (n - 1) / 2;
	double aux;
	int i, j;
	for (int y = a; y < h-a; y++)
		for (int x = a; x < w-a; x++)
		{
			aux = 0;
			for (int t = -a; t <= a; t++)
				for (int s = -a; s <= a; s++)
					aux += imgData[(y+s) * w + x+s] * exp(-(s * s + t * t) / (2 * gamma * gamma));
				
			aux= aux / (2 * M_PI * gamma * gamma);
			if (aux < 0)
				aux = 0;

			if (aux > 255)
				aux = 255;

			rezData[y * w + x] = aux;
		}
	return rez;

}

cv::Mat* contrastImage(cv::Mat* img, int a, int b, int s_a, int s_b)
{
	int h = img->rows;
	int w = img->cols;

	cv::Mat* rez = new cv::Mat(h, w, CV_8UC1);

	if (a < 0 || a >= b || b>255 || s_a > 255 || s_b > 255 || s_a < 0 || s_b < 0)
		return img;

	unsigned char* imgData = (unsigned char*)img->data;
	unsigned char* imgRez = (unsigned char*)rez->data;

	unsigned char m = s_a / a, n = (s_b - s_a) / (b - a), p = (255 - s_b) / (255 - b);


	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++)
		{
			if (imgData[w * y + x] >= 0 && imgData[w * y + x] <= a)
				imgRez[w * y + x] = m * imgData[w * y + x];

			if (imgData[w * y + x] > a && imgData[w * y + x] <= b)
				imgRez[w * y + x] = n * (imgData[w * y + x] - a) + s_a;

			if (imgData[w * y + x] > b && imgData[w * y + x] <= 255)
				imgRez[w * y + x] = p * (imgData[w * y + x] - b) + s_b;

		}
	return rez;
}

	cv::Mat* combineOutlines(cv::Mat * sy, cv::Mat * sx)
	{
		if (sy->rows != sx->rows|| sy->cols!= sx->cols)
		{
			std::cout << "\nDimensiunile contururilor nu corespund!(combineOutlines)";
			exit(EXIT_FAILURE);
		}
		int size=sx->rows*sx->cols;
		cv::Mat* rez = new cv::Mat(sx->rows, sx->cols, CV_8UC1);
		
		unsigned char* imgDatay = (unsigned char*)sy->data;
		unsigned char* imgDatax = (unsigned char*)sx->data;
		unsigned char* imgRez = (unsigned char*)rez->data;
		int aux;
		for (int i = 0; i < size; i++)
		{	
			aux = imgDatax[i] + imgDatay[i];
			if (aux > 255)
				aux = 255;
			imgRez[i] = aux;
		}

		return rez;
	}
		
	int getMaxTeta(std::map<int, int>& v)
	{
		std::map<int, int>::iterator it= v.begin();
		std::pair<int, int> max=*it;
		++it;
		while (it != v.end())
		{
			if (it->second > max.second)
				max = *it;
			++it;
		}
		return max.first;
	}

	double houghFunction(int x, int y, double teta)
	{
		return (double)x* cos(teta) + (double)y * sin(teta);
	}

	int* getIntLines(double p1, double teta1, double p2, double teta2)
	{
		int* rez = new int[2];

		rez[0] = (int)round((p2 * cos(teta1) - p1 * cos(teta2))/sin(teta2 - teta1));//y
		rez[1] = (int)round((p2 * sin(teta1) - p1 * sin(teta2))/ sin(teta1 - teta2));//x
		return rez;
		
	}

	void draw_it()
	{
		//
	}



	double getCircleEq(int x1, int y1, int x, int y)
	{
		return (x - x1) * (x - x1) + (y - y1) * (y - y1);
	}


	int ecDreapta(double xA, double yA, double xB, double yB, double x)
	{
		return (int)round((yB-yA) / (xB-xA) * (x - xA) + yA);
	}

	cv::Mat* getHoughPeaks(cv::Mat* img)
	{

		int nr_lines = 0;

		std::cout << "\h: " << img->rows;
		std::cout << "\w: " << img->cols;
		std::cout << std::endl;
		std::map<double, std::map<int, int>>* v;
		std::map<int, int>* lines;
		std::map<double, double>* P;

		unsigned char* imgData = (unsigned char*)img->data;
		
		cv::Mat* rez = new cv::Mat();
		cv::cvtColor(*img, *rez, cv::COLOR_GRAY2BGR);
		int ch = rez->channels();
		unsigned char* rezData = (unsigned char*)rez->data;
		


		int h = img->rows;
		int w = img->cols;
		double p;
		int Dmin =45;
		
		int Dmax=54;
		
		int Rmax = Dmax / 2;
		int Rmin = Dmin/2;
		int y_v;
		int x_v;
		double aux;

		double T0 = 10;
		double Tp = 5;
		double Tl = 5;
		double delta_tau, delta_p, delta_c, ma_c;
		double alfa, epsilon;
		

		int nr = 0;
		double delta_alfa, T_alfa = 10;
		int minL = 14;
		bool flag3;
		int pas = 1;


		double pmax = 98;

		for (int y = Rmax; y < h-Rmax; y++)
			for (int x = Rmax; x < w-Rmax ; x++)
			{
				v=new std::map<double, std::map<int, int>>;
				
				flag3 = false;
				
				for (int t = -Rmax; t <= Rmax; t++)
					for (int s = -Rmax; s <= Rmax; s++)
					{
						y_v = y + t;
						x_v = x+s;
						if (imgData[y_v * w + x_v])
						{
							aux = getCircleEq(x, y, x_v, y_v);
							if ( aux>= Rmin*Rmin&& aux<=Rmax*Rmax)
								for (int teta = 0; teta <= 180; teta++)
								{
									p = (double)x_v * cos(teta) + (double)y_v * sin(teta);
									if(p>=-pmax && p<=pmax)
										++(*v)[round(p)][teta];
								}
						}
					}



				if (v->size() >= 4)
				{
					lines = new std::map<int, int>;
					P = new std::map<double, double>;
					
					for (auto H = v->begin(); H != v->end(); H++)
					{
						(*lines)[H->first] = getMaxTeta(H->second);
						nr_lines++;
					
					}


					for (auto Hi = lines->begin(); Hi != --lines->end(); Hi++)
					{
						auto aux2 = Hi;
						++aux2;
						for (auto Hj = aux2; Hj != lines->end(); Hj++)
						{
							delta_tau = abs(Hi->second - Hj->second);
							delta_p = abs(Hi->first + Hj->first);
							delta_c = abs((*v)[Hi->first][Hi->second] - (*v)[Hj->first][Hj->second]);
							ma_c = ((*v)[Hi->first][Hi->second] + (*v)[Hj->first][Hj->second]) / 2;

							if (delta_tau < T0 && delta_p < Tp && delta_c < Tl * ma_c && (*v)[Hi->first][Hi->second] >= minL)
							{
								epsilon = abs(Hi->first - Hj->first) / 2.0;

								alfa = (Hi->second + Hj->second) / 2.0;

								(*P)[epsilon] = alfa;
							}
						}
					}

					
					
					if (P->size() >=2)
						for (auto Pk = P->begin(); Pk != --P->end(); Pk++)
						{
							auto aux2 = Pk;
							++aux2;

							for (auto Pl = aux2; Pl != P->end(); Pl++)
							{
								delta_alfa = abs(abs(Pk->second - Pl->second) - 90);
								if (delta_alfa < T_alfa)
								{
									int* Varfuri[4];
									//int* getIntLines(double p1, double teta1, double p2, double teta2)
									Varfuri[0] = getIntLines(-Pl->first, Pl->second, -Pk->first, Pk->second);
									Varfuri[1] = getIntLines(-Pl->first, Pl->second, Pk->first, Pk->second);
									Varfuri[2] = getIntLines(Pl->first, Pl->second, Pk->first, Pk->second);
									Varfuri[3] = getIntLines(Pl->first, Pl->second, -Pk->first, Pk->second);

									for (int i = 0; i < 4; i++)
									{
										Varfuri[i][0] += y;
										Varfuri[i][1] += x;
									}
									

									bool flag = true;

									for (int i = 0; i < 4; i++)
										if (Varfuri[i][0] >= h || Varfuri[i][0] < 0 || Varfuri[i][1] < 0 || Varfuri[i][1] >= w)
										{
											flag = false;
											break;
										}

									if (flag)
									{
										
										std::cout << "\nRectangle: ";


										std::cout << "\nUnghi: " << Pk->second;
										std::cout << "\nv1: (y: " << Varfuri[0][0] << ",x: " << Varfuri[0][1] << ")";
										std::cout << "\nv2: (y: " << Varfuri[1][0] << ",x: " << Varfuri[1][1] << ")";
										std::cout << "\nv3: (y: " << Varfuri[2][0] << ",x: " << Varfuri[2][1] << ")";
										std::cout << "\nv4: (y: " << Varfuri[3][0] << ",x: " << Varfuri[3][1] << ")\n\n";


										for (int i = 0; i < 4; i++)
										{
											int startX = Varfuri[i][1];
											int stopX = Varfuri[(i + 1) % 4][1];
											int startY = Varfuri[i][0];
											int stopY = Varfuri[(i + 1) % 4][0];
											if (startX > stopX)
											{
												int aux = startX;
												startX = stopX;
												stopX = aux;

												aux = startY;
												startY = stopY;
												stopY = aux;
											}

											for (int x_r = startX; x_r <= stopX; x_r++)
											{
												int aux4 = ecDreapta(startX, startY, stopX, stopY, x_r);
												rezData[(aux4 * w + x_r) * ch] = 0;
												rezData[(aux4 * w + x_r) * ch + 1] = 0;
												rezData[(aux4 * w + x_r) * ch + 2] = 255;
											}


										}


									}







									for (int i = 0; i < 4; i++)
										delete[] Varfuri[i];


								}
							}
						}
					delete P;
					delete lines;
				}
					
				delete v;
				
			}
			std::cout << "\nNr_lines: " << nr_lines << "\n";
			return rez;
		}