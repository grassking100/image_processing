#include "stdafx.h"
#include "ImageContainer.h"

ImageContainer::ImageContainer(int width, int height)
{
	initialize(width, height);
}
void ImageContainer::initialize(int width, int height)
{
	rgb = 0;
	this->init = false;
	if (width > 0 && height > 0)
	{
		this->width = width;
		this->height = height;
		this->imageType = COLOR;
		this->rgb = new DWORD[width*height];
		if (rgb != 0)
		{
			this->init = true;
		}
	}
}
bool ImageContainer::isInitialize()const
{
	return init;
}

DWORD  ImageContainer::getColor(int x, int y)const
{
	int index = this->getIndex(x, y);
	if (index != -1)
	{
		return this->rgb[index];
	}
	return 0;
}

void ImageContainer::setColor(int x, int y, DWORD  color)
{
	int index = this->getIndex(x, y);
	if (index != -1)
	{
		this->rgb[index] = color;
	}
}

int ImageContainer::getHeight()const
{
	return height;
}

int ImageContainer::getWidth()const
{
	return width;
}

ImageContainer::ImageType ImageContainer::getImageType()const
{
	return imageType;
}

void ImageContainer::toGray()
{
	if (this->imageType != GRAY)
	{
		int size = this->getSize();
		for (int i = 0; i < size; i++)
		{
			DWORD color = this->rgb[i];

			this->rgb[i] = ImageContainer::getGrayColor(color);
		}
		this->imageType = GRAY;
	}
}

ImageContainer* ImageContainer::copy()const
{
	ImageContainer *temp = new ImageContainer(width, height);
	if (temp != 0 && temp->isInitialize())
	{
		int size = this->getSize();
		for (int i = 0; i < size; i++)
		{
			temp->rgb[i] = this->rgb[i];
		}
		temp->imageType = this->imageType;
		return temp;
	}
	else
	{
		delete temp;
		return 0;
	}


}

ImageContainer* ImageContainer::upScale(double scale)const
{
	ImageContainer *rescaleImage = new ImageContainer((int)(width*scale), (int)(height*scale));
	if (rescaleImage != 0 && rescaleImage->isInitialize())
	{
		for (int h = 0; h < height; h++)
		{
			for (int w = 0; w < width; w++)
			{
				COLORREF col = getColor(w, h);
				for (int subW = 0; subW < scale; subW++)
				{
					for (int subH = 0; subH < scale; subH++)
					{
						rescaleImage->setColor((int)(w*scale) + subW, (int)(h*scale) + subH, col);
					}
				}
			}
		}
		return rescaleImage;
	}
	else
	{
		delete rescaleImage;
		return 0;
	}
}



int ImageContainer::getSize()const
{
	return width*height;
}

void ImageContainer::initialize()
{
	rgb = 0;
	this->init = false;
	this->imageType = UNDEFINE;
	width = height = 0;
}

double ImageContainer::distance(DWORD lhs, DWORD rhs)
{
	double redDistance = pow(GetRValue(lhs) - GetRValue(rhs), 2);
	double greenDistance = pow(GetGValue(lhs) - GetGValue(rhs), 2);
	double blueDistance = pow(GetBValue(lhs) - GetBValue(rhs), 2);
	return pow(blueDistance + greenDistance + redDistance, 0.5);
}

ImageContainer * ImageContainer::downScale(double scale)const
{
	int newWidth = (int)(width / scale);
	int newHeight = (int)(height / scale);
	ImageContainer *rescaleImage = new ImageContainer(newWidth, newHeight);
	if (rescaleImage != 0 && rescaleImage->isInitialize())
	{
		for (int i = 0; i < newHeight; i++)
		{
			for (int j = 0; j < newWidth; j++)
			{
				rescaleImage->setColor(j, i, this->getColor((int)(j*scale), (int)(i*scale)));
			}
		}
		return rescaleImage;
	}
	else
	{
		delete rescaleImage;
		return 0;
	}
}

ImageContainer * ImageContainer::getSubColor(DWORD color, int range)const
{
	ImageContainer *rescaleImage = new ImageContainer(width, height);
	if (rescaleImage != 0 && rescaleImage->isInitialize())
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				DWORD targetColor = this->getColor(j, i);
				double distance = ImageContainer::distance(color, targetColor);
				DWORD newColor;
				if (distance <= range)
				{
					newColor = targetColor;
				}
				else
				{
					newColor = ImageContainer::getGrayColor(targetColor);
				}
				rescaleImage->setColor(j, i, newColor);
			}
		}
		return rescaleImage;
	}
	else
	{
		delete rescaleImage;
		return 0;
	}
}

