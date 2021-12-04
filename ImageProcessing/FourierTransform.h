#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <complex>
class FourierTransform
{
public:
	FourierTransform()
	{
		pointer = 0;
	}
	~FourierTransform()
	{
		delete pointer;
		pointer = 0;
	}
	const std::complex<double>*  DiscreteFourierTransform(int* image, int size)
	{
		std::cout << "Enter" << '\n';
		if (pointer != 0)
		{
			delete pointer;
		}
		pointer = new std::complex<double>[size];
		for (int i = 0; i < size; i++)
		{
			pointer[i] = SubDiscreteFourierTransform(image, i, size);
		}
		return pointer;
	}
private:
	std::complex<double>* pointer = 0;
	std::complex<double>  SubDiscreteFourierTransform(int *image, int index, int size)
	{
		std::complex<double> sum = 0;
		const std::complex<double> I(0, 1);
		const std::complex<double> E(M_E, 0);
		const std::complex<double> PI2(M_PI * 2, 0);

		const std::complex<double> INDEX(index, 0);

		for (int i = 0; i < size; i++)
		{
			const std::complex<double> nOfN((double(i) / double(size)), 0);
			std::complex<double> DATA(image[i], 0);
			sum += (DATA * std::complex<double>(pow(E, -I*INDEX *PI2*nOfN)));
		}
		return sum;
	}
};
class FourierTransform2D
{
public:

	void  DiscreteFourierTransform2D(std::complex<double>** output, std::complex<double>** input, int width, int height)
	{
		for (int h = 0; h < height; h++)
		{
			for (int w = 0; w < width; w++)
			{
				output[h][w] = commonBase2D(input, width, height, w, h, FT);
			}
		}
	}
	void  DiscreteFourierTransformTwo1D(std::complex<double>** output, std::complex<double>** input, int width, int height)
	{
		std::complex<double>** temp = new std::complex<double>*[height];
		for (int i = 0; i < height; i++)
		{
			temp[i] = new std::complex<double>[width];
		}

		for (int h = 0; h < height; h++)
		{
			for (int w = 0; w < width; w++)
			{

				temp[h][w] = commonBase1D(input, width, height, w, h, FT, true);
				//printf("(%d,%d)\n", w, h);
			}
			//printf("\n");
		}
		//printf("\n");
		for (int h = 0; h < height; h++)
		{
			for (int w = 0; w < width; w++)
			{
				output[h][w] = commonBase1D(temp, width, height, w, h, FT, false);
				//printf("(%d,%d)\n", w, h);
				//	std::cout << output[h][w];
			}
			//	printf("\n");
		}
		for (int i = 0; i < height; i++)
		{
			delete temp[i];
		}
		delete temp;
	}
	void  DiscreteInverseFourierTransformTwo1D(std::complex<double>** output, std::complex<double>** input, int width, int height)
	{
		std::complex<double>** temp = new std::complex<double>*[height];
		for (int i = 0; i < height; i++)
		{
			temp[i] = new std::complex<double>[width];
		}

		for (int h = 0; h < height; h++)
		{
			for (int w = 0; w < width; w++)
			{

				temp[h][w] = commonBase1D(input, width, height, w, h, IFT, true);
				//printf("(%d,%d)\n", w, h);
			}
			//printf("\n");
		}
		//printf("\n");
		for (int h = 0; h < height; h++)
		{
			for (int w = 0; w < width; w++)
			{
				output[h][w] = commonBase1D(temp, width, height, w, h, IFT, false);
				//printf("(%d,%d)\n", w, h);
				//	std::cout << output[h][w];
			}
			//	printf("\n");
		}
		for (int i = 0; i < height; i++)
		{
			delete temp[i];
		}
		delete temp;
	}
	void  DiscreteInvesereFourierTransform2D(std::complex<double>** output, std::complex<double>** input, int width, int height)
	{
		for (int h = 0; h < height; h++)
		{
			for (int w = 0; w < width; w++)
			{
				output[h][w] = commonBase2D(input, width, height, w, h, IFT);
			}
		}
	}

	enum METHOD { FT = -1, IFT = 1 };

private:
	std::complex<double>   commonBase1D(std::complex<double>** input, int width, int height, int x, int y, METHOD method, bool countRow)
	{
		int size;
		std::complex<double> sum = 0;
		const std::complex<double> I(0, 1);
		const std::complex<double> E(M_E, 0);
		const std::complex<double> PI2(M_PI * 2, 0);
		int countIndex, bound, startX, startY, incX, incY;
		if (countRow)
		{
			size = width;
			countIndex = x;
			bound = width;
			startX = 0;
			startY = y;
			incX = 1;
			incY = 0;
		}
		else
		{
			size = height;
			countIndex = y;
			bound = height;
			startX = x;
			startY = 0;
			incX = 0;
			incY = 1;
		}
		std::complex<double> INDEX(countIndex, 0);

		for (int i = 0; i < bound; i++)
		{
			std::complex<double> nOfN((double(i) / double(bound)), 0);
			std::complex<double> data = input[startY + incY*i][startX + incX*i];
			std::complex<double> coeff = pow(E, std::complex<double>(method, 0)*I*PI2*INDEX*nOfN);
			sum += data*coeff;
		}
		if (method == FT)
		{
			return sum;
		}
		else
		{
			return sum / std::complex<double>(size, 0);
		}
	}
	std::complex<double>  SubDiscreteFourierTransform(int *image, int index, int size)
	{
		std::complex<double> sum = 0;
		const std::complex<double> I(0, 1);
		const std::complex<double> E(M_E, 0);
		const std::complex<double> PI2(M_PI * 2, 0);

		const std::complex<double> INDEX(index, 0);

		for (int i = 0; i < size; i++)
		{
			const std::complex<double> nOfN((double(i) / double(size)), 0);
			std::complex<double> DATA(image[i], 0);
			sum += (DATA * std::complex<double>(pow(E, -I*INDEX *PI2*nOfN)));
		}
		return sum;
	}

	std::complex<double>  commonBase2D(std::complex<double>** input, int width, int height, int x, int y, METHOD method)
	{
		int size = width*height;
		std::complex<double> sum = 0;
		const std::complex<double> I(0, 1);
		const std::complex<double> E(M_E, 0);
		const std::complex<double> PI2(M_PI * 2, 0);
		const std::complex<double> INDEX_X(x, 0);
		const std::complex<double> INDEX_Y(y, 0);
		for (int h = 0; h < height; h++)
		{
			for (int w = 0; w < width; w++)
			{
				const std::complex<double> nOfNW((double(w) / double(width)), 0);
				const std::complex<double> nOfNH((double(h) / double(height)), 0);
				sum += (input[h][w] * std::complex<double>(pow(E, std::complex<double>(method, 0)*I*PI2*(INDEX_X*nOfNW + INDEX_Y *nOfNH))));
			}
		}
		if (method == FT)
		{
			return sum;
		}
		else
		{
			return sum / std::complex<double>(size, 0);
		}
	}

};
