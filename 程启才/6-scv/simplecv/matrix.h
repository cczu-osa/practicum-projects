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
// Created by Richard Chien on 6/21/16.
//

#include "types.h"

#ifndef SIMPLECV_MATRIX_H
#define SIMPLECV_MATRIX_H

float scvMatGetVal(const ScvMat *mat, int i, int j);

void scvMatSetVal(ScvMat *mat, int i, int j, float value);

void scvMatDotProduct(const ScvMat *left, const ScvMat *right, ScvMat *dst);

void scvMatNumProduct(float k, const ScvMat *mat, ScvMat *dst);

void scvMatInverse(const ScvMat *src, ScvMat *dst);

float scvMatDet(const ScvMat *mat);

void scvMatMinor(const ScvMat *src, ScvMat *dst, int i, int j);

void scvMatAdjugate(const ScvMat *src, ScvMat *dst);

void scvMatTranspose(const ScvMat *src, ScvMat *dst);

#endif //SIMPLECV_MATRIX_H
