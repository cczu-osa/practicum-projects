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

#include "matrix.h"
#include "core.h"

#pragma mark - Export

float scvMatGetVal(const ScvMat *mat, int i, int j) {
    if (i < 0 || i >= mat->rows || j < 0 || j >= mat->cols) {
        return 0;
    }
    return mat->data[i * mat->cols + j];
}

void scvMatSetVal(ScvMat *mat, int i, int j, float value) {
    if (i < 0 || i >= mat->rows || j < 0 || j >= mat->cols) {
        return;
    }
    mat->data[i * mat->cols + j] = value;
}

void scvMatDotProduct(const ScvMat *left, const ScvMat *right, ScvMat *dst) {
    if (left->cols != right->rows || dst->rows != left->rows || dst->cols != right->cols) {
        // Cannot calculate product
        return;
    }

    int cloned = 0;
    if (left == dst) {
        left = scvCloneMat(dst);
        cloned |= (1 << 1);
    }
    if (right == dst) {
        right = scvCloneMat(dst);
        cloned |= 1;
    }

    const int n = left->cols;
    for (int i = 0; i < dst->rows; i++) {
        for (int j = 0; j < dst->cols; j++) {
            float cur = 0;
            for (int k = 0; k < n; k++) {
                cur += scvMatGetVal(left, i, k) * scvMatGetVal(right, k, j);
            }
            scvMatSetVal(dst, i, j, cur);
        }
    }

    if (cloned & (1 << 1)) {
        scvReleaseMat((ScvMat *) left);
    }
    if (cloned & 1) {
        scvReleaseMat((ScvMat *) right);
    }
}

void scvMatNumProduct(float k, const ScvMat *mat, ScvMat *dst) {
    if (mat->rows != dst->rows || mat->cols != dst->cols) {
        // Cannot calculate
        return;
    }

    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            scvMatSetVal(dst, i, j, k * scvMatGetVal(mat, i, j));
        }
    }
}

void scvMatInverse(const ScvMat *src, ScvMat *dst) {
    if (src->rows != src->cols || dst->rows != dst->cols || src->rows != dst->rows) {
        // Cannot calculate
        return;
    }

    const int n = src->rows;
    float det = scvMatDet(src);
    ScvMat *adj = scvCreateMat(n, n);
    scvMatAdjugate(src, adj);
    scvMatNumProduct(1 / det, adj, dst);
    scvReleaseMat(adj);
}

float scvMatDet(const ScvMat *mat) {
    if (mat->rows != mat->cols) {
        // Must be square matrix
        return 0;
    }

    if (1 == mat->rows) {
        return mat->data[0];
    }

    if (2 == mat->rows) {
        float *m = mat->data;
        return m[0] * m[3] - m[1] * m[2];
    }

    if (3 == mat->rows) {
        float *m = mat->data;
        return m[0] * m[4] * m[8] + m[3] * m[7] * m[2] + m[6] * m[1] * m[5]
               - m[2] * m[4] * m[6] - m[5] * m[7] * m[0] - m[8] * m[1] * m[3];
    }

    const int minorRows = mat->rows - 1;
    float result = 0;
    int sign = 1;
    for (int i = 0; i < mat->rows; i++) {
        ScvMat *curMinor = scvCreateMat(minorRows, minorRows);
        scvMatMinor(mat, curMinor, i, 0);
        result += scvMatGetVal(mat, i, 0) * sign * scvMatDet(curMinor);
        sign = -sign;
        scvReleaseMat(curMinor);
    }
    return result;
}

/**
 * Obtain minor matrix by removing just one row and one column.
 */
void scvMatMinor(const ScvMat *src, ScvMat *dst, int i, int j) {
    if (src->rows != src->cols || dst->rows != dst->cols || src->rows != dst->rows + 1) {
        // Cannot calculate
        return;
    }

    for (int k = 0, nowI = 0; k < dst->rows; nowI++) {
        if (nowI == i) {
            // Skip the selected row
            continue;
        }

        for (int l = 0, nowJ = 0; l < dst->cols; nowJ++) {
            if (nowJ == j) {
                // Skip the selected column
                continue;
            }

            scvMatSetVal(dst, k, l, scvMatGetVal(src, nowI, nowJ));
            l++;
        }
        k++;
    }
}

void scvMatAdjugate(const ScvMat *src, ScvMat *dst) {
    if (src->rows != src->cols || dst->rows != dst->cols || src->rows != dst->rows) {
        // Cannot calculate
        return;
    }

    int cloned = 0;
    if (src == dst) {
        src = scvCloneMat(dst);
        cloned = 1;
    }

    const int n = dst->rows;
    ScvMat *minor = scvCreateMat(n - 1, n - 1);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int sign = (i + j) % 2 == 0 ? 1 : -1;
            scvMatMinor(src, minor, j, i);
            scvMatSetVal(dst, i, j, sign * scvMatDet(minor));
        }
    }
    scvReleaseMat(minor);

    if (cloned) {
        scvReleaseMat((ScvMat *) src);
    }
}

void scvMatTranspose(const ScvMat *src, ScvMat *dst) {
    if (src->rows != dst->cols || src->cols != dst->rows) {
        // Cannot transpose
        return;
    }

    int cloned = 0;
    if (src == dst) {
        src = scvCloneMat(dst);
        cloned = 1;
    }

    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            scvMatSetVal(dst, j, i, scvMatGetVal(src, i, j));
        }
    }

    if (cloned) {
        scvReleaseMat((ScvMat *) src);
    }
}
