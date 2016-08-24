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

#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include "core.h"
#include "matrix.h"

#pragma mark - Inner

#define PI 3.14159265
#define MIN(val1, val2) ((val1) > (val2) ? (val2) : (val1))
#define MAX(val1, val2) ((val1) > (val2) ? (val1) : (val2))

int max(int a, int b, int c) {
    int m = a;
    m = b > m ? b : m;
    m = c > m ? c : m;
    return m;
}

float avg(int a, int b, int c) {
    return (a + b + c) / 3.0f;
}

float avgArr(int count, const int num[]) {
    int sum = 0;
    for (int i = 0; i < count; i++) {
        sum += num[i];
    }
    return (float) sum / count;
}

float avgRGBWeighed(int r, int g, int b) {
    return 0.30f * r + 0.59f * g + 0.11f * b;
}

float avgArrWeighed(int count, const int *num, const int *weight) {
    int sumWeight = 0;
    int sum = 0;
    for (int i = 0; i < count; i++) {
        sum += num[i] * weight[i];
        sumWeight += weight[i];
    }
    return (float) sum / sumWeight;
}

int compareInt(const void *a, const void *b) {
    return *(int *) a - *(int *) b;
}

float medianArr(int count, int num[]) {
    if (1 == count) {
        return num[0];
    }

    qsort(num, (size_t) count, sizeof(int), &compareInt);
    if (count % 2 == 0) {
        int m = count / 2;
        return ((float) num[m - 1] + num[m]) / 2.0f;
    } else {
        return num[count / 2];
    }
}

int grayValueOfPixel(ScvPixel pxl, SCV_GRAYING_TYPE type) {
    switch (type) {
        case SCV_GRAYING_R:
            return pxl.r;
        case SCV_GRAYING_G:
            return pxl.g;
        case SCV_GRAYING_B:
            return pxl.b;
        case SCV_GRAYING_MAX:
            return max(pxl.b, pxl.g, pxl.r);
        case SCV_GRAYING_AVG:
            return (int) (avg(pxl.r, pxl.g, pxl.b) + 0.5f);
        case SCV_GRAYING_W_AVG:
            return (int) (avgRGBWeighed(pxl.r, pxl.g, pxl.b) + 0.5f);
        default:
            return -1;
    }
}

float thresholdOtsu(const ScvHistogram *hist, int total) {
    // https://en.wikipedia.org/wiki/Otsu%27s_method
    int sum = 0;
	int i;
    for (i = 1; i < 256; ++i) {
        sum += i * hist->val[i];
    }
    int sumB = 0;
    int wB = 0;
    int wF = 0;
    int mB;
    int mF;
    float max = 0.0;
    float between;
    float threshold1 = 0.0;
    float threshold2 = 0.0;
    for (i = 0; i < 256; ++i) {
        wB += hist->val[i];
        if (wB == 0) {
            continue;
        }
        wF = total - wB;
        if (wF == 0) {
            break;
        }
        sumB += i * hist->val[i];
        mB = sumB / wB;
        mF = (sum - sumB) / wF;
        between = wB * wF * (mB - mF) * (mB - mF);
        if (between >= max) {
            threshold1 = i;
            if (between > max) {
                threshold2 = i;
            }
            max = between;
        }
    }
    return (threshold1 + threshold2) / 2.0f;
}

// Cumulative Distribution Function
void calcCDF(int length, const int *src, int *dst) {
    int curSum = 0;
    for (int i = 0; i < length; i++) {
        curSum += src[i];
        dst[i] = curSum;
    }
}

