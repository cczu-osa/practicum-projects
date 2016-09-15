//
// Created by Richard Chien on 6/19/16.
//

#include <stdio.h>
#include <stdint.h>
#include "bitmap.h"
#include "LogUtils/log_utils.h"

// 按照 2 字节对齐, 否则 sizeof 获取结构体长度不对
#pragma pack(2)

typedef struct {
    int8_t bfType[2];
    int32_t bfSize;
    int32_t bfReserved;
    int32_t bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    int32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    int16_t biPlanes;
    int16_t biBitCount;
    int32_t biCompression;
    int32_t biSizeImage;
    int32_t biXPixPerMeter;
    int32_t biYPixPerMeter;
    int32_t biClrUsed;
    int32_t biClrImportant;
} BITMAPINFOHEADER;

void writeImageToBmp(IplImage *src, const char *outFilePath) {
    IplImage *image;
    if (3 == src->nChannels) {
        // The original image already has b, g, r channels.
        image = cvCloneImage(src);
    } else {
        image = cvCreateImage(cvGetSize(src), src->depth, 3);
        for (int i = 0; i < src->height; i++) {
            for (int j = 0; j < src->width; j++) {
                CvScalar scalar = cvGet2D(src, i, j);
                scalar.val[1] = scalar.val[2] = scalar.val[0];
                cvSet2D(image, i, j, scalar);
            }
        }
    }

    int imageSize = image->imageSize * 3;

    BITMAPFILEHEADER fileHeader = {0};
    fileHeader.bfType[0] = 'B';
    fileHeader.bfType[1] = 'M';
    fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + imageSize;
    fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    log_d("writeImageToBmp()", "offset: %d", fileHeader.bfOffBits);

    BITMAPINFOHEADER infoHeader = {0};
    infoHeader.biSize = sizeof(BITMAPINFOHEADER);
    infoHeader.biHeight = -image->height;
    infoHeader.biWidth = image->width;
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = 24;
    infoHeader.biSizeImage = imageSize;
    infoHeader.biCompression = 0;

    FILE *bmpFile = fopen(outFilePath, "wb");
    if (bmpFile == NULL) {
        log_e("writeImageToBmp()", "Open bmp file failed.");
    } else {
        fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, bmpFile);
        fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, bmpFile);
        fwrite(image->imageData, (size_t) imageSize, 1, bmpFile);
        fclose(bmpFile);
    }

    cvReleaseImage(&image);
}
