//
// Created by andy on 2021/6/14.
//
#pragma once

#include <iostream>
#include <qimage.h>
#include "gdal_priv.h"
#include "gdal.h"

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
    //��ȡͼ�񵥲������ؾ���
    void ReadImage(const char *InputImagePath, const int nBand);

    //�������
    void BandCombination(BandData imagename);

    //���ͼ�񲨶�������Ϣ
    double **GetImageData();

    //ͼ������
    void ImageStretching(const char *InputImagePath);

    //���ͼ����Ϣ
    int GetBandNum();

    int GetImgWidth();

    int GetImgHeight();

    int GetDepth();


protected:
    //ͼ����Ϣ
    int bandNum;
    int imgWidth;
    int imgHeight;
    int depth;

    //����ÿ���������ֵ������ָ��
    int maxPix[4];

    //������������ֵ�Ķ�ά�����ָ��
    double **imagedata;

    GDALDataset *ImageData;

};