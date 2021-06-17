//
// Created by andy on 2021/6/17.
//

#ifndef RS_IMAGE_PROCESSING_MAIN_H
#define RS_IMAGE_PROCESSING_MAIN_H

#endif //RS_IMAGE_PROCESSING_MAIN_H
#include "include/correct.h"
#include "include/image.h"
#include "include/water.h"

void ShowLiscence()
{
    cout<<"Welcome to RS Image Processing System"<<endl;
    cout<<"Created by Guoqiang Lv"<<endl;
    cout<<"Version: 1.3.2"<<endl<<endl<<endl;
}

void ShowMenu()
{
    cout<<"***********************************"<<endl;
    cout<<"1.波段组合"<<endl;
    cout<<"2.图像纠正"<<endl;
    cout<<"3.水体分析"<<endl;
    cout<<"0.退出系统"<<endl;
    cout<<"***********************************"<<endl;
}

void FirstProcessing()
{
    The_Image the_image;
    BandData gaojing;

    //定义图像路径
    const char* InputImagePath = "../data/Band_Combination/gaojing_subset.tif";

    //原图像标准假彩色波段组合
    cout<<"开始执行原图像标准假彩色波段组合"<<endl;
    the_image.BandCombination(InputImagePath,gaojing,"gaojing_Standard_false_color");

    cout<<"开始执行图像分段线性拉伸"<<endl;
    //16位->8位
    the_image.ImageStretching(InputImagePath,"gaojing_Stretched.tif");

    //开始执行分段线性拉伸之后图像标准假彩色波段组合
    cout<<"开始执行分段线性拉伸之后图像标准假彩色波段组合"<<endl;
    const char* StretchedImagePath = "../Output_Image/gaojing_Stretched.tif";
    the_image.BandCombination(StretchedImagePath,gaojing,"Stretched_gaojing_Standard_false_color");

    system("pause");
    system("clear");
}

void SecondProcessing()
{
    Correct wucesource;
    //获取参数
    wucesource.GetCoordinate();
    //进行相对配准
    cout<<"开始进行相对配准"<<endl;
    wucesource.RelativeRegistration("wuce_corrected.tif");
    cout<<"相对配准完成"<<endl;

    system("pause");
    system("clear");
}

void ThirdProcessing()
{
    Water wuhansource;

    cout<<"获取图像NDWI数据"<<endl;
    wuhansource.GetNDWI();
    cout<<"获取图像MNDWI数据"<<endl;
    wuhansource.GetMNDWI();

    cout<<"绘制 wuhan_ndwi_1993 "<<endl;
    wuhansource.DrawImage(wuhansource.GetNDWI_1993(),"wuhan_ndwi_1993");
    cout<<"绘制 wuhan_ndwi_2005 "<<endl;
    wuhansource.DrawImage(wuhansource.GetNDWI_2005(),"wuhan_ndwi_2005");
    cout<<"绘制 wuhan_mndwi_1993 "<<endl;
    wuhansource.DrawImage(wuhansource.GetMNDWI_1993(),"wuhan_mndwi_1993");
    cout<<"绘制 wuhan_mndwi_2005 "<<endl;
    wuhansource.DrawImage(wuhansource.GetMNDWI_2005(),"wuhan_mndwi_2005");

    cout<<"水体NDWI/MNDWI图像分析完毕"<<endl;

    system("pause");
    system("clear");
}