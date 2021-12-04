#pragma once
#include "stdafx.h"
#include "afxdialogex.h"
#include <atlimage.h>
#include <fstream>
#include <string>
#include "PPM_Reader.h"
#include <conio.h>
#include <cmath>
using namespace std;
bool PPM_Reader::isInitalize()const
{
	return initalize;
}
CImage * PPM_Reader::toCImage() {
	if (isInitalize())
	{
		if (pic != 0)
		{
			CImage *temp = new CImage();


			if (temp->Create(width, height, bit_per_pixal, NULL))
			{

				for (int i = 0; i < height; i++)
				{
					memcpy(temp->GetPixelAddress(0, i),pic+ i*bit_per_pixal*width / 8, bit_per_pixal*width / 8);
				}
				return temp;
			}
			else
			{
				temp->Destroy();
			}
		}
	}
	return 0;
}

PPM_Reader::PPM_Reader()
{
	initialize();
}

PPM_Reader::~PPM_Reader()
{
	initialize();
}

void PPM_Reader::initialize()
{
	fileType.clear();
	maxval = 0;
	initalize = false;
	readSuccees = false;
	name.clear();
	pictureType.clear();
	sourceLocation.clear();
	width = 0;
	height = 0;
	delete pic;
	pic = 0;
	bit_per_pixal = 0;
}

bool PPM_Reader::isPPM(string extension)
{

	return extension.compare("ppm") == 0;
}

// whether the file is ppm file


// read ppm file
boolean PPM_Reader::readPPM(string filepath, string filename, string extension)
{
	FILE *file = 0;
	//AllocConsole();
	string fileWholePath = "";
	fileWholePath.append(filepath);
	fileWholePath.append("\\");
	fileWholePath.append(filename);
	fileWholePath.append(".");
	fileWholePath.append(extension);
	fopen_s(&file, fileWholePath.c_str(), "r");
	if (file != 0)
	{
		pictureType = extension;
		name = filename;
		sourceLocation = filepath;
		// start read the data
		int headerNumber = 0;
		char buffer[300];
		while (fgets(buffer, 300, file))
		{
			if (buffer[0] != '#')
			{
				if (fileType.empty())
				{
					char temp[3];
					sscanf_s(buffer, "%s", temp, (unsigned)_countof(temp));
					fileType.append(temp);
				}
				else
				{
					if (width == 0 && height == 0)
					{
						sscanf_s(buffer, "%d %d", &width, &height);
					}
					else
					{
						if (maxval == 0)
						{
							sscanf_s(buffer, "%d", &maxval);
							break;
						}
					}
				}
				headerNumber++;
			}
		}
		bit_per_pixal = (int)log2(maxval + 1) * 3;

		size = width * height * 3 * bit_per_pixal / 8;
		unsigned  char *colors = new unsigned char[size];
		int strSize = 0;
		/*for(int i=0;i<height;i++)
		{
			fread(colors+ strSize, 1, width*3, file);
			strSize+=(width*3);
		}*/
		fread(colors, 1, size, file);
		_cprintf("PictureType:%s\n", pictureType.c_str());
		_cprintf("FileType:%s\n", fileType.c_str());
		_cprintf("Width:%d\n", width);
		_cprintf("Height:%d\n", height);
		_cprintf("Maxval:%d\n", maxval);
		_cprintf("headerNumber:%d\n", headerNumber);
		fclose(file);
	

			pic= new byte[size];

		
		
		for (int i = 0; i < size; i += 3)
		{
			pic[i] = colors[i + 2];
			pic[i + 1] = colors[i + 1];
			pic[i + 2] = colors[i];
		}
		free(colors);
		initalize = true;
		return true;
	}
	return false;
}



