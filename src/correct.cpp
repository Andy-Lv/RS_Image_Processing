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

void Correct::GetParameter(double source_x[8],
                           double source_y[8],
                           double target_x[8],
                           double target_y[8])
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
        A(i, 1) = target_x[i];
        A(i, 2) = target_y[i];
        A(i, 3) = pow(target_x[i], 2);
        A(i, 4) = target_x[i] * target_y[i];
        A(i, 5) = pow(target_y[i], 2);
    }

    for (int i = 0; i < 8; ++i)
    {
        Lx(i, 0) = source_x[i];
        Ly(i, 0) = source_y[i];
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
//第一步:纠正后图像边界范围确定

    //读取第一波段获得图像属性数据
    The_Image wuce;
    const char *InputImagePath = "../data/Geometric_Correction/wuce.tif";
    wuce.ReadImage(InputImagePath, 1);

    //获取四个角点像素坐标
    int corner_x[4] = {0, wuce.GetImgWidth(), wuce.GetImgWidth(), 0};
    int corner_y[4] = {0, 0, wuce.GetImgHeight(), wuce.GetImgWidth()};

    //计算角点转换后的坐标位置
    double new_corner_x[4];
    double new_corner_y[4];

    //获取参数
    this->GetParameter(coor_a, coor_b, coor_x, coor_y);

    for (int i = 0; i < 4; ++i)
    {
        new_corner_x[i] = this->para_a[0] + this->para_a[1] * corner_x[i] +
                          this->para_a[2] * corner_y[i] + this->para_a[3] * pow(corner_x[i], 2) +
                          this->para_a[4] * corner_x[i] * corner_y[i] + this->para_a[5] * pow(corner_y[i], 2);
        new_corner_y[i] = this->para_b[0] + this->para_b[1] * corner_x[i] +
                          this->para_b[2] * corner_y[i] + this->para_b[3] * pow(corner_x[i], 2) +
                          this->para_b[4] * corner_x[i] * corner_y[i] + this->para_b[5] * pow(corner_y[i], 2);
    }

    //获取转换后坐标位置的最大值和最小值
    double max_x, max_y, min_x, min_y;
    double temp_x = 0;
    double temp_y = 0;

    for (int i = 0; i < 4; ++i)
    {
        temp_x = max(temp_x, new_corner_x[i]);
        temp_y = max(temp_y, new_corner_y[i]);
    }
    max_x = temp_x;
    max_y = temp_y;

    for (int i = 0; i < 4; ++i)
    {
        temp_x = min(temp_x, new_corner_x[i]);
        temp_y = min(temp_y, new_corner_y[i]);
    }
    min_x = temp_x;
    min_y = temp_y;

    this->new_imgWidth = int((max_x - min_y) / 2) + 1;
    this->new_imgHeight = int((max_y - min_y) / 2) + 1;

//间接法纠正
    GDALAllRegister();
    //图像驱动
    GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");

    GDALDataset *InputImage = (GDALDataset *) GDALOpen(InputImagePath, GA_ReadOnly);
    //创建8bit的数据
    GDALDataset *TargetImage = poDriver->Create("../Output_Image/wuce_corrected.tif", new_imgWidth, new_imgHeight,
                                                wuce.GetBandNum(),
                                                GDT_Byte,
                                                NULL);
    double dGeoTrans[6] = {0};

    //设置仿射变换参数
    InputImage->GetGeoTransform(dGeoTrans);
    TargetImage->SetGeoTransform(dGeoTrans);
    //设置图像投影信息
    TargetImage->SetProjection(InputImage->GetProjectionRef());

    //用于保存读取的8bit数据
    GByte *TargetData = new GByte[new_imgWidth];

    //定义两幅图像坐标远点的偏移
    int Dx = int(min_x);
    int Dy = int(min_y);

    //定义原始波段像素数据
    double **banddata;

    //定义间接法回到原图像后的坐标
    double Gx, Gy;

    //获取参数
    this->GetParameter(coor_x, coor_y, coor_a, coor_b);

    //循环波段
    for (int iBand = 1; iBand <= wuce.GetBandNum(); iBand++)
    {
        GDALRasterBand *TargetBand = TargetImage->GetRasterBand(iBand);

        wuce.ReadImage(InputImagePath, iBand);
        banddata = wuce.GetImageData();

        for (int i = 0; i < new_imgHeight; i++)    //循环图像高
        {
            //循环新图像,采用间接法双线性内插方式获取像素值
            for (int j = 0; j < new_imgWidth; j++)
            {
                //新图像坐标系转换为原图像的坐标系
                Gx = this->para_a[0] + this->para_a[1] * (j + Dx) +
                     this->para_a[2] * (i + Dy) + this->para_a[3] * pow((j + Dx), 2) +
                     this->para_a[4] * (j + Dx) * (i + Dy) + this->para_a[5] * pow((i + Dy), 2);
                Gy = this->para_b[0] + this->para_b[1] * (j + Dx) +
                     this->para_b[2] * (i + Dy) + this->para_b[3] * pow((j + Dx), 2) +
                     this->para_b[4] * (j + Dx) * (i + Dy) + this->para_b[5] * pow((i + Dy), 2);

                //双线性内插
                if (Gx <= wuce.GetImgWidth() && Gy <= wuce.GetImgHeight() && Gx >= 0 && Gy >= 0)
                {
                    double p = Gx - int(Gx);
                    double q = Gy - int(Gy);
                    TargetData[j] =
                            (1 - q) * ((1 - p) * banddata[int(Gy)][int(Gx)] + p * banddata[int(Gy)][int(Gx) + 1]) +
                            q * ((1 - p) * banddata[int(Gy) + 1][int(Gx)] + p * banddata[int(Gy) + 1][int(Gx) + 1]);
                }
            }
            TargetBand->RasterIO(GF_Write, 0, i, new_imgWidth, 1, TargetData, new_imgWidth, 1, GDT_Byte, 0, 0);
        }
        free(TargetData);    //释放内存
    }
    //关闭原始图像和结果图像
    GDALClose((GDALDatasetH) TargetImage);
    GDALClose((GDALDatasetH) InputImage);
}