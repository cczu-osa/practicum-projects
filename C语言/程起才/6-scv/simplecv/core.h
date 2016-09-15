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

#include "types.h"

#ifndef SIMPLECV_CORE_H
#define SIMPLECV_CORE_H

#pragma mark - Make

ScvMat *scvCreateMat(int rows, int cols);

ScvMat *scvCloneMat(const ScvMat *mat);

void scvCopyMat(const ScvMat *src, ScvMat *dst);

void scvReleaseMat(ScvMat *mat);

ScvImage *scvCreateImage(ScvSize size);

ScvImage *scvCloneImage(const ScvImage *image);

void scvCopyImage(const ScvImage *src, ScvImage *dst);

void scvReleaseImage(ScvImage *image);

ScvHistogram *scvCreateHist(SCV_GRAYING_TYPE grayingType);

ScvHistogram *scvCloneHist(const ScvHistogram *histogram);

void scvCopyHist(const ScvHistogram *src, ScvHistogram *dst);

void scvReleaseHist(ScvHistogram *histogram);

#pragma mark - Getter and Setter

ScvPixel *scvGetPixelRef(const ScvImage *image, int x, int y);

ScvPixel scvGetPixel(const ScvImage *image, int x, int y);

void scvSetPixel(ScvImage *image, int x, int y, ScvPixel pixel);

ScvSize scvGetSize(const ScvImage *image);

ScvPoint scvGetCenter(const ScvImage *image);

#pragma mark - Calculator

void scvCalcHist(const ScvImage *image, ScvHistogram *hist);

#pragma mark - Geometrical Transformation

void scvWarpAffine(const ScvImage *src, ScvImage *dst, const ScvMat *mat, ScvPixel fillPxl);

void scvRotationMatrix(ScvPoint center, float angle, ScvMat *mat);

void scvScaleMatrix(ScvPoint center, float scaleX, float scaleY, ScvMat *mat);

void scvTranslationMatrix(float dx, float dy, ScvMat *mat);

void scvFlipMatrix(ScvPoint center, SCV_FLIP_TYPE type, ScvMat *mat);

#pragma mark - Point Transformation

void scvFillImage(ScvImage *image, ScvPixel fillPxl);

void scvGraying(const ScvImage *src, ScvImage *dst, SCV_GRAYING_TYPE type);

/**
 * Applies fixed-level threshold to gray-scale image.
 * If the image is not gray-scaled, graying it with the parameter grayingType.
 */
void scvThreshold(const ScvImage *src, ScvImage *dst, SCV_GRAYING_TYPE grayingType);

void scvSplit(const ScvImage *src, ScvImage *b, ScvImage *g, ScvImage *r);

void scvInverse(const ScvImage *src, ScvImage *dst);

void scvEqualizeHist(const ScvImage *src, const ScvHistogram *hist, ScvImage *dst);

void scvSmooth(const ScvImage *src, ScvImage *dst, SCV_SMOOTH_TYPE type);

// The image passed in must be gray-scaled image.
void scvCanny(const ScvImage *image, ScvImage *path);

void scvAddWeighed(const ScvImage *src1, float alpha, const ScvImage *src2, float beta, ScvImage *dst);

#endif //SIMPLECV_CORE_H
