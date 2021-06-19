//
// Created by andy on 2021/6/17.
//

#ifndef RS_IMAGE_PROCESSING_WATER_H
#define RS_IMAGE_PROCESSING_WATER_H

#endif //RS_IMAGE_PROCESSING_WATER_H

#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <math.h>
#include <qimage.h>
#include "gdal_priv.h"
#include "gdal.h"
#include "Eigen/Eigen"

using namespace Eigen;
using namespace std;

class Water
{
public:
    void GetNDWI();

    void GetMNDWI();

    void DrawImage(double **imagedata, const QString outputname);

    void SmoothImage(const QString outputname);

    double **GetNDWI_1993();
    double **GetNDWI_2005();
    double **GetMNDWI_1993();
    double **GetMNDWI_2005();

protected:
    double **NDWI_1993;
    double **NDWI_2005;
    double **MNDWI_1993;
    double **MNDWI_2005;
    int imgWidth;
    int imgHeight;
};