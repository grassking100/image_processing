主要新增的兩個function在ImageHandler.cpp:
	bool toGray(CImage *dst,const CImage * src):將圖src轉換成灰階圖並輸出在dst上
	bool grayIntensityHistogram(CImage *dst,const CImage  *src):將src的亮度值呈現在dst裡