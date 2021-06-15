//
// Created by andy on 2021/6/15.
//

#ifndef RS_IMAGE_PROCESSING_CORRECT_H
#define RS_IMAGE_PROCESSING_CORRECT_H

#endif //RS_IMAGE_PROCESSING_CORRECT_H

#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include <qimage.h>
#include "gdal_priv.h"
#include "gdal.h"
#include "Eigen/Eigen"

using namespace Eigen;

class Correct
{
public:
    //获取纠正前后像素坐标
    void GetCoordinate();

    //获取纠正参数
    void GetParameter();

    //相对配准(间接法)
    void RelativeRegistration();

protected:
    //定义参数数组(二阶多项式)
    double para_a[6];
    double para_b[6];

    //定义纠正前的图像坐标
    double coor_x[8];
    double coor_y[8];
    //定义纠正后的图像坐标
    double coor_a[8];
    double coor_b[8];
};
