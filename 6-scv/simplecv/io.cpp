//
// Copyright (c) 2016 Richard Chien
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//
// Created by Richard Chien on 6/20/16.
//

#include <stdio.h>
#include "io.h"
#include "core.h"

#pragma mark - Inner

typedef signed char Int8;
typedef short Int16;
typedef int Int32;
#ifdef _WIN32
typedef __int64 Int64;
#else
typedef long long Int64;
#endif

#pragma pack(push)
#pragma pack(2)

typedef struct _BitmapFileHeader {
    Int16 bfType;
    Int32 bfSize;
    Int32 bfReserved;
    Int32 bfOffBits;
} BitmapFileHeader;

typedef struct _BitmapInfoHeader {
    Int32 biSize;
    Int32 biWidth;
    Int32 biHeight;
    Int16 biPlanes;
    Int16 biBitCount;
    Int32 biCompression;
    Int32 biSizeImage;
    Int32 biXPixPerMeter;
    Int32 biYPixPerMeter;
    Int32 biClrUsed;
    Int32 biClrImportant;
} BitmapInfoHeader;

#pragma pack(pop)

ScvBool saveImageToBmp(ScvImage *image, const char *filename) {
    int imageByteCount = image->widthBytes * image->height;

    BitmapFileHeader fileHeader = {0};
    fileHeader.bfType = 0x4D42; // "BM"
    fileHeader.bfSize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + imageByteCount;
    fileHeader.bfOffBits = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);

    BitmapInfoHeader infoHeader = {0};
    infoHeader.biSize = sizeof(BitmapInfoHeader);
    infoHeader.biHeight = image->origin ? image->height : -image->height;
    infoHeader.biWidth = image->width;
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = 24;
    infoHeader.biSizeImage = imageByteCount;
    infoHeader.biCompression = 0;

    FILE *bmpFile = fopen(filename, "wb");
    if (NULL != bmpFile) {
        fwrite(&fileHeader, sizeof(BitmapFileHeader), 1, bmpFile);
        fwrite(&infoHeader, sizeof(BitmapInfoHeader), 1, bmpFile);
        fwrite(image->data, (size_t) imageByteCount, 1, bmpFile);
        fclose(bmpFile);
        return SCV_TRUE;
    }
    return SCV_FALSE;
}

ScvImage *readImageFromBmp(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (NULL == fp) {
        return NULL;
    }

    BitmapFileHeader fileHeader;
    fread(&fileHeader, sizeof(BitmapFileHeader), 1, fp);

    BitmapInfoHeader infoHeader;
    fread(&infoHeader, sizeof(BitmapInfoHeader), 1, fp);

    const int h = infoHeader.biHeight;
    int origin = h > 0 ? 1 : 0;
    ScvImage *image = scvCreateImage(
            scvSize(infoHeader.biWidth, h > 0 ? h : -h));
    image->origin = origin;

    fread(image->data, (size_t) infoHeader.biSizeImage, 1, fp);

    fclose(fp);
    return image;
}

#pragma mark - Export

ScvImage *scvLoadImage(const char *filename) {
    return readImageFromBmp(filename);
}

ScvBool scvSaveImage(ScvImage *image, const char *filename) {
    return saveImageToBmp(image, filename);
}
