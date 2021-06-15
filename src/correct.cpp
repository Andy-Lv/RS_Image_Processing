//
// Created by andy on 2021/6/15.
//

#include "include/correct.h"
#include "include/image.h"

void Correct::GetCoordinate()
{
    //定义中间变量
    int i = 0;
    double x, y;

    //打开纠正前坐标文件
    ifstream source;
    source.open("../data/Geometric_Correction/movingpoints.txt", ios::in);

    if (!source.is_open())
        cout << "can not open the movingpoints.txt" << endl;

    while (!source.eof())
    {
        //读取x坐标和y坐标
        source >> x >> y;
        this->coor_x[i] = x;
        this->coor_y[i] = y;
        //i+1 使得下一个读取的数据赋值到数组中
        i++;
    }
    source.close();

    //重置i
    i = 0;

    //读取纠正后坐标文件
    ifstream target;
    target.open("../data/Geometric_Correction/fixdpoints.txt", ios::in);

    if (!target.is_open())
        cout << "can not open the fixdpoints.txt" << endl;

    while (!target.eof())
    {
        //读取x坐标和y坐标
        target >> x >> y;
        this->coor_a[i] = x;
        this->coor_b[i] = y;
        //i+1 使得下一个读取的数据赋值到数组中
        i++;
    }
    target.close();
}

void Correct::GetParameter()
{
    //读取参数坐标
    this->GetCoordinate();

    //定义参数矩阵
    Matrix<double, 6, 1> delt_a;
    Matrix<double, 6, 1> delt_b;
    Matrix<double, 8, 6> A;
    Matrix<double, 8, 1> Lx;
    Matrix<double, 8, 1> Ly;
    Matrix<double, 8, 1> Vx;
    Matrix<double, 8, 1> Vy;

    //给参数矩阵赋值
    for (int i = 0; i < 8; ++i)
    {
        A(i, 0) = 1;
        A(i, 1) = this->coor_a[i];
        A(i, 2) = this->coor_b[i];
        A(i, 3) = pow(this->coor_a[i], 2);
        A(i, 4) = this->coor_a[i] * this->coor_a[i];
        A(i, 5) = pow(this->coor_b[i], 2);
    }

    for (int i = 0; i < 8; ++i)
    {
        Lx(i, 0) = this->coor_x[i];
        Ly(i, 0) = this->coor_y[i];
    }

    //计算多项式系数
    delt_a = (A.transpose() * A).inverse() * A.transpose() * Lx;
    delt_b = (A.transpose() * A).inverse() * A.transpose() * Ly;

    //返回多项式结果到参数数组中
    for (int i = 0; i < 6; ++i)
    {
        this->para_a[0] = delt_a(i, 0);
        this->para_b[0] = delt_b(i, 0);
    }
}

void Correct::RelativeRegistration()
{
//TODO:1 纠正后图像边界范围确定
//TODO:2 间接法纠正
//TODO:3 双线心内插灰度重采样
}

