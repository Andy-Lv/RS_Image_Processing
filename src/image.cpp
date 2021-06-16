//
// Created by andy on 2021/6/14.
//
#include"include/image.h"

void The_Image::ReadImage(const char *InputImagePath, const int nBand)
{
    //GDAL���в�������Ҫ��ע��
    GDALAllRegister();

    //֧������·��
    CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");


    //��ȡTIFFͼ����Ϣ
    ImageData = (GDALDataset *) GDALOpen(InputImagePath, GA_ReadOnly);

    //�ж�ͼ���Ƿ��ȡ�ɹ�
    if (ImageData == nullptr)
    {
        cout << "Can not open the image" << endl;
    }

    //��ȡͼ����Ϣ

    this->imgWidth = ImageData->GetRasterXSize();//ͼ����
    this->imgHeight = ImageData->GetRasterYSize();//ͼ��߶�
    this->bandNum = ImageData->GetRasterCount();    //������
    this->depth = GDALGetDataTypeSize(ImageData->GetRasterBand(1)->GetRasterDataType()) / 8;    //ͼ�����

    cout << imgWidth << endl;
    cout << imgHeight << endl;
    cout << bandNum << endl;
    cout << depth << endl;

    imagedata = new double *[imgHeight];
    for (int i = 0; i < imgHeight; i++)
    {
        imagedata[i] = new double[imgWidth];
    }

    //��ȡ����
    GDALRasterBand *poBand;
    poBand = ImageData->GetRasterBand(nBand);

    cout << "Data Type:" << GDALGetDataTypeName(poBand->GetRasterDataType()) << endl;

    //��ȡ�ò��ε�����ֵ
    float *pafScanline;
    pafScanline = (float *) CPLMalloc(sizeof(float) * imgWidth * imgHeight);


    poBand->RasterIO(GF_Read, 0, 0, imgWidth, imgHeight, pafScanline, imgWidth, imgHeight, GDT_Float32, 0, 0);

    int t = 0;
    int max = 0;
    for (int i = 0; i < imgHeight; i++)
    {
        for (int j = 0; j < imgWidth; j++)
        {
            //TODO: �ֶ���������
            imagedata[i][j] = (float) pafScanline[t++];

            //�ҳ��������������ֵ
            if (imagedata[i][j] > max)
            {
                max = imagedata[i][j];
            }
        }
    }

    //���max����255������Ϊ255
    if (max > 255)
        this->maxPix[nBand - 1] = 255;
    else
        this->maxPix[nBand - 1] = max;

    //�ͷŻ�����
    CPLFree(pafScanline);

    //�ر�ͼ��
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
    //1 ������ͼ�豸
    QImage image(this->imgWidth, this->imgHeight, QImage::Format_ARGB32);

    //QImage�����޸�ͼƬ
    for (int i = 0; i < this->imgHeight; i++)
    {
        for (int j = 0; j < this->imgWidth; j++)
        {
            //��������
            image.setPixel(QPoint(j, i), qRgb(imagename.banddata_4[i][j],
                                              imagename.banddata_3[i][j],
                                              imagename.banddata_2[i][j]));
        }
    }

    //4 ����ͼƬ
    image.save("../Output_Image/Standard_false_color.png");
}

void The_Image::ImageStretching(const char *InputImagePath)
{
    GDALAllRegister();

    //ԭʼͼ��
    GDALDataset *InputImage = (GDALDataset *) GDALOpen(InputImagePath, GA_ReadOnly);

    //ͼ������
    GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");

    //����8bit������
    GDALDataset *OutputImage = poDriver->Create("../Output_Image/gaojing_Stretched.tif", imgWidth, imgHeight, bandNum,
                                                GDT_Byte,
                                                NULL);
    double dGeoTrans[6] = {0};

    //���÷���任����
    InputImage->GetGeoTransform(dGeoTrans);
    OutputImage->SetGeoTransform(dGeoTrans);
    //����ͼ��ͶӰ��Ϣ
    OutputImage->SetProjection(InputImage->GetProjectionRef());

    //���ڱ����ȡ��16bit����
    GUInt16 *InputData = new GUInt16[imgWidth];
    GByte *OutputData = new GByte[imgWidth];

    //ѭ������
    for (int iBand = 1; iBand <= bandNum; iBand++)
    {
        GDALRasterBand *InputBand = InputImage->GetRasterBand(iBand);
        GDALRasterBand *OutputBand = OutputImage->GetRasterBand(iBand);

        for (int i = 0; i < imgHeight; i++)    //ѭ��ͼ���
        {
            //�����ݶ�����
            InputBand->RasterIO(GF_Read, 0, i, imgWidth, 1, InputData, imgWidth, 1, GDT_UInt16, 0, 0);

            //ѭ������12bit����רΪ8bit���ݣ�ʹ���������췽ʽ
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
        free(OutputData);    //�ͷ��ڴ�

        //�ر�ԭʼͼ��ͽ��ͼ��
        GDALClose((GDALDatasetH) OutputImage);
        GDALClose((GDALDatasetH) InputImage);
    }
}