#pragma once
#include "stdafx.h"
#include <atlimage.h>
#include "ImageContainer.h"
#ifdef __cplusplus
int linear(int x1, int y1, int x2, int y2, int targetX);
int bilinear(int x11, int y11, int num11, int x12, int y12, int num12, int x21, int y21, int num21, int x22, int y22, int num22, int targetX, int targetY);
bool CImageCopy(CImage *dst, const CImage  *src);
bool toGray(CImage *dst,const CImage * src);
bool CImageCopy(CImage * dst, const CImage * src, int width, int height, bool reCreateDestination);
CImage * downScale(CImage * input, int scale);
CImage * upScale(CImage * input, int scale);
CImage * resize(CImage * input, float scale);
bool setBlankDefault(CImage * image, int width, int height, int bpp);
int colorValidate(int color, int max);
bool translation(CImage *dst, const CImage * src,int x,int y);
bool rotate(CImage *dst, const CImage *src, int degree);
bool isValidPosition(int x, int y, int widthMax,int heightMax);
bool grayIntensityHistogram(CImage *dst,const CImage  *src);
bool filter(CImage * dst, const CImage *src, double* mask, int maskWidth, int maskHeight);
bool downScale(CImage *  dst, const CImage * src, int scale);
bool fourierTransform(CImage *  dst, const CImage * src);
bool upScale(CImage *  dst, const CImage * src, int scale);
bool gammaCorrection(CImage *  dst, const CImage * src, double gammaValue);
bool wave(CImage *  dst, int width, int height, double coeff, double hor_shoft, double ver_shift, double theta_coeff);
bool waveFFT(CImage *  wave, CImage *  fft, int width, int height, double coeff, double hor_shoft, double ver_shift, double theta_coeff);
bool inverseFourierTransform(CImage *  dst, const CImage * src);
bool toCImage(CImage *image, const ImageContainer &container);
bool fromCImage(const CImage *image, ImageContainer &container);
bool toSubColor(CImage *  dst, const CImage * src, int range, COLORREF selectedColor);
bool toonShading(CImage *  dst, const CImage * src);
#endif