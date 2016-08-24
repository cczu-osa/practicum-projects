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

#ifndef SIMPLECV_TYPES_H
#define SIMPLECV_TYPES_H

#ifndef SCV_INLINE
#  if defined __cplusplus
#    define SCV_INLINE inline
#  elif defined _MSC_VER
#    define SCV_INLINE __inline
#  else
#    define SCV_INLINE static
#  endif
#endif // SCV_INLINE

typedef signed char ScvByte;
typedef unsigned char ScvUByte;
typedef int ScvBool;
#define SCV_FALSE 0
#define SCV_TRUE 1

#pragma pack(1)

typedef struct _ScvPoint {
    int x;
    int y;
} ScvPoint;

SCV_INLINE ScvPoint scvPoint(int x, int y) {
    ScvPoint p;
    p.x = x;
    p.y = y;
    return p;
}

typedef struct _ScvSize {
    int width;
    int height;
} ScvSize;

SCV_INLINE ScvSize scvSize(int width, int height) {
    ScvSize size;
    size.width = width;
    size.height = height;
    return size;
}

typedef struct _ScvMat {
    int rows;
    int cols;
    float *data;
} ScvMat;

SCV_INLINE ScvMat scvMat(int rows, int cols, float *data) {
    ScvMat mat;
    mat.rows = rows;
    mat.cols = cols;
    mat.data = data;
    return mat;
}

#pragma pack(push)
#pragma pack(1)

typedef struct _ScvPixel {
    ScvUByte b;
    ScvUByte g;
    ScvUByte r;
} ScvPixel;

#pragma pack(pop)

SCV_INLINE ScvPixel scvPixel(int b, int g, int r) {
    ScvPixel pixel;
    pixel.b = (ScvUByte) b;
    pixel.g = (ScvUByte) g;
    pixel.r = (ScvUByte) r;
    return pixel;
}

SCV_INLINE ScvPixel scvPixelAll(int bgr) {
    ScvPixel pixel;
    pixel.b = (ScvUByte) bgr;
    pixel.g = (ScvUByte) bgr;
    pixel.r = (ScvUByte) bgr;
    return pixel;
}

typedef struct _ScvImage {
    // The logical origin point if left-top,

    int width; // Real width in pixel
    int height;
    int widthBytes; // Bmp width in byte (a multiple of 4)

    /**
     * The real origin of image,
     * 0 if left-top, 1 if left-bottom.
     * Only need to be considered when manipulating data array.
     */
    int origin;

    /**
     * Pixel data, e.g. a 2*2 image: [b g r b g r 0 0 b g r b g r 0 0],
     * the tailing 0 in every line is for aligning
     */
    void *data;
} ScvImage;

typedef enum _SCV_GRAYING_TYPE {
    SCV_GRAYING_R = 0,
    SCV_GRAYING_G,
    SCV_GRAYING_B,
    SCV_GRAYING_MAX,
    SCV_GRAYING_AVG,
    SCV_GRAYING_W_AVG
} SCV_GRAYING_TYPE;

typedef struct _ScvHistogram {
    SCV_GRAYING_TYPE grayingType;
    int *val;
} ScvHistogram;

SCV_INLINE ScvHistogram scvHistogram(SCV_GRAYING_TYPE grayingType, int val[256]) {
    ScvHistogram histogram;
    histogram.grayingType = grayingType;
    histogram.val = val;
    return histogram;
}

typedef enum _SCV_FLIP_TYPE {
    SCV_FLIP_HORIZONTAL,
    SCV_FLIP_VERTICAL
} SCV_FLIP_TYPE;

typedef enum _SCV_SMOOTH_TYPE {
    SCV_SMOOTH_AVG,
    SCV_SMOOTH_MEDIAN,
    SCV_SMOOTH_GAUSSIAN
} SCV_SMOOTH_TYPE;

#endif //SIMPLECV_TYPES_H