DWORD ImageContainer::getGrayColor(DWORD color)const
{
	int green = GetGValue(color), red = GetRValue(color), blue = GetBValue(color);
	int gray = (green + red + blue) / 3;
	return RGB(gray, gray, gray);
}
void ImageContainer::subFilter(double * mask, int maskWidth, int maskHeight, int nowW, int nowH, double& red, double&green, double&blue)const
{
	int halfHeight = maskHeight / 2;
	int halfWidth = maskWidth / 2;
	double sumRed = 0, sumGreen = 0, sumBlue = 0;
	for (int sub_h = -halfHeight; sub_h <= halfHeight; sub_h++)
	{
		for (int sub_w = -halfWidth; sub_w <= halfWidth; sub_w++)
		{
			int tempW = nowW + sub_w;
			int tempH = nowH + sub_h;
			int indexOfMask = (sub_h + halfHeight)*maskWidth + (sub_w + halfWidth);
			if (0 <= tempW && tempW < width && 0 <= tempH && tempH < height)
			{
				int color = this->getColor(tempW, tempH);
				int _red = GetRValue(color);
				int _blue = GetBValue(color);
				int _green = GetGValue(color);
				sumRed += (_red*mask[indexOfMask]);
				sumGreen += (_green*mask[indexOfMask]);
				sumBlue += (_blue*mask[indexOfMask]);
			}
		}
	}
	red = sumRed;
	green = sumGreen;
	blue = sumBlue;
}
void ImageContainer::toSafeColor(int &color)
{
	if (color > 255)
	{
		color = 255;
	}
	else if (color < 0)
	{
		color = 0;
	}
}
ImageContainer * ImageContainer::filter(double * mask, int maskWidth, int maskHeight, bool AbsoluteValue)const
{
	ImageContainer *image = new ImageContainer(width, height);
	if (image != 0 && image->isInitialize())
	{
		for (int h = 0; h < height; h++)
		{
			for (int w = 0; w < width; w++)
			{
				double sumRed = 0, sumGreen = 0, sumBlue = 0;
				subFilter(mask, maskWidth, maskHeight, w, h, sumRed, sumGreen, sumBlue);
				int red = (int)sumRed;
				int green = (int)sumGreen;
				int blue = (int)sumBlue;
				if (AbsoluteValue)
				{
					red = abs(red);
					green = abs(green);
					blue = abs(blue);
				}
				toSafeColor(red);
				toSafeColor(green);
				toSafeColor(blue);
				image->setColor(w, h, RGB(red, green, blue));
			}
		}
		return image;
	}
	else
	{
		delete image;
		return 0;
	}
}

int ImageContainer::getIndex(int x, int y)const
{
	if (x > 0 && x < width && y>0 && y < height)
	{
		return width*y + x;
	}
	return -1;
}
void ImageContainer::toBinary(int threshold)
{

	int size = this->getSize();
	for (int i = 0; i < size; i++)
	{
		DWORD color = this->rgb[i];
		int _red = GetRValue(color) > threshold ? 255 : 0;
		int _blue = GetBValue(color) > threshold ? 255 : 0;
		int _green = GetGValue(color) > threshold ? 255 : 0;
		this->rgb[i] = RGB(_red, _green, _blue);
	}
	this->imageType = BINARY;

}
void ImageContainer::toReduceColor() 
{
	ImageContainer *image =reducedColorDepth();
	int size = this->getSize();
	if (image != 0)
	{
		for (int i = 0; i < size; i++)
		{
			this->rgb[i] = image->rgb[i];
		}
		delete image;
	}
}
void ImageContainer::toSmooth()
{
	double mask[9] = { 0.1,0.1 ,0.1 ,
		0.1 ,0.2 ,0.1 ,
		0.1,0.1 ,0.1 };
	ImageContainer *image = filter(mask,3,3,true);
	int size = this->getSize();
	if (image != 0)
	{
		for (int i = 0; i < size; i++)
		{
			this->rgb[i] = image->rgb[i];
		}
		delete image;
	}
}
ImageContainer* ImageContainer::toonShading()
{
	ImageContainer *image = new ImageContainer(width, height);
	if (image == 0)
	{
		return 0;
	}
	ImageContainer* copyE =copy();
	if (copyE == 0)
	{
		return 0;
	}
	copyE->toGray();
	//copyE->toSmooth();
	//copyE->toSmooth();
	//copyE->toSmooth();
	//copyE->toReduceColor();
	ImageContainer* blackEdged = copyE->edge();
	if (blackEdged == 0)
	{
		return 0;
	}
	blackEdged->toBinary(20);
	ImageContainer* copyR = copy();
	if (copyR == 0)
	{
		return 0;
	}
	copyR->toSmooth();
	copyR->toSmooth();
	copyR->toSmooth();
	copyR->toReduceColor();
	ImageContainer* reduced= copyR;
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			int rColor = reduced->getColor(w, h);
			int eColor = blackEdged->getColor(w, h);
			int targetColor = rColor;
			int _red = GetRValue(targetColor);
			int _blue = GetBValue(targetColor);
			int _green = GetGValue(targetColor);
			if (GetRValue(eColor) == 255)
			{
				_red = _blue = _green = 0;
			}
			toSafeColor(_red);
			toSafeColor(_blue);
			toSafeColor(_green);
			image->setColor(w, h, RGB(_red, _green, _blue));
		}
	}
	delete copyR;
	delete blackEdged;
	delete copyE;
	return image;
}
ImageContainer* ImageContainer::edge()const
{
	double mask[9] =
	{ -1,-1,-1,
	  -1,8,-1,
	  -1,-1,-1
	};
	return this->filter(mask, 3, 3, true);
}
ImageContainer* ImageContainer::reducedColorDepth()const
{
	ImageContainer *image = new ImageContainer(width, height);
	int shift = 5;
	int shiftMax = (255 >> shift) << shift;
	if (image != 0 && image->isInitialize())
	{
		for (int h = 0; h < height; h++)
		{
			for (int w = 0; w < width; w++)
			{
				int color = this->getColor(w, h);
				int _red = GetRValue(color);
				int _blue = GetBValue(color);
				int _green = GetGValue(color);
				int red, green, blue;
				red = (_red >> shift) << shift;
				blue = (_blue >> shift) << shift;
				green = (_green >> shift) << shift;
				image->setColor(w, h, RGB(red * 255 / shiftMax, green * 255 / shiftMax, blue * 255 / shiftMax));
			}
		}
		return image;
	}
	else
	{
		delete image;
		return 0;
	}
}
ImageContainer::~ImageContainer()
{
	printf("MyClass destroyed\n");
	delete[] rgb;
	rgb = 0;
	this->init = false;
	this->imageType = UNDEFINE;
}