void traceEdge(int y, int x, int nThrLow, ScvUByte *pResult, int *pMag, ScvSize sz) {
    // http://blog.csdn.net/likezhaobin/article/details/6892629
    int xNum[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    int yNum[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    int yy, xx, k;
    for (k = 0; k < 8; k++) {
        yy = y + yNum[k];
        xx = x + xNum[k];
        if (pResult[yy * sz.width + xx] == 128 && pMag[yy * sz.width + xx] >= nThrLow) {
            pResult[yy * sz.width + xx] = 255;
            traceEdge(yy, xx, nThrLow, pResult, pMag, sz);
        }
    }
}

#pragma mark - Export

#pragma mark -- Make

ScvImage *scvCreateImage(ScvSize size) {
    ScvImage *image = (ScvImage *) malloc(sizeof(ScvImage));
    image->origin = 0;
    image->width = size.width;
    image->height = size.height;
    const int realWidthBytes = image->width * 3;
    image->widthBytes = realWidthBytes % 4 ? ((realWidthBytes >> 2) + 1) << 2 : realWidthBytes;
    const int dataSize = image->widthBytes * image->height;
    image->data = malloc((size_t) dataSize);
    memset(image->data, 0, (size_t) dataSize);
    return image;
}

ScvImage *scvCloneImage(const ScvImage *image) {
    ScvImage *result = scvCreateImage(scvGetSize(image));
    scvCopyImage(image, result);
    return result;
}

void scvCopyImage(const ScvImage *src, ScvImage *dst) {
    dst->origin = src->origin;
    dst->width = src->width;
    dst->height = src->height;
    dst->widthBytes = src->widthBytes;
    memcpy(dst->data, src->data, (size_t) (src->widthBytes * src->height));
}

void scvReleaseImage(ScvImage *image) {
    free(image->data);
    free(image);
}

ScvMat *scvCreateMat(int rows, int cols) {
    ScvMat *mat = (ScvMat *) malloc(sizeof(ScvMat));
    mat->rows = rows;
    mat->cols = cols;
    mat->data = (float *) malloc(rows * cols * sizeof(float));
    return mat;
}

ScvMat *scvCloneMat(const ScvMat *mat) {
    ScvMat *result = scvCreateMat(mat->rows, mat->cols);
    scvCopyMat(mat, result);
    return result;
}

void scvCopyMat(const ScvMat *src, ScvMat *dst) {
    dst->rows = src->rows;
    dst->cols = src->cols;
    for (int i = 0; i < src->rows * src->cols; i++) {
        dst->data[i] = src->data[i];
    }
}

void scvReleaseMat(ScvMat *mat) {
    free(mat->data);
    free(mat);
}

ScvHistogram *scvCreateHist(SCV_GRAYING_TYPE grayingType) {
    ScvHistogram *histogram = (ScvHistogram *) malloc(sizeof(ScvHistogram));
    histogram->grayingType = grayingType;
    histogram->val = (int *) malloc(256 * sizeof(int));
    memset(histogram->val, 0, 256 * sizeof(int));
    return histogram;
}

ScvHistogram *scvCloneHist(const ScvHistogram *histogram) {
    ScvHistogram *result = scvCreateHist(histogram->grayingType);
    scvCopyHist(histogram, result);
    return result;
}

void scvCopyHist(const ScvHistogram *src, ScvHistogram *dst) {
    dst->grayingType = src->grayingType;
    memcpy(dst->val, src->val, 256 * sizeof(int));
}

void scvReleaseHist(ScvHistogram *histogram) {
    free(histogram->val);
    free(histogram);
}

#pragma mark -- Getter and Setter

ScvPixel *scvGetPixelRef(const ScvImage *image, int x, int y) {
    const int w = image->width;
    const int h = image->height;
    const int bW = image->widthBytes;
    const int o = image->origin;

    if (x < 0 || x >= w || y < 0 || y >= h) {
        return NULL;
    }

    // Offer different offset by image.origin
    return (ScvPixel *) ((char *) image->data + (o ? h - 1 - y : y) * bW + x * 3);
}

ScvPixel scvGetPixel(const ScvImage *image, int x, int y) {
    ScvPixel pixel = {0};

    ScvPixel *tmpPxl = scvGetPixelRef(image, x, y);
    if (NULL == tmpPxl) {
        return pixel;
    }

    pixel.b = tmpPxl->b;
    pixel.g = tmpPxl->g;
    pixel.r = tmpPxl->r;
    return pixel;
}

void scvSetPixel(ScvImage *image, int x, int y, ScvPixel pixel) {
    ScvPixel *tmpPxl = scvGetPixelRef(image, x, y);
    if (NULL == tmpPxl) {
        return;
    }

    tmpPxl->b = pixel.b;
    tmpPxl->g = pixel.g;
    tmpPxl->r = pixel.r;
}

ScvSize scvGetSize(const ScvImage *image) {
    ScvSize size;
    size.width = image->width;
    size.height = image->height;
    return size;
}

ScvPoint scvGetCenter(const ScvImage *image) {
    return scvPoint(image->width / 2, image->height / 2);
}

#pragma mark -- Calculator

void scvCalcHist(const ScvImage *image, ScvHistogram *hist) {
    memset(hist->val, 0, 256 * sizeof(int));
    for (int iy = 0; iy < image->height; iy++) {
        for (int ix = 0; ix < image->width; ix++) {
            ScvPixel pxl = scvGetPixel(image, ix, iy);
            int index = grayValueOfPixel(pxl, hist->grayingType);
            if (index >= 0 && index < 256) {
                hist->val[index]++;
            }
        }
    }
}

#pragma mark -- Geometrical Transformation

void scvWarpAffine(const ScvImage *src, ScvImage *dst, const ScvMat *mat, ScvPixel fillPxl) {
    if (!(2 == mat->rows && 3 == mat->cols)) {
        /**
         * Must be:
         * [ a b | c ]
         * [ d e | f ]
         */
        return;
    }

    int cloned = 0;
    if (src == dst) {
        src = scvCloneImage(dst);
        cloned = 1;
    }

    scvFillImage(dst, fillPxl);

    float m[9];
    for (int i = 0; i < 6; i++) {
        m[i] = mat->data[i];
    }
    ScvMat stdMat = scvMat(3, 3, m);
    scvMatSetVal(&stdMat, 2, 0, 0.0f);
    scvMatSetVal(&stdMat, 2, 1, 0.0f);
    scvMatSetVal(&stdMat, 2, 2, 1.0f);

    float mInv[9];
    ScvMat stdMatInv = scvMat(3, 3, mInv);
    scvMatInverse(&stdMat, &stdMatInv);

    float pNew[3];
    ScvMat stdPntNew = scvMat(3, 1, pNew);
    scvMatSetVal(&stdPntNew, 2, 0, 1.0f);

    float pOrig[3];
    ScvMat stdPntOrig = scvMat(3, 1, pOrig);

    // Traverse the destination image
    for (int iy = 0; iy < dst->height; iy++) {
        for (int ix = 0; ix < dst->width; ix++) {
            /**
             * Calculate the origin point of
             * the current point on new image,
             * and fill the empty pixels using
             * Nearest Neighbor Interpolation algorithm.
             */

            scvMatSetVal(&stdPntNew, 0, 0, ix + 0.5f);
            scvMatSetVal(&stdPntNew, 1, 0, iy + 0.5f);

            scvMatDotProduct(&stdMatInv, &stdPntNew, &stdPntOrig);
            float fxO = scvMatGetVal(&stdPntOrig, 0, 0);
            float fyO = scvMatGetVal(&stdPntOrig, 1, 0);

            fxO += fxO > 0 ? 0 : -0.5f;
            fyO += fyO > 0 ? 0 : -0.5f;

            scvSetPixel(dst, ix, iy, scvGetPixel(src, (int) fxO, (int) fyO));
        }
    }

    if (cloned) {
        scvReleaseImage((ScvImage *) src);
    }
}

void scvRotationMatrix(ScvPoint center, float angle, ScvMat *mat) {
    if (!(2 == mat->rows && 3 == mat->cols)) {
        /**
         * Must be:
         * [ a b | c ]
         * [ d e | f ]
         */
        return;
    }

    const float rad = (float) (angle / 180 * PI);
    const float cos = cosf(rad);
    const float sin = sinf(rad);
    const int cx = center.x;
    const int cy = center.y;

    scvMatSetVal(mat, 0, 0, cos);
    scvMatSetVal(mat, 0, 1, sin);
    scvMatSetVal(mat, 0, 2, cos * (-cx) + sin * (-cy) + cx);
    scvMatSetVal(mat, 1, 0, -sin);
    scvMatSetVal(mat, 1, 1, cos);
    scvMatSetVal(mat, 1, 2, -sin * (-cx) + cos * (-cy) + cy);
}

void scvScaleMatrix(ScvPoint center, float scaleX, float scaleY, ScvMat *mat) {
    if (!(2 == mat->rows && 3 == mat->cols)) {
        /**
         * Must be:
         * [ a b | c ]
         * [ d e | f ]
         */
        return;
    }

    const float cx = center.x;
    const float cy = center.y;
    const float newCX = cx * scaleX;
    const float newCY = cy * scaleY;

    scvMatSetVal(mat, 0, 0, scaleX);
    scvMatSetVal(mat, 0, 1, 0);
    scvMatSetVal(mat, 0, 2, -(newCX - cx));
    scvMatSetVal(mat, 1, 0, 0);
    scvMatSetVal(mat, 1, 1, scaleY);
    scvMatSetVal(mat, 1, 2, -(newCY - cy));
}

void scvTranslationMatrix(float dx, float dy, ScvMat *mat) {
    if (!(2 == mat->rows && 3 == mat->cols)) {
        /**
         * Must be:
         * [ a b | c ]
         * [ d e | f ]
         */
        return;
    }

    scvMatSetVal(mat, 0, 0, 1.0f);
    scvMatSetVal(mat, 0, 1, 0);
    scvMatSetVal(mat, 0, 2, dx);
    scvMatSetVal(mat, 1, 0, 0);
    scvMatSetVal(mat, 1, 1, 1.0f);
    scvMatSetVal(mat, 1, 2, dy);
}

void scvFlipMatrix(ScvPoint center, SCV_FLIP_TYPE type, ScvMat *mat) {
    switch (type) {
        case SCV_FLIP_HORIZONTAL:
            scvScaleMatrix(center, -1.0f, 1.0f, mat);
            break;
        case SCV_FLIP_VERTICAL:
            scvScaleMatrix(center, 1.0f, -1.0f, mat);
            break;
        default:
            break;
    }
}

#pragma mark -- Point Transformation

void scvFillImage(ScvImage *image, ScvPixel fillPxl) {
    for (int iy = 0; iy < image->height; iy++) {
        for (int ix = 0; ix < image->width; ix++) {
            scvSetPixel(image, ix, iy, fillPxl);
        }
    }
}

void scvGraying(const ScvImage *src, ScvImage *dst, SCV_GRAYING_TYPE type) {
    for (int iy = 0; iy < src->height; iy++) {
        for (int ix = 0; ix < src->width; ix++) {
            ScvPixel sPxl = scvGetPixel(src, ix, iy);
            ScvPixel *dPxlRef = scvGetPixelRef(dst, ix, iy);
            if (NULL != dPxlRef) {
                int value = grayValueOfPixel(sPxl, type);
                if (value >= 0 && value < 256) {
                    dPxlRef->b = dPxlRef->g = dPxlRef->r = (ScvUByte) value;
                }
            }
        }
    }
}

void scvThreshold(const ScvImage *src, ScvImage *dst, SCV_GRAYING_TYPE grayingType) {
    ScvHistogram *hist = scvCreateHist(grayingType);
    scvCalcHist(src, hist);
    float thresh = thresholdOtsu(hist, src->width * src->height);
    scvReleaseHist(hist);
    for (int iy = 0; iy < src->height; iy++) {
        for (int ix = 0; ix < src->width; ix++) {
            ScvPixel sPxl = scvGetPixel(src, ix, iy);
            ScvPixel *dPxlRef = scvGetPixelRef(dst, ix, iy);
            if (NULL != dPxlRef) {
                int value = grayValueOfPixel(sPxl, grayingType);
                if (value >= 0 && value < 256) {
                    dPxlRef->b = dPxlRef->g = dPxlRef->r = (ScvUByte) (value > thresh ? 255 : 0);
                }
            }
        }
    }
}

void scvSplit(const ScvImage *src, ScvImage *b, ScvImage *g, ScvImage *r) {
    for (int iy = 0; iy < src->height; iy++) {
        for (int ix = 0; ix < src->width; ix++) {
            ScvPixel sPxl = scvGetPixel(src, ix, iy);
            scvSetPixel(b, ix, iy, scvPixelAll(sPxl.b));
            scvSetPixel(g, ix, iy, scvPixelAll(sPxl.g));
            scvSetPixel(r, ix, iy, scvPixelAll(sPxl.r));
        }
    }
}

void scvInverse(const ScvImage *src, ScvImage *dst) {
    for (int iy = 0; iy < src->height; iy++) {
        for (int ix = 0; ix < src->width; ix++) {
            ScvPixel sPxl = scvGetPixel(src, ix, iy);
            scvSetPixel(dst, ix, iy, scvPixel(255 - sPxl.b, 255 - sPxl.g, 255 - sPxl.r));
        }
    }
}

void scvEqualizeHist(const ScvImage *src, const ScvHistogram *hist, ScvImage *dst) {
    int cdf[256];
    calcCDF(256, hist->val, cdf);
    int cdfMin = 0;
    for (int i = 0; i < 256; i++) {
        if (0 != cdf[i]) {
            cdfMin = cdf[i];
            break;
        }
    }

    const int pixelCount = src->width * src->height;
    for (int iy = 0; iy < src->height; iy++) {
        for (int ix = 0; ix < src->width; ix++) {
            int value = grayValueOfPixel(scvGetPixel(src, ix, iy), hist->grayingType);
            if (value >= 0 && value < 256) {
                // Calculate the equalized value
                // https://en.wikipedia.org/wiki/Histogram_equalization
                int newVal = (int) (((float) cdf[value] - cdfMin) / (pixelCount - cdfMin) * 255 + 0.5f);
                scvSetPixel(dst, ix, iy, scvPixelAll(newVal));
            }
        }
    }
}

void scvSmooth(const ScvImage *src, ScvImage *dst, SCV_SMOOTH_TYPE type) {
    /**
     * [i][0] -> dx;
     * [i][1] -> dy;
     */
    int step[9][3] = {{-1, -1}, // Top-left
                      {0,  -1}, // Top
                      {1,  -1}, // Top-right
                      {-1, 0}, // Left
                      {0,  0}, // Center
                      {1,  0}, // Right
                      {-1, 1}, // Bottom-left
                      {0,  1}, // Bottom
                      {1,  1}}; // Bottom-right

    // Weight template
    int avgWeight[9] = {1, 1, 1,
                        1, 1, 1,
                        1, 1, 1};
    int gaussianWeight[9] = {1, 2, 1,
                             2, 4, 2,
                             1, 2, 1};

    int *weight;
    switch (type) {
        case SCV_SMOOTH_GAUSSIAN:
            weight = gaussianWeight;
            break;
        case SCV_SMOOTH_AVG:
        case SCV_SMOOTH_MEDIAN:
        default:
            weight = avgWeight;
            break;
    }

    int r, g, b;
    int arrR[9], arrG[9], arrB[9];
    int w[9];
    for (int iy = 0; iy < src->height; iy++) {
        for (int ix = 0; ix < src->width; ix++) {
            int count = 0;
            for (int i = 0; i < 9; i++) {
                int curX = ix + step[i][0];
                int curY = iy + step[i][1];
                if (curX >= 0 && curX < src->width
                    && curY >= 0 && curY < src->height) {
                    ScvPixel pxl = scvGetPixel(src, curX, curY);
                    arrR[count] = pxl.r;
                    arrG[count] = pxl.g;
                    arrB[count] = pxl.b;
                    w[count] = weight[i];
                    count++;
                }
            }

            switch (type) {
                case SCV_SMOOTH_GAUSSIAN:
                case SCV_SMOOTH_AVG:
                    r = (int) (avgArrWeighed(count, arrR, w) + 0.5f);
                    g = (int) (avgArrWeighed(count, arrG, w) + 0.5f);
                    b = (int) (avgArrWeighed(count, arrB, w) + 0.5f);
                    break;
                case SCV_SMOOTH_MEDIAN:
                    r = (int) (medianArr(count, arrR) + 0.5f);
                    g = (int) (medianArr(count, arrG) + 0.5f);
                    b = (int) (medianArr(count, arrB) + 0.5f);
                    break;
                default:
                    r = g = b = -1;
                    break;
            }
            if (r >= 0 && g >= 0 && b >= 0) {
                scvSetPixel(dst, ix, iy, scvPixel(b, g, r));
            }
        }
    }
}

void scvCanny(const ScvImage *image, ScvImage *path) {
    // http://blog.csdn.net/likezhaobin/article/details/6892629
    // The image is gray-scaled.
    int i, j, ix, iy;

    // Canny algorithm start

    // Gaussian filter
    ScvImage *filtered = scvCreateImage(scvGetSize(image));
    scvSmooth(image, filtered, SCV_SMOOTH_GAUSSIAN);

    int nWidth = image->width;
    int nHeight = image->height;

    float *P = (float *) malloc(nWidth * nHeight * sizeof(float));
    float *Q = (float *) malloc(nWidth * nHeight * sizeof(float));
    int *M = (int *) malloc(nWidth * nHeight * sizeof(int));
    float *theta = (float *) malloc(nWidth * nHeight * sizeof(float));

    for (iy = 0; iy < nHeight - 1; iy++) {
        for (ix = 0; ix < nWidth - 1; ix++) {
            P[iy * nWidth + ix] = (scvGetPixel(filtered, MIN(ix + 1, nWidth - 1), iy).r
                                   - scvGetPixel(filtered, ix, iy).r
                                   + scvGetPixel(filtered, MIN(ix + 1, nWidth - 1), MIN(iy + 1, nHeight - 1)).r
                                   - scvGetPixel(filtered, ix, MIN(iy + 1, nHeight - 1)).r) / 2.0f;
            Q[iy * nWidth + ix] = (scvGetPixel(filtered, ix, iy).r
                                   - scvGetPixel(filtered, ix, MIN(iy + 1, nHeight - 1)).r
                                   + scvGetPixel(filtered, MIN(ix + 1, nWidth - 1), iy).r
                                   - scvGetPixel(filtered, MIN(ix + 1, nWidth - 1), MIN(iy + 1, nHeight - 1)).r) / 2.0f;
        }
    }

    for (i = 0; i < nHeight; i++) {
        for (j = 0; j < nWidth; j++) {
            M[i * nWidth + j] = (int) (
                    sqrtf(P[i * nWidth + j] * P[i * nWidth + j] + Q[i * nWidth + j] * Q[i * nWidth + j]) + 0.5f);
            theta[i * nWidth + j] = atan2f(Q[i * nWidth + j], Q[i * nWidth + j]) * 57.3f;
            if (theta[i * nWidth + j] < 0)
                theta[i * nWidth + j] += 360;
        }
    }

    ScvUByte *N = (ScvUByte *) malloc(nWidth * nHeight * sizeof(ScvUByte));
    int g1 = 0, g2 = 0, g3 = 0, g4 = 0;
    float dTmp1 = 0.0, dTmp2 = 0.0;
    float dWeight;

    for (i = 0; i < nWidth; i++) {
        N[i] = 0;
        N[(nHeight - 1) * nWidth + i] = 0;
    }
    for (j = 0; j < nHeight; j++) {
        N[j * nWidth] = 0;
        N[j * nWidth + (nWidth - 1)] = 0;
    }

    for (i = 1; i < (nWidth - 1); i++) {
        for (j = 1; j < (nHeight - 1); j++) {
            int nPointIdx = i + j * nWidth;
            if (M[nPointIdx] == 0) {
                N[nPointIdx] = 0;
            }
            else {
                /////////////////////////////////////////////////////
                /////////       g1  g2                  /////////////
                /////////           C                   /////////////
                /////////           g3  g4              /////////////
                /////////////////////////////////////////////////////
                if (((theta[nPointIdx] >= 90) && (theta[nPointIdx] < 135)) ||
                    ((theta[nPointIdx] >= 270) && (theta[nPointIdx] < 315))) {
                    g1 = M[nPointIdx - nWidth - 1];
                    g2 = M[nPointIdx - nWidth];
                    g3 = M[nPointIdx + nWidth];
                    g4 = M[nPointIdx + nWidth + 1];
                    dWeight = fabsf(P[nPointIdx]) / fabsf(Q[nPointIdx]);
                    dTmp1 = g1 * dWeight + g2 * (1 - dWeight);
                    dTmp2 = g4 * dWeight + g3 * (1 - dWeight);
                }
                    /////////////////////////////////////////////////////
                    /////////       g1                      /////////////
                    /////////       g2  C   g3              /////////////
                    /////////               g4              /////////////
                    /////////////////////////////////////////////////////
                else if (((theta[nPointIdx] >= 135) && (theta[nPointIdx] < 180)) ||
                         ((theta[nPointIdx] >= 315) && (theta[nPointIdx] < 360))) {
                    g1 = M[nPointIdx - nWidth - 1];
                    g2 = M[nPointIdx - 1];
                    g3 = M[nPointIdx + 1];
                    g4 = M[nPointIdx + nWidth + 1];
                    dWeight = fabsf(Q[nPointIdx]) / fabsf(P[nPointIdx]);
                    dTmp1 = g2 * dWeight + g1 * (1 - dWeight);
                    dTmp2 = g4 * dWeight + g3 * (1 - dWeight);
                }
                    /////////////////////////////////////////////////////
                    /////////           g1  g2              /////////////
                    /////////           C                   /////////////
                    /////////       g4  g3                  /////////////
                    /////////////////////////////////////////////////////
                else if (((theta[nPointIdx] >= 45) && (theta[nPointIdx] < 90)) ||
                         ((theta[nPointIdx] >= 225) && (theta[nPointIdx] < 270))) {
                    g1 = M[nPointIdx - nWidth];
                    g2 = M[nPointIdx - nWidth + 1];
                    g3 = M[nPointIdx + nWidth];
                    g4 = M[nPointIdx + nWidth - 1];
                    dWeight = fabsf(P[nPointIdx]) / fabsf(Q[nPointIdx]);
                    dTmp1 = g2 * dWeight + g1 * (1 - dWeight);
                    dTmp2 = g3 * dWeight + g4 * (1 - dWeight);
                }
                    /////////////////////////////////////////////////////
                    /////////               g1              /////////////
                    /////////       g4  C   g2              /////////////
                    /////////       g3                      /////////////
                    /////////////////////////////////////////////////////
                else if (((theta[nPointIdx] >= 0) && (theta[nPointIdx] < 45)) ||
                         ((theta[nPointIdx] >= 180) && (theta[nPointIdx] < 225))) {
                    g1 = M[nPointIdx - nWidth + 1];
                    g2 = M[nPointIdx + 1];
                    g3 = M[nPointIdx + nWidth - 1];
                    g4 = M[nPointIdx - 1];
                    dWeight = fabsf(Q[nPointIdx]) / fabsf(P[nPointIdx]);
                    dTmp1 = g1 * dWeight + g2 * (1 - dWeight);
                    dTmp2 = g3 * dWeight + g4 * (1 - dWeight);
                }
            }
            if ((M[nPointIdx] >= dTmp1) && (M[nPointIdx] >= dTmp2)) {
                N[nPointIdx] = 128;
            }
            else {
                N[nPointIdx] = 0;
            }
        }
    }

    int nHist[1024];
    int nEdgeNum;
    int nMaxMag = 0;
    int nHighCount;

    for (i = 0; i < 1024; i++) {
        nHist[i] = 0;
    }
    for (i = 0; i < nHeight; i++) {
        for (j = 0; j < nWidth; j++) {
            if (N[i * nWidth + j] == 128)
                nHist[M[i * nWidth + j]]++;
        }
    }

    nEdgeNum = nHist[0];
    nMaxMag = 0;
    for (i = 1; i < 1024; i++) {
        if (nHist[i] != 0) {
            nMaxMag = i;
        }
        nEdgeNum += nHist[i];
    }

    float dRatHigh = 0.79;
    float dThrHigh;
    float dThrLow;
    float dRatLow = 0.5;
    nHighCount = (int) (dRatHigh * nEdgeNum + 0.5f);
    j = 1;
    nEdgeNum = nHist[1];
    while ((j < (nMaxMag - 1)) && (nEdgeNum < nHighCount)) {
        j++;
        nEdgeNum += nHist[j];
    }
    dThrHigh = j;
    dThrLow = (int) ((dThrHigh) * dRatLow + 0.5f);

    ScvSize sz;
    sz.width = nWidth;
    sz.height = nHeight;
    for (i = 0; i < nHeight; i++) {
        for (j = 0; j < nWidth; j++) {
            if ((N[i * nWidth + j] == 128) && (M[i * nWidth + j] >= dThrHigh)) {
                N[i * nWidth + j] = 255;
                traceEdge(i, j, (int) (dThrLow + 0.5f), N, M, sz);
            }
        }
    }

    // Canny algorithm end

    for (iy = 0; iy < nHeight; iy++) {
        for (ix = 0; ix < nWidth; ix++) {
            if (N[iy * nWidth + ix] != 255) {
                scvSetPixel(path, ix, iy, scvPixelAll(0));
            } else {
                scvSetPixel(path, ix, iy, scvPixelAll(255));
            }
        }
    }

    scvReleaseImage(filtered);
    free(P);
    free(Q);
    free(M);
    free(theta);
    free(N);
}

void scvAddWeighed(const ScvImage *src1, float alpha, const ScvImage *src2, float beta, ScvImage *dst) {
    float rate = 1.0f / (alpha + beta);
    alpha *= rate;
    beta *= rate;

    for (int iy = 0; iy < dst->height; iy++) {
        for (int ix = 0; ix < dst->width; ix++) {
            if (ix < src1->width && iy < src1->height
                && ix < src2->width && iy < src2->height) {
                // In range of both src1 and src2
                ScvPixel pxl1 = scvGetPixel(src1, ix, iy);
                ScvPixel pxl2 = scvGetPixel(src2, ix, iy);
                scvSetPixel(dst, ix, iy, scvPixel(
                        (int) (alpha * pxl1.b + beta * pxl2.b),
                        (int) (alpha * pxl1.g + beta * pxl2.g),
                        (int) (alpha * pxl1.r + beta * pxl2.r)));
            } else if (ix < src1->width && iy < src1->height) {
                // In range of src1
                scvSetPixel(dst, ix, iy, scvGetPixel(src1, ix, iy));
            } else if (ix < src2->width && iy < src2->height) {
                // In range of src2
                scvSetPixel(dst, ix, iy, scvGetPixel(src2, ix, iy));
            }
        }
    }
}
