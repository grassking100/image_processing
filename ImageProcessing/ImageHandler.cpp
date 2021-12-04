#pragma once

#include "stdafx.h"
#include "ImageHandler.h"
#include <Windows.h>
#include <conio.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include "FourierTransform.h"
#include <cmath>
#include <iostream>
#include <complex>
int linear(int x1, int y1, int x2, int y2, int targetX)
{
	return (targetX - x1)*(y2 - y1) / (x2 - x1) + y1;
}
int bilinear(int x11, int y11, int num11, int x12, int y12, int num12, int x21, int y21, int num21, int x22, int y22, int num22, int targetX, int targetY)
{
	int top = linear(x11, num11, x12, num12, targetX);
	int down = linear(x21, num21, x22, num22, targetX);
	return linear(y11, top, y21, down, targetY);
}
int colorValidate(int color, int max)
{
	if (0 > color)
	{
		return 0;
	}
	else if (color > max)
	{
		return max;
	}
	return color;
}
CImage * downScale(CImage * input, int scale)
{
	CImage *newImage = new CImage();
	int width = input->GetWidth();
	int height = input->GetHeight();
	int newWidth = width / scale;
	int newHeight = height / scale;
	newImage->Create(newWidth, newHeight, input->GetBPP());
	for (int i = 0; i < newHeight; i++)
	{
		for (int j = 0; j < newWidth; j++)
		{
			newImage->SetPixel(j, i, input->GetPixel(j*scale, i*scale));
		}
	}
	return newImage;
}
CImage * upScale(CImage * input, int scale)
{
	CImage *newImage = new CImage();
	if (newImage != 0)
	{
		int width = input->GetWidth();
		int height = input->GetHeight();
		newImage->Create(width*scale, height*scale, input->GetBPP());

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				newImage->SetPixel(j*scale, i*scale, input->GetPixel(j, i));
			}
		}
		int newWidth = width * scale;
		int newHeight = height * scale;
		//initialzie along row;
		int interval = scale;
		for (int i = 0; i < newHeight; i += scale)
		{
			for (int j = 0; j < newWidth; j += 1)
			{
				if (j % scale != 0)
				{
					int leftX = j - j%scale, rightX = leftX + scale;
					int color[2][3];
					int r = 0, g = 1, b = 2;
					for (int smallWidth = leftX; smallWidth <= rightX; smallWidth += scale)
					{
						int tempX = (smallWidth - leftX) / scale;
						if (smallWidth < newWidth)
						{
							color[tempX][r] = (int)GetRValue(newImage->GetPixel(smallWidth, i));
							color[tempX][g] = (int)GetGValue(newImage->GetPixel(smallWidth, i));
							color[tempX][b] = (int)GetBValue(newImage->GetPixel(smallWidth, i));
						}
						else
						{
							color[tempX][r] = (int)GetRValue(newImage->GetPixel(newWidth - scale, i));
							color[tempX][g] = (int)GetGValue(newImage->GetPixel(newWidth - scale, i));
							color[tempX][b] = (int)GetBValue(newImage->GetPixel(newWidth - scale, i));
						}

					}
					int newColor[3];
					for (int colorIndex = 0; colorIndex < 3; colorIndex++)
					{
						newColor[colorIndex] = linear(leftX, color[0][colorIndex], rightX, color[1][colorIndex], j);
					}
					newImage->SetPixelRGB(j, i, colorValidate(newColor[r], 255), colorValidate(newColor[g], 255), colorValidate(newColor[b], 255));
				}
			}
		}
		for (int j = 0; j < newWidth; j += (scale))
		{
			for (int i = 0; i < newHeight; i += 1)
			{
				if (i %scale != 0)
				{
					int upY = i - i%scale, downY = upY + scale;
					int color[2][3];
					int r = 0, g = 1, b = 2;
					for (int smallHeight = upY; smallHeight <= downY; smallHeight += scale)
					{
						int tempY = (smallHeight - upY) / scale;
						if (smallHeight < newHeight)
						{
							color[tempY][r] = (int)GetRValue(newImage->GetPixel(j, smallHeight));
							color[tempY][g] = (int)GetGValue(newImage->GetPixel(j, smallHeight));
							color[tempY][b] = (int)GetBValue(newImage->GetPixel(j, smallHeight));
						}
						else {
							color[tempY][r] = (int)GetRValue(newImage->GetPixel(j, newHeight - scale));
							color[tempY][g] = (int)GetGValue(newImage->GetPixel(j, newHeight - scale));
							color[tempY][b] = (int)GetBValue(newImage->GetPixel(j, newHeight - scale));
						}
					}

					int newColor[3];
					for (int colorIndex = 0; colorIndex < 3; colorIndex++)
					{
						newColor[colorIndex] = linear(upY, color[0][colorIndex], downY, color[1][colorIndex], i);
					}
					newImage->SetPixelRGB(j, i, colorValidate(newColor[r], 255), colorValidate(newColor[g], 255), colorValidate(newColor[b], 255));
				}
			}
		}
		for (int j = 0; j < newWidth; j += 1)
		{
			for (int i = 0; i < newHeight; i += 1)
			{
				if ((i %scale) != 0 && (j % scale) != 0)
				{
					int upY = i - i%scale, downY = upY + scale;
					int leftX = j - j%scale, rightX = leftX + scale;
					int r = 0, g = 1, b = 2;
					int color[2][2][3];
					for (int smallHeight = upY; smallHeight <= downY; smallHeight += scale)
					{
						for (int smallWidth = leftX; smallWidth <= rightX; smallWidth += scale)
						{
							int tempY = (smallHeight - upY) / scale;
							int tempX = (smallWidth - leftX) / scale;
							int tempWidth = smallWidth < newWidth ? smallWidth : newWidth - scale;
							int tempHeight = smallHeight < newHeight ? smallHeight : newHeight - scale;
							color[tempY][tempX][r] = (int)GetRValue(newImage->GetPixel(tempWidth, tempHeight));
							color[tempY][tempX][g] = (int)GetGValue(newImage->GetPixel(tempWidth, tempHeight));
							color[tempY][tempX][b] = (int)GetBValue(newImage->GetPixel(tempWidth, tempHeight));
						}
					}


					int newColor[3];
					for (int colorIndex = 0; colorIndex < 3; colorIndex++)
					{
						newColor[colorIndex] = bilinear(
							leftX, upY, color[0][0][colorIndex],
							rightX, upY, color[0][1][colorIndex],
							leftX, downY, color[1][0][colorIndex],
							rightX, downY, color[1][1][colorIndex],
							j, i);
					}
					newImage->SetPixelRGB(j, i,
						colorValidate(newColor[r], 255),
						colorValidate(newColor[g], 255),
						colorValidate(newColor[b], 255));

				}
			}
		}
	}
	return newImage;
}
CImage * resize(CImage * input, float scale)
{
	if (scale > 1)
	{
		return upScale(input, (int)scale);
	}
	else if (scale < 1)
	{
		return downScale(input, (int)(1 / scale));
	}
	return 0;
}
bool CImageCopy(CImage * dst, const CImage * src)
{
	if (dst != 0 && src != 0 && dst != src)
	{
		int width = src->GetWidth();
		int height = src->GetHeight();

		return CImageCopy(dst, src, width, height, true);
	}
	return false;
}
bool setBlankDefault(CImage * image, int width, int height, int bpp)
{
	if (image != 0)
	{
		if (!image->IsNull())
		{
			image->Destroy();
		}
		image->Create(width, height, bpp, NULL);
		return true;
	}
	return false;
}
bool CImageCopy(CImage * dst, const CImage * src, int width, int height, bool reCreateDestination)
{
	if (dst != 0 && src != 0 && dst != src)
	{
		if (reCreateDestination)
		{
			if (!dst->IsNull())
			{
				dst->Destroy();
			}
			dst->Create(width, height, 32);
		}
		StretchBlt(dst->GetDC(), 0, 0, width, height, src->GetDC(), 0, 0, src->GetWidth(), src->GetHeight(), SRCCOPY);
		dst->ReleaseDC();
		src->ReleaseDC();
		return true;
	}
	return false;
}
bool translation(CImage * dst, const CImage * src, int x, int y)
{
	if (dst != 0 && src != 0 && !src->IsNull())
	{
		int oldWidth = src->GetWidth();
		int oldHeight = src->GetHeight();
		int oldBPP = src->GetBPP();
		if (dst->Create(oldWidth, oldHeight, oldBPP))
		{
			int newHeight = dst->GetHeight();
			for (int h = 0; h < newHeight; h++)
			{
				int startX = x >= 0 ? 0 : -x;
				int startY = y >= 0 ? h : h - y;
				int endX = x >= 0 ? x : 0;
				int endY = y >= 0 ? y + h : h;
				if (startX < oldWidth && endX < oldWidth && startY < oldHeight && endY < oldHeight)
				{
					memcpy(dst->GetPixelAddress(endX, endY), src->GetPixelAddress(startX, startY), (oldBPP)*(oldWidth - abs(x)) / 8);
				}
			}
			return true;
		}
	}
	return false;
}
bool rotate(CImage * dst, const CImage  *src, int degree)
{
	if (dst != 0 && src != 0 && !src->IsNull())
	{
		double radian = (M_PI*(degree % 360) / 180);
		int oldHeight = src->GetHeight();
		int oldWidth = src->GetWidth();
		int oldBpp = src->GetBPP();
		int  newWidth = (int)(abs(oldWidth* cos(radian)) + abs(oldHeight*sin(radian)));
		int  newHeight = (int)(abs(oldHeight*cos(radian)) + abs(oldWidth*sin(radian)));
		if (dst->Create(newWidth, newHeight, oldBpp))
		{
			double sinConstant;
			double cosConstant;
			sinConstant = sin(-radian);
			cosConstant = cos(-radian);
			for (int h = 0; h < newHeight; h++)
			{
				for (int w = 0; w < newWidth; w++)
				{
					int newW = (int)((w - newWidth / 2)*cosConstant - (h - newHeight / 2)*sinConstant);
					int newH = (int)((w - newWidth / 2)*sinConstant + (h - newHeight / 2)*cosConstant);
					if (isValidPosition(newW + oldWidth / 2, newH + oldHeight / 2, oldWidth, oldHeight))
					{
						dst->SetPixel(w, h, src->GetPixel(newW + oldWidth / 2, newH + oldHeight / 2));
					}
				}
			}
			return true;
		}
	}
	return false;
}
bool isValidPosition(int x, int y, int widthMax, int heightMax)
{
	return x >= 0 && widthMax > x && y >= 0 && heightMax > y;
}
bool grayIntensityHistogram(CImage * dst, const CImage *src)
{
	if (dst != 0 && src != 0 && !src->IsNull())
	{
		const int height = 256;
		const int width = 512;
		const int axisWidth = 5;
		const int windowHeight = height + axisWidth;
		const int windowWidth = width + axisWidth;
		int bpp = 32;
		if (dst->Create(windowWidth, windowHeight, bpp))
		{
			int intensity[width] = { 0 };
			int src_height = src->GetHeight();
			int src_width = src->GetWidth();
			for (int h = 0; h < src_height; h++)
			{
				for (int w = 0; w < src_width; w++)
				{
					int color = src->GetPixel(w, h);
					int gray = GetRValue(color);
					intensity[gray]++;
				}
			}
			int max = 0;
			for (int w = 0; w < width; w++)
			{
				if (max < intensity[w])
				{
					max = intensity[w];
				}
			}
			int hist[windowHeight][windowWidth];
			int white = RGB(255, 255, 255);
			for (int w = 0; w < windowWidth; w++)
			{
				for (int h = 0; h < windowHeight; h++)
				{
					hist[h][w] = white;
				}
			}
			if (max > 0)
			{
				for (int w = 0; w < width; w += 1)
				{
					int subHeight = (intensity[w / 2] * height) / max;
					for (int h = 0; h < subHeight; h++)
					{
						hist[height - h][w + axisWidth] = 0;
					}
				}
			}

			for (int h = 0; h <= height; h++)
			{
				int ratio = (height - h) * 255 / (height);
				for (int w = 0; w < axisWidth; w++)
				{
					hist[h][w] = RGB(ratio, ratio, ratio);
				}
			}
			for (int w = 0; w < width; w++)
			{
				for (int h = 0; h < axisWidth; h++)
				{
					hist[windowHeight - 1 - h][w + axisWidth] = RGB(w / 2, w / 2, w / 2);
				}
			}
			for (int h = 0; h < windowHeight; h++)
			{
				memcpy(dst->GetPixelAddress(0, h), hist[h], sizeof(int)*windowWidth);
			}
			return true;
		}
	}
	return false;
}
bool toGray(CImage * dst, const CImage *src)
{
	if (dst != 0 && src != 0 && !src->IsNull())
	{
		int height = src->GetHeight();
		int width = src->GetWidth();
		int bpp = src->GetBPP();
		ImageContainer container(width,height);
		if (dst->Create(width, height, bpp) && container.isInitialize() && fromCImage(src,container))
		{
			container.toGray();
			toCImage(dst,container);
			return true;
		}
	}
	return false;
}
bool filter(CImage * dst, const CImage *src, double* mask, int maskWidth, int maskHeight)
{
	if (dst != 0 && src != 0 && !src->IsNull())
	{
		int height = src->GetHeight();
		int width = src->GetWidth();
		int bpp = src->GetBPP();
		ImageContainer container(width, height);
		if (dst->Create(width, height, bpp) && container.isInitialize() && fromCImage(src, container))
		{
			ImageContainer * temp=container.filter(mask, maskWidth,  maskHeight,true);
			if (temp != 0)
			{
				toCImage(dst, *temp);
				delete temp;
				return true;
			}
		}
	}
	return false;
}
bool downScale(CImage *  dst, const CImage * src, int scale)
{
	if (dst != 0 && src != 0 && !src->IsNull())
	{
		int width = src->GetWidth();
		int height = src->GetHeight();
		int newWidth = width / scale;
		int newHeight = height / scale;
		int bpp = src->GetBPP();
		ImageContainer container(width, height);
		if (dst->Create(newWidth, newHeight, bpp) && container.isInitialize())
		{
			fromCImage(src, container);
			ImageContainer* temp = container.downScale(scale);
			if (temp != 0)
			{
				toCImage(dst, *temp);
				delete temp;
				return true;
			}
		}
	}
	return false;
}
bool upScale(CImage *  dst, const CImage * src, int scale)
{
	if (dst != 0 && src != 0 && !src->IsNull())
	{
		int width = src->GetWidth();
		int height = src->GetHeight();
		int newWidth = width * scale;
		int newHeight = height * scale;
		int bpp = src->GetBPP();
		ImageContainer container(width, height);
		if (dst->Create(newWidth, newHeight, bpp) && container.isInitialize())
		{
			fromCImage(src, container);
			ImageContainer* temp = container.upScale(scale);
			if (temp != 0)
			{
				toCImage(dst, *temp);
				delete temp;
				return true;
			}
		}
	}
	return false;
}
bool fourierTransform(CImage *  dst, const CImage * src)
{

	if (dst != 0 && src != 0 && !src->IsNull())
	{
		int width = src->GetWidth();
		int height = src->GetHeight();
		int bpp = src->GetBPP();
		if (dst->Create(width, height, bpp))
		{
			FourierTransform2D f;
			std::complex<double>** input = new std::complex<double>*[height];
			std::complex<double>** output = new std::complex<double>*[height];
			double** rescaleValue = new double*[height];
			for (int i = 0; i < height; i++)
			{
				input[i] = new std::complex<double>[width];
				output[i] = new std::complex<double>[width];
				rescaleValue[i] = new double[width];
			}
			for (int h = 0; h < height; h++)
			{
				for (int w = 0; w < width; w++)
				{
					input[h][w] = std::complex<double>(GetRValue(src->GetPixel(w, h)), 0);
				}
			}
			f.DiscreteFourierTransformTwo1D(output, input, width, height);

			double max = 0;
			for (int h = 0; h < height; h++)
			{
				for (int w = 0; w < width; w++)
				{
					std::complex<double> fComplex = output[h][w];
					double temp = log(pow(pow(fComplex.real(), 2) + pow(fComplex.imag(), 2), 0.5));
					if (temp > max)
					{
						max = temp;
					}
					rescaleValue[h][w] = temp;
				}
			}
			for (int h = 0; h < height; h++)
			{
				for (int w = 0; w < width; w++)
				{

					int temp = (int)(255 * rescaleValue[(h + height / 2 + 1) % height][(w + width / 2 + 1) % width] / max);
					dst->SetPixel(w, h, RGB(temp, temp, temp));

				}

			}
			for (int i = 0; i < height; i++)
			{
				delete input[i];
				delete output[i];
				delete rescaleValue[i];
			}
			delete output;
			delete input;
			delete rescaleValue;

			return true;
		}
	}
	return false;
}
bool gammaCorrection(CImage *  dst, const CImage * src, double gammaValue)
{
	if (dst != 0 && src != 0 && !src->IsNull())
	{
		int width = src->GetWidth();
		int height = src->GetHeight();
		int bpp = src->GetBPP();
		if (dst->Create(width, height, bpp))
		{
			for (int h = 0; h < height; h++)
			{
				for (int w = 0; w < width; w++)
				{
					COLORREF col = src->GetPixel(w, h);
					int newRValue = (int)(255 * pow((double)GetRValue(col) / (double)255, gammaValue));
					int newGValue = (int)(255 * pow((double)GetGValue(col) / (double)255, gammaValue));
					int newBValue = (int)(255 * pow((double)GetBValue(col) / (double)255, gammaValue));
					dst->SetPixel(w, h, RGB(newRValue, newGValue, newBValue));
				}
			}
			return true;
		}
	}
	return false;
}
bool wave(CImage *  dst, int width, int height, double coeff, double hor_shoft, double ver_shift, double theta_coeff)
{
	if (dst != 0)
	{
		if (dst->Create(width, height, 32))
		{
			for (int h = 0; h < height; h++)
			{
				for (int w = 0; w < width; w++)
				{
					double value = coeff*sin(theta_coeff*w + hor_shoft);
					double temp = abs(value);
					if (temp > 255)
					{
						temp = 255;
					}
					dst->SetPixel(w, h, RGB(temp, temp, temp));
				}
			}
			return true;
		}
	}
	return false;
}
bool waveFFT(CImage *  wave, CImage *  fft, int width, int height, double coeff, double hor_shoft, double ver_shift, double theta_coeff)
{
	if (fft != 0 && wave != 0)
	{
		if (fft->Create(width, height, 32) && wave->Create(width, height, 32))
		{
			std::complex<double>** input = new std::complex<double>*[height];
			std::complex<double>** output = new std::complex<double>*[height];
			double** rescaleValue = new double*[height];
			for (int i = 0; i < height; i++)
			{
				input[i] = new std::complex<double>[width];
				output[i] = new std::complex<double>[width];
				rescaleValue[i] = new double[width];
			}
			for (int h = 0; h < height; h++)
			{
				for (int w = 0; w < width; w++)
				{
					double value = coeff*sin(theta_coeff*w + hor_shoft);
					double temp = abs(value);
					if (temp > 255)
					{
						temp = 255;
					}
					wave->SetPixel(w, h, RGB(temp, temp, temp));
					input[h][w] = std::complex<double>(value, 0);
				}
			}
			FourierTransform2D f;
			f.DiscreteFourierTransformTwo1D(output, input, width, height);
			double max = 0;
			for (int h = 0; h < height; h++)
			{
				for (int w = 0; w < width; w++)
				{
					std::complex<double> fComplex = output[h][w];
					double temp = pow(pow(fComplex.real(), 2) + pow(fComplex.imag(), 2), 0.5);
					if (temp > max)
					{
						max = temp;
					}
					rescaleValue[h][w] = temp;

				}
			}
			for (int h = 0; h < height; h++)
			{
				for (int w = 0; w < width; w++)
				{
					int temp = (int)(255 * rescaleValue[(h + height / 2 + 1) % height][(w + width / 2 + 1) % width] / max);
					fft->SetPixel(w, h, RGB(temp, temp, temp));
				}
			}
			for (int i = 0; i < height; i++)
			{
				delete input[i];
				delete output[i];
				delete rescaleValue[i];
			}
			delete output;
			delete input;
			delete rescaleValue;

			return true;
		}
	}
	return false;
}
bool inverseFourierTransform(CImage *  dst, const CImage * src)
{

	if (dst != 0 && src != 0 && !src->IsNull())
	{
		int width = src->GetWidth();
		int height = src->GetHeight();
		int bpp = src->GetBPP();
		if (dst->Create(width, height, bpp))
		{
			FourierTransform2D f;
			std::complex<double>** input = new std::complex<double>*[height];
			std::complex<double>** output = new std::complex<double>*[height];
			std::complex<double>** temp = new std::complex<double>*[height];
			double** rescaleValue = new double*[height];
			for (int i = 0; i < height; i++)
			{
				input[i] = new std::complex<double>[width];
				temp[i] = new std::complex<double>[width];
				output[i] = new std::complex<double>[width];
				rescaleValue[i] = new double[width];
			}
			for (int h = 0; h < height; h++)
			{
				for (int w = 0; w < width; w++)
				{
					input[h][w] = std::complex<double>(GetRValue(src->GetPixel(w, h)), 0);
				}
			}
			f.DiscreteFourierTransformTwo1D(temp, input, width, height);
			for (int h = -height / 20; h <= height / 20; h++)
			{
				for (int w = -width / 20; w <= width / 20; w++)
				{
					temp[(h + height) % height][(width + w) % width] = 0;
				}
			}
			f.DiscreteInverseFourierTransformTwo1D(output, temp, width, height);
			double max = 0;
			for (int h = 0; h < height; h++)
			{
				for (int w = 0; w < width; w++)
				{
					std::complex<double> fComplex = output[h][w];
					double temp = pow(pow(fComplex.real(), 2) + pow(fComplex.imag(), 2), 0.5);
					if (temp > max)
					{
						max = temp;
					}
					rescaleValue[h][w] = temp;
				}
			}
			for (int h = 0; h < height; h++)
			{
				for (int w = 0; w < width; w++)
				{

					int temp = (int)(255 * rescaleValue[h][w] / max);
					dst->SetPixel(w, h, RGB(temp, temp, temp));

				}

			}
			for (int i = 0; i < height; i++)
			{
				delete input[i];
				delete temp[i];
				delete output[i];
				delete rescaleValue[i];
			}
			delete temp;
			delete output;
			delete input;
			delete rescaleValue;

			return true;
		}
	}
	return false;
}
bool toCImage(CImage * image, const ImageContainer &container)
{
	int width = container.getWidth();
	int height = container.getHeight();
	if (image != 0 && !image->IsNull() && image->GetWidth() == width && image->GetHeight() == height)
	{
		for (int h = 0; h < height; h++)
		{
			for (int w = 0; w < width; w++)
			{
				image->SetPixel(w, h, container.getColor(w, h));
			}
		}
		return true;
	}
	return false;
}
bool fromCImage(const CImage * image, ImageContainer & container)
{
	if (image != 0 && !image->IsNull())
	{
		int width = image->GetWidth();
		int height = image->GetHeight();
		int bpp = image->GetBPP();
		container.~ImageContainer();
		container.initialize(width, height);
		if (container.isInitialize())
		{
			for (int h = 0; h < height; h++)
			{
				for (int w = 0; w < width; w++)
				{
					container.setColor(w, h, image->GetPixel(w, h));
				}
			}
			return true;
		}
	}
	return false;
}
bool toSubColor(CImage *  dst, const CImage * src, int range, COLORREF selectedColor)
{

	if (dst != 0 && src != 0 && !src->IsNull())
	{
		int width = src->GetWidth();
		int height = src->GetHeight();
		int bpp = src->GetBPP();
		ImageContainer container(width, height);
		if (dst->Create(width, height, bpp) && container.isInitialize())
		{
			fromCImage(src, container);
			ImageContainer *subColors=container.getSubColor(selectedColor, range);
			if (subColors != 0)
			{
				toCImage(dst, *subColors);
				delete subColors;
				return true;
			}
		}
	}
	return false;
}
bool toonShading(CImage *  dst, const CImage * src)
{
	if (dst != 0 && src != 0 && !src->IsNull())
	{
		int width = src->GetWidth();
		int height = src->GetHeight();
		int bpp = src->GetBPP();
		ImageContainer container(width, height);
		if (dst->Create(width, height, bpp) && container.isInitialize())
		{
			fromCImage(src, container);
			ImageContainer *toon = container.toonShading();
			if (toon != 0)
			{
				toCImage(dst, *toon);
				delete toon;
				return true;
			}
			return true;
		}
	}
	return false;
}