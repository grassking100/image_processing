#include "stdafx.h"
#include "CStaticHandler.h"
#include "ImageHandler.h"
bool   CStaticHandler::SetBitmap(CStatic *device, CImage * target, CImage * src)
{
	if (target != NULL && src != NULL)
	{
		HBITMAP window = device->GetBitmap();
		CImageCopy(target, src);
		device->SetBitmap(*target);
		return true;
	}
	return false;
}
bool   CStaticHandler::SetResizeBitmap(CStatic *device, CImage * target, CImage * src, int width, int height)
{
	if (target != NULL && src != NULL)
	{
		int newWidth = src->GetWidth();
		int newHeight = src->GetHeight();
		double ratio = 0;
		HBITMAP window = device->GetBitmap();
		if (window != NULL)
		{
			double widthRatio = width / (double)newWidth;
			double heightRatio = height / (double)newHeight;
			ratio = widthRatio > heightRatio ? heightRatio : widthRatio;
			newWidth = (int)ratio * src->GetWidth();
			newHeight = (int)ratio * src->GetHeight();
		}
		if (ratio > 1)
		{
			target = upScale(src, (int)ratio);
			device->SetBitmap(*target);
			return true;
		}
		else if (ratio < 1 && ratio > 0)
		{
			int temp = (int)(1 / ratio);
			target = downScale(src, temp);
			device->SetBitmap(*target);
			return true;
		}
		else if (ratio == 1)
		{
			CImageCopy(target, src);
			device->SetBitmap(*target);
			return true;
		}
	}
	return false;
}
int CStaticHandler::getCStaticWidth(CStatic * device)
{
	HBITMAP window = device->GetBitmap();
	if (window != NULL)
	{
		BITMAP windwoBitmap;
		if (GetObject(window, sizeof(windwoBitmap), &windwoBitmap) != 0)
		{
			return  windwoBitmap.bmWidth;
		}
	}
	return 0;
}
int CStaticHandler::getCStaticHeight(CStatic * device)
{
	HBITMAP window = device->GetBitmap();
	if (window != NULL)
	{
		BITMAP windwoBitmap;
		if (GetObject(window, sizeof(windwoBitmap), &windwoBitmap) != 0)
		{
			return  windwoBitmap.bmHeight;
		}
	}
	return 0;
}
int CStaticHandler::getCStaticBitPerPixal(CStatic * device)
{
	HBITMAP window = device->GetBitmap();
	if (window != NULL)
	{
		BITMAP windwoBitmap;
		if (GetObject(window, sizeof(windwoBitmap), &windwoBitmap) != 0)
		{
			return  windwoBitmap.bmBitsPixel;
		}
	}
	return 0;
}
bool CStaticHandler::setBlankDefault(CStatic * device, int width, int height, int bpp)
{

	if (device != 0)
	{
		CImage temp;
		if (temp.Create(width, height, bpp, NULL))
		{
			device->SetBitmap(temp);
			return true;
		}
	}
	return false;
}



