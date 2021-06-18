//
// Created by andy on 2021/6/14.
//
#pragma once

#include <iostream>
#include <qimage.h>
#include "gdal_priv.h"
#include "gdal.h"
#include "Eigen/Eigen"

using namespace Eigen;
using namespace std;

class BandData
{
public:
    double **banddata_1;
    double **banddata_2;
    double **banddata_3;
    double **banddata_4;
};

class The_Image
{
public:
    //提取图像单波段像素矩阵
    void ReadImage(const char *InputImagePath, const int nBand);

    //波段组合
    void BandCombination(const char *InputImagePath,BandData imagename, const QString outputimage);

    //输出图像波段像素信息
    double **GetImageData();

    //图像拉伸
    void ImageStretching(const char *InputImagePath,const char *outputimage);

    //输出图像信息
    int GetBandNum();

    int GetImgWidth();

    int GetImgHeight();

    int GetDepth();


protected:
    //图像信息
    int bandNum;
    int imgWidth;
    int imgHeight;
    int depth;


    //创建储存像素值的二维数组空指针
    double **imagedata;

    GDALDataset *ImageData;

};