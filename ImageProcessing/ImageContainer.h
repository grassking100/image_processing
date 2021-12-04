#pragma once
class ImageContainer
{
public:
	enum ImageType { COLOR, GRAY, BINARY, UNDEFINE };
	ImageContainer(int width, int height);
	void initialize(int width, int height);
	bool isInitialize()const;
	DWORD  getColor(int x, int y)const;
	void setColor(int x, int y, DWORD color);
	int getHeight()const;
	int getWidth()const;
	ImageType getImageType()const;
	void toGray();
	ImageContainer* copy()const;
	ImageContainer* upScale(double scale)const;
	int getSize()const;
	~ImageContainer();
	void initialize();
	static double distance(DWORD lhs, DWORD rhs);
	ImageContainer* downScale(double scale)const;
	ImageContainer * getSubColor(DWORD color, int range)const;
	DWORD getGrayColor(DWORD color)const;
	ImageContainer * filter(double * mask, int maskWidth, int maskHeight, bool AbsoluteValue) const;
	ImageContainer * toonShading() ;
	ImageContainer * edge() const;
	ImageContainer * reducedColorDepth() const;
private:
	DWORD *rgb = 0;
	void subFilter(double * mask, int maskWidth, int maskHeight, int nowW, int nowH, double & red, double & green, double & blue)const;
	static void toSafeColor(int & color);
	
	bool init = false;
	int height = 0, width = 0;
	ImageType imageType = UNDEFINE;
	int getIndex(int x, int y)const;
	void toBinary(int threshold);
	void toReduceColor();
	void toSmooth();
};