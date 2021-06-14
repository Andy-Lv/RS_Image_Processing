#include"image.h"

int main()
{
	The_Image the_image;
	BandData gaojing;


	//定义图像路径
	const char* InputImagePath = "../gaojing_subset.tif";

    //读取第一波段
	the_image.ReadImage(InputImagePath, 1);
	gaojing.banddata_1 = the_image.GetImageData();

	//读取第二波段
    the_image.ReadImage(InputImagePath, 2);
    gaojing.banddata_2 = the_image.GetImageData();

    //读取第二波段
    the_image.ReadImage(InputImagePath, 3);
    gaojing.banddata_3 = the_image.GetImageData();

    //读取第二波段
    the_image.ReadImage(InputImagePath, 4);
    gaojing.banddata_4 = the_image.GetImageData();

    //波段组合
    the_image.BandCombination(gaojing);

    //16位->8位
    the_image.ImageStretching(InputImagePath);

	return 0;
}