#pragma once
#include "stdafx.h"
#include "afxdialogex.h"
#include <string>
#include "ImageHandler.h"
using namespace std;
class PPM_Reader
{
protected:
	boolean readSuccees = false;
	string name;
	int size;
	int bit_per_pixal;
	string pictureType;
	string sourceLocation;
	int width = 0;
	int height = 0;
	string fileType ;
	byte* pic;
	bool initalize;
	// ' A "magic number" for identifying the file type. A ppm image's magic
	// number is the two characters "P6" of "P3" ' from
	// http://netpbm.sourceforge.net/doc/ppm.html
	int maxval = NULL;
public:
	bool isInitalize() const;
	CImage * toCImage() ;
	PPM_Reader();
	~PPM_Reader();
	void initialize();
	bool isPPM(string extension);
	boolean readPPM(string filepath,string filename,string extension);
};
