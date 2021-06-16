//
// Created by andy on 2021/6/14.
//
#include"include/image.h"

void The_Image::ReadImage(const char *InputImagePath, const int nBand)
{
    //GDAL所有操作都需要先注册
    GDALAllRegister();

    //支持中文路径
    CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");


    //读取TIFF图像信息
    ImageData = (GDALDataset *) GDALOpen(InputImagePath, GA_ReadOnly);

    //判断图像是否读取成功
    if (ImageData == nullptr)
    {
        cout << "Can not open the image" << endl;
    }

    //读取图像信息

    this->imgWidth = ImageData->GetRasterXSize();//图像宽度
    this->imgHeight = ImageData->GetRasterYSize();//图像高度
    this->bandNum = ImageData->GetRasterCount();    //波段数
    this->depth = GDALGetDataTypeSize(ImageData->GetRasterBand(1)->GetRasterDataType()) / 8;    //图像深度

    cout << imgWidth << endl;
    cout << imgHeight << endl;
    cout << bandNum << endl;
    cout << depth << endl;

    imagedata = new double *[imgHeight];
    for (int i = 0; i < imgHeight; i++)
    {
        imagedata[i] = new double[imgWidth];
    }

    //读取波段
    GDALRasterBand *poBand;
    poBand = ImageData->GetRasterBand(nBand);

    cout << "Data Type:" << GDALGetDataTypeName(poBand->GetRasterDataType()) << endl;

    //读取该波段的像素值
    float *pafScanline;
    pafScanline = (float *) CPLMalloc(sizeof(float) * imgWidth * imgHeight);


    poBand->RasterIO(GF_Read, 0, 0, imgWidth, imgHeight, pafScanline, imgWidth, imgHeight, GDT_Float32, 0, 0);

    int t = 0;
    int max = 0;
    for (int i = 0; i < imgHeight; i++)
    {
        for (int j = 0; j < imgWidth; j++)
        {
            //TODO: 分段线性拉伸
            imagedata[i][j] = (float) pafScanline[t++];

            //找出本波段最大像素值
            if (imagedata[i][j] > max)
            {
                max = imagedata[i][j];
            }
        }
    }

    //如果max大于255就设置为255
    if (max > 255)
        this->maxPix[nBand - 1] = 255;
    else
        this->maxPix[nBand - 1] = max;

    //释放缓冲区
    CPLFree(pafScanline);

    //关闭图像
    GDALClose(ImageData);
}

int The_Image::GetBandNum()
{
    return this->bandNum;
}

int The_Image::GetImgWidth()
{
    return this->imgWidth;
}

int The_Image::GetImgHeight()
{
    return this->imgHeight;
}

int The_Image::GetDepth()
{
    return this->depth;
}

double **The_Image::GetImageData()
{
    return this->imagedata;
}

void The_Image::BandCombination(BandData imagename)
{
    //1 创建绘图设备
    QImage image(this->imgWidth, this->imgHeight, QImage::Format_ARGB32);

    //QImage可以修改图片
    for (int i = 0; i < this->imgHeight; i++)
    {
        for (int j = 0; j < this->imgWidth; j++)
        {
            //设置像素
            image.setPixel(QPoint(j, i), qRgb(imagename.banddata_4[i][j],
                                              imagename.banddata_3[i][j],
                                              imagename.banddata_2[i][j]));
        }
    }

    //4 保存图片
    image.save("../Output_Image/Standard_false_color.png");
}

void The_Image::ImageStretching(const char *InputImagePath)
{
    GDALAllRegister();

    //原始图像
    GDALDataset *InputImage = (GDALDataset *) GDALOpen(InputImagePath, GA_ReadOnly);

    //图像驱动
    GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");

    //创建8bit的数据
    GDALDataset *OutputImage = poDriver->Create("../Output_Image/gaojing_Stretched.tif", imgWidth, imgHeight, bandNum,
                                                GDT_Byte,
                                                NULL);
    double dGeoTrans[6] = {0};

    //设置仿射变换参数
    InputImage->GetGeoTransform(dGeoTrans);
    OutputImage->SetGeoTransform(dGeoTrans);
    //设置图像投影信息
    OutputImage->SetProjection(InputImage->GetProjectionRef());

    //用于保存读取的16bit数据
    GUInt16 *InputData = new GUInt16[imgWidth];
    GByte *OutputData = new GByte[imgWidth];

    //循环波段
    for (int iBand = 1; iBand <= bandNum; iBand++)
    {
        GDALRasterBand *InputBand = InputImage->GetRasterBand(iBand);
        GDALRasterBand *OutputBand = OutputImage->GetRasterBand(iBand);

        for (int i = 0; i < imgHeight; i++)    //循环图像高
        {
            //将数据读出来
            InputBand->RasterIO(GF_Read, 0, i, imgWidth, 1, InputData, imgWidth, 1, GDT_UInt16, 0, 0);

            //循环，将12bit数据专为8bit数据，使用线性拉伸方式
            for (int j = 0; j < imgWidth; j++)
            {
                double TempData = InputData[j];
                if (TempData > 255.0)
                {
                    TempData = 255;
                } else
                {
                    TempData = TempData * maxPix[iBand - 1] / 255;
                    OutputData[j] = (GUInt16) TempData;
                }
            }

            OutputBand->RasterIO(GF_Write, 0, i, imgWidth, 1, OutputData, imgWidth, 1, GDT_Byte, 0, 0);
        }
        free(OutputData);    //释放内存

        //关闭原始图像和结果图像
        GDALClose((GDALDatasetH) OutputImage);
        GDALClose((GDALDatasetH) InputImage);
    }
}