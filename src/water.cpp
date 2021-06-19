//
// Created by andy on 2021/6/17.
//
#include "include/image.h"
#include "include/water.h"

void Water::GetNDWI()
{
    The_Image wuhan;
    BandData wuhan_water;

    //wuhan_1993的TM2和TM4储存在banddata_1和banddata_2中
    wuhan.ReadImage("../data/Water/1993_10_12_B2.TIF", 1);
    wuhan_water.banddata_1 = wuhan.GetImageData();

    wuhan.ReadImage("../data/Water/1993_10_12_B4.TIF", 1);
    wuhan_water.banddata_2 = wuhan.GetImageData();

    //wuhan_2005的TM2和TM4储存在banddata_3和banddata_4中
    wuhan.ReadImage("../data/Water/2005_09_11_B2.TIF", 1);
    wuhan_water.banddata_3 = wuhan.GetImageData();

    wuhan.ReadImage("../data/Water/2005_09_11_B4.TIF", 1);
    wuhan_water.banddata_4 = wuhan.GetImageData();

    //获取图像大小
    this->imgWidth = wuhan.GetImgWidth();
    this->imgHeight = wuhan.GetImgHeight();


    //创建NDWI像素矩阵
    this->NDWI_1993 = new double *[wuhan.GetImgHeight()];
    for (int i = 0; i < wuhan.GetImgHeight(); i++)
    {
        this->NDWI_1993[i] = new double[wuhan.GetImgWidth()];
    }

    //创建MNDWI像素矩阵
    this->NDWI_2005 = new double *[wuhan.GetImgHeight()];
    for (int i = 0; i < wuhan.GetImgHeight(); i++)
    {
        this->NDWI_2005[i] = new double[wuhan.GetImgWidth()];
    }

    //计算NDWI
    for (int i = 0; i < wuhan.GetImgHeight(); ++i)
    {
        for (int j = 0; j < wuhan.GetImgWidth(); ++j)
        {
            NDWI_1993[i][j] = (wuhan_water.banddata_1[i][j] - wuhan_water.banddata_2[i][j]) /
                              (wuhan_water.banddata_1[i][j] + wuhan_water.banddata_2[i][j]);
            NDWI_2005[i][j] = (wuhan_water.banddata_3[i][j] - wuhan_water.banddata_4[i][j]) /
                              (wuhan_water.banddata_3[i][j] + wuhan_water.banddata_4[i][j]);
        }
    }
}

void Water::GetMNDWI()
{
    The_Image wuhan;
    BandData wuhan_water;

    //wuhan_1993的TM2和TM5储存在banddata_1和banddata_2中
    wuhan.ReadImage("../data/Water/1993_10_12_B2.TIF", 1);
    wuhan_water.banddata_1 = wuhan.GetImageData();

    wuhan.ReadImage("../data/Water/1993_10_12_B5.TIF", 1);
    wuhan_water.banddata_2 = wuhan.GetImageData();

    //wuhan_2005的TM2和TM5储存在banddata_3和banddata_4中
    wuhan.ReadImage("../data/Water/2005_09_11_B2.TIF", 1);
    wuhan_water.banddata_3 = wuhan.GetImageData();

    wuhan.ReadImage("../data/Water/2005_09_11_B5.TIF", 1);
    wuhan_water.banddata_4 = wuhan.GetImageData();


    //创建MNDWI_1993像素矩阵
    this->MNDWI_1993 = new double *[wuhan.GetImgHeight()];
    for (int i = 0; i < wuhan.GetImgHeight(); i++)
    {
        this->MNDWI_1993[i] = new double[wuhan.GetImgWidth()];
    }

    //创建MNDWI_2005像素矩阵
    this->MNDWI_2005 = new double *[wuhan.GetImgHeight()];
    for (int i = 0; i < wuhan.GetImgHeight(); i++)
    {
        this->MNDWI_2005[i] = new double[wuhan.GetImgWidth()];
    }

    //计算NDWI
    for (int i = 0; i < wuhan.GetImgHeight(); ++i)
    {
        for (int j = 0; j < wuhan.GetImgWidth(); ++j)
        {
            MNDWI_1993[i][j] = (wuhan_water.banddata_1[i][j] - wuhan_water.banddata_2[i][j]) /
                               (wuhan_water.banddata_1[i][j] + wuhan_water.banddata_2[i][j]);
            MNDWI_2005[i][j] = (wuhan_water.banddata_3[i][j] - wuhan_water.banddata_4[i][j]) /
                               (wuhan_water.banddata_3[i][j] + wuhan_water.banddata_4[i][j]);
        }
    }
}

void Water::DrawImage(double **imagedata, const QString outputname)
{
    //1 创建绘图设备
    QImage image(this->imgWidth, this->imgHeight, QImage::Format_ARGB32);

    //对图像进行平滑处理
    double filtering[9];//定义滤波区域

    //QImage可以修改图片
    for (int i = 0; i < this->imgHeight; i++)
    {
        for (int j = 0; j < this->imgWidth; j++)
        {
            //设置像素
            if (imagedata[i][j] > 0)
            {
                image.setPixel(QPoint(j, i), qRgb(255, 255, 255));
            } else
            {
                image.setPixel(QPoint(j, i), qRgb(0, 0, 0));
            }


        }
    }

    //4 保存图片
    image.save("../Output_Image/" + outputname + ".png");

    cout<<"成功保存图像: "<<outputname.toStdString()<< ".png" << endl;
}

bool cmp(QColor a,QColor b)
{
    return (a.red()+a.blue()+a.green())>(b.red()+b.blue()+b.green());
}

void Water::SmoothImage(const QString outputname)
{
    QImage origin("../Output_Image/" + outputname + ".png");
    QImage newImage(origin.width(), origin.height(), QImage::Format_ARGB32);

    QColor oldColor;

    //对图像进行平滑处理
    QColor filtering[9];//定义滤波区域

    int averange;

    for(int i = 0; i<origin.width(); i++)
    {
        for(int j = 0; j<origin.height(); j++)
        {
            //开始中值滤波处理
            if(i>0&&j>0&&i<origin.height()-1&&j<origin.width())
            {
                //滤波矩阵赋值
                filtering[0]=origin.pixel(i-1,j-1);
                filtering[1]=origin.pixel(i-1,j);
                filtering[2]=origin.pixel(i-1,j+1);
                filtering[3]=origin.pixel(i,j-1);
                filtering[4]=origin.pixel(i,j);
                filtering[5]=origin.pixel(i,j+1);
                filtering[6]=origin.pixel(i+1,j-1);
                filtering[7]=origin.pixel(i+1,j);
                filtering[8]=origin.pixel(i+1,j+1);

                //给滤波矩阵排序
                sort(filtering,filtering+8,cmp);

                //中值滤波完成
                int average = (filtering[4].red()+filtering[4].green()+filtering[4].blue())/3;
                newImage.setPixel(i,j,qRgb(average,average,average));
            }
            else
            {
                oldColor = QColor(origin.pixel(i,j));
                int average = (oldColor.red()+oldColor.green()+oldColor.blue())/3;
                newImage.setPixel(i,j,qRgb(average,average,average));
            }
        }
    }

    //4 保存图片
    newImage.save("../Output_Image/" + outputname + "_smoothed.png");
}

double **Water::GetNDWI_1993()
{
    return this->NDWI_1993;
}

double **Water::GetNDWI_2005()
{
    return this->NDWI_2005;
}

double **Water::GetMNDWI_1993()
{
    return this->MNDWI_1993;
}
double **Water::GetMNDWI_2005()
{
    return this->MNDWI_2005;
}