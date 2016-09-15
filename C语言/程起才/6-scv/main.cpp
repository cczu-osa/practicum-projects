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
// Created by Richard Chien on 6/25/16.
//

#include <stdio.h>
#include <string.h>
#include "simplecv/scv.h"

char *gOutFilePath;
char *gSrcFilePath;
ScvImage *gSrcImage;
ScvImage *gGrayImage;

const int TASK_FLAG_GRAYING = 0x10;
const int TASK_ROTATE = 0x01;
const int TASK_FLIP = 0x02;

typedef struct _TaskMessage {
    int what;
    union {
        int intVal;
        float floatVal;
        void *obj;
    } val;
} TaskMessage;

void dispatchTask(TaskMessage msg);

int main(int argc, char **argv) {
    printf("\n\n---------数字图像处理 几何变换1 v1.0.0---------\n\n");

    if (argc == 1) {
        printf("用法: command-name <source-file-path> -o <output-file-path>\n");
        return 0;
    } else if (0 == strcmp("-o", argv[2]) && 4 == argc) {
        gSrcFilePath = argv[1];
        gOutFilePath = argv[3];
    } else {
        printf("参数有误.\n");
        return 1;
    }

    gSrcImage = scvLoadImage(gSrcFilePath);

    int choice;
    // Main loop
    for (; ;) {
        printf("0. 退出\n"
                       "1. 旋转灰度图像\n"
                       "2. 旋转彩色图像\n"
                       "3. 灰度水平镜像\n"
                       "4. 彩色水平镜像\n"
                       "5. 灰度垂直镜像\n"
                       "6. 彩色垂直镜像\n");
        printf("\n请输入选项: ");
        scanf("%d", &choice);
        TaskMessage msg = {0};
        switch (choice) {
            case 0:
                return 0;
            case 1:
                msg.what |= TASK_FLAG_GRAYING;
            case 2:
                msg.what |= TASK_ROTATE;
                printf("请输入要旋转的角度: ");
                scanf("%f", &msg.val.floatVal);
                dispatchTask(msg);
                printf("操作成功! 变换后的图像已经输出到 %s.\n", gOutFilePath);
                break;
            case 3:
                msg.what |= TASK_FLAG_GRAYING;
            case 4:
                msg.what |= TASK_FLIP;
                msg.val.intVal = SCV_FLIP_HORIZONTAL;
                dispatchTask(msg);
                printf("操作成功! 变换后的图像已经输出到 %s.\n", gOutFilePath);
                break;
            case 5:
                msg.what |= TASK_FLAG_GRAYING;
            case 6:
                msg.what |= TASK_FLIP;
                msg.val.intVal = SCV_FLIP_VERTICAL;
                dispatchTask(msg);
                printf("操作成功! 变换后的图像已经输出到 %s.\n", gOutFilePath);
                break;
            default:
                // Prompt again.
                break;
        }
        printf("\n");
    }
}

void saveImage(ScvImage *image, const char *path) {
    scvSaveImage(image, path);
}

void rotateImage(ScvImage *src, float angle) {
    ScvMat *mat = scvCreateMat(2, 3);
    scvRotationMatrix(scvGetCenter(src), angle, mat);
    scvWarpAffine(src, src, mat, scvPixelAll(0));
}

void flipImage(ScvImage *src, SCV_FLIP_TYPE type) {
    ScvMat *mat = scvCreateMat(2, 3);
    scvFlipMatrix(scvGetCenter(src), type, mat);
    scvWarpAffine(src, src, mat, scvPixelAll(0));
}

void dispatchTask(TaskMessage msg) {
    ScvImage *image;

    if (msg.what & TASK_FLAG_GRAYING) {
        if (gGrayImage == NULL) {
            gGrayImage = scvCreateImage(scvGetSize(gSrcImage));
            scvGraying(gSrcImage, gGrayImage, SCV_GRAYING_W_AVG);
        }
        image = scvCloneImage(gGrayImage);
        msg.what &= ~TASK_FLAG_GRAYING;
    } else {
        image = scvCloneImage(gSrcImage);
    }

    switch (msg.what) {
        case TASK_ROTATE:
            rotateImage(image, msg.val.floatVal);
            break;
        case TASK_FLIP:
            flipImage(image, (SCV_FLIP_TYPE) msg.val.intVal);
            break;
        default:
            break;
    }

    saveImage(image, gOutFilePath);
    scvReleaseImage(image);
}
