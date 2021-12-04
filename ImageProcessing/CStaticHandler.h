#pragma once
#include "stdafx.h"
#include <atlimage.h>
class CStaticHandler
{
public:
	static bool SetBitmap(CStatic * device, CImage * target, CImage * src);
	static bool SetResizeBitmap(CStatic * device, CImage * target, CImage * src, int width, int height);
	static int getCStaticWidth(CStatic * device);
	static int getCStaticHeight(CStatic * device);
	static int getCStaticBitPerPixal(CStatic * device);
	static bool setBlankDefault(CStatic * device, int width, int height, int bpp);
};