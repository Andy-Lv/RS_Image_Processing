#include"image.h"

int main()
{
	The_Image the_image;
	BandData gaojing;


	//����ͼ��·��
	const char* InputImagePath = "../gaojing_subset.tif";

    //��ȡ��һ����
	the_image.ReadImage(InputImagePath, 1);
	gaojing.banddata_1 = the_image.GetImageData();

	//��ȡ�ڶ�����
    the_image.ReadImage(InputImagePath, 2);
    gaojing.banddata_2 = the_image.GetImageData();

    //��ȡ�ڶ�����
    the_image.ReadImage(InputImagePath, 3);
    gaojing.banddata_3 = the_image.GetImageData();

    //��ȡ�ڶ�����
    the_image.ReadImage(InputImagePath, 4);
    gaojing.banddata_4 = the_image.GetImageData();

    //�������
    the_image.BandCombination(gaojing);

    //16λ->8λ
    the_image.ImageStretching(InputImagePath);

	return 0;
}