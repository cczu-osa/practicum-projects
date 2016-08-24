#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <opencv2/core/types_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <cv.h>
#include "LogUtils/log_utils.h"
#include "my_math.h"
#include "bitmap.h"

char *gOutFilePath;
char *gSrcFilePath;
IplImage *gSrcImage;
IplImage *gGrayImage;
bool gOutputBmp;

void doGraying();

void moveGray();

void moveOriginal();

void scaleKeepRatioGray();

void scaleKeepRatioOriginal();

void promptMove(IplImage *src);

void realScale(IplImage *src, IplImage *dst, float scaleX, float scaleY);

void promptScaleKeepRatio(IplImage *src);

void promptScale(IplImage *src);

void scaleGray();

void scaleOriginal();

int main(int argc, char **argv) {
    printf("\n\n---------数字图像处理 几何变换2 v1.0.0---------\n\n");

    if (argc <= 1) {
        printf("用法: 5 [-o <output-file-path>] <source-file-path>\n");
        printf("其中 -o <output-file-path> 参数用来指定输出的 bmp 文件的路径.\n");
        printf("如果不指定该参数, 则不输出 bmp.\n");
        return 0;
    }

    if (0 == strcmp("-o", argv[1]) && 4 == argc) {
        gOutFilePath = argv[2];
        gSrcFilePath = argv[3];
        gOutputBmp = true;
        log_d("main()", "outfile path: %s", gOutFilePath);
    } else if (2 == argc) {
        gSrcFilePath = argv[1];
        gOutputBmp = false;
    } else {
        printf("参数错误!");
        exit(1);
    }

    gSrcImage = cvLoadImage(gSrcFilePath, CV_LOAD_IMAGE_COLOR);

    int choice;
    // Main loop
    for (; ;) {
        printf("0. 退出\n"
                       "1. 平移灰度图像\n"
                       "2. 平移彩色图像\n"
                       "3. 等比例缩放灰度图像\n"
                       "4. 等比例缩放彩色图像\n"
                       "5. 非等比例缩放灰度图像\n"
                       "6. 非等比例缩放彩色图像\n");
        printf("请输入选项: ");
        scanf("%d", &choice);
        switch (choice) {
            case 0:
                break;
            case 1:
                moveGray();
                break;
            case 2:
                moveOriginal();
                break;
            case 3:
                scaleKeepRatioGray();
                break;
            case 4:
                scaleKeepRatioOriginal();
                break;
            case 5:
                scaleGray();
                break;
            case 6:
                scaleOriginal();
                break;
            default:
                // Prompt again.
                continue;
        }

        // Only be reached when input is correct.
        // If default has been matched in switch,
        // the following return clause will be skipped.
        return 0;
    }
}

void moveGray() {
    doGraying();
    promptMove(gGrayImage);
}

void moveOriginal() {
    promptMove(gSrcImage);
}

void scaleKeepRatioGray() {
    doGraying();
    promptScaleKeepRatio(gGrayImage);
}

void scaleKeepRatioOriginal() {
    promptScaleKeepRatio(gSrcImage);
}

void scaleGray() {
    doGraying();
    promptScale(gGrayImage);
}

void scaleOriginal() {
    promptScale(gSrcImage);
}

void doGraying() {
    if (gGrayImage == NULL) {
        // Only need to make gray image for one time.
        gGrayImage = cvCreateImage(cvGetSize(gSrcImage), gSrcImage->depth, 1);
        for (int i = 0; i < gSrcImage->height; i++) {
            for (int j = 0; j < gSrcImage->width; j++) {
                CvScalar scalar = cvGet2D(gSrcImage, i, j);
                // Calculate average value of blue, green and red channels.
                scalar.val[0] = avg_double(3, scalar.val[0], scalar.val[1], scalar.val[2]);
                cvSet2D(gGrayImage, i, j, scalar);
            }
        }
    }
}

void promptMove(IplImage *src) {
    float detX, detY;
    printf("请输入 X, Y 方向上的平移距离(用空格隔开): ");
    scanf("%f %f", &detX, &detY);

    float m[6] = {1.0f, 0, detX,
                  0, 1.0f, detY};
    CvMat mat = cvMat(2, 3, CV_32F, m);
    IplImage *result = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    cvWarpAffine(src, result, &mat, CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS, cvScalarAll(0));

    if (gOutputBmp) {
        writeImageToBmp(result, gOutFilePath);
    }

    cvShowImage("Source", src);
    cvShowImage("Moved", result);
    cvWaitKey(0);
}

void promptScaleKeepRatio(IplImage *src) {
    float scale;
    printf("请输入缩放比例(1 表示不缩放): ");
    scanf("%f", &scale);

    IplImage *result = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    realScale(src, result, scale, scale);

    if (gOutputBmp) {
        writeImageToBmp(result, gOutFilePath);
    }

    cvShowImage("Source", src);
    cvShowImage("Scaled", result);
    cvWaitKey(0);
}

void promptScale(IplImage *src) {
    float scaleX, scaleY;
    printf("请输入 X, Y 方向的缩放比例(空格隔开, 1 表示不缩放): ");
    scanf("%f %f", &scaleX, &scaleY);

    IplImage *result = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    realScale(src, result, scaleX, scaleY);

    if (gOutputBmp) {
        writeImageToBmp(result, gOutFilePath);
    }

    cvShowImage("Source", src);
    cvShowImage("Scaled", result);
    cvWaitKey(0);
}

void realScale(IplImage *src, IplImage *dst, float scaleX, float scaleY) {
    float m[6] = {scaleX, 0, (1.0f - scaleX) * src->width / 2.0f,
                  0, scaleY, (1.0f - scaleY) * src->height / 2.0f};
    CvMat mat = cvMat(2, 3, CV_32F, m);
    cvWarpAffine(src, dst, &mat, CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS, cvScalarAll(0));
}

