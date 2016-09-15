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

void graying();

void grayScale();

void grayReverse();

void grayBinary();

void splitRGB();

int main(int argc, char **argv) {
    printf("\n\n---------数字图像处理 点运算 v1.0.0---------\n\n");

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
                       "1. 灰度化\n"
                       "2. 灰度图像拉伸\n"
                       "3. 灰度图像反色\n"
                       "4. 灰度图像二值化\n"
                       "5. 提取 RGB 通道\n");
        printf("请输入选项: ");
        scanf("%d", &choice);
        switch (choice) {
            case 0:
                break;
            case 1:
                graying();
                break;
            case 2:
                grayScale();
                break;
            case 3:
                grayReverse();
                break;
            case 4:
                grayBinary();
                break;
            case 5:
                splitRGB();
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

void graying() {
    doGraying();

    if (gOutputBmp) {
        writeImageToBmp(gGrayImage, gOutFilePath);
    }

    cvShowImage("Source", gSrcImage);
    cvShowImage("Gray", gGrayImage);
    cvWaitKey(0);
}

void grayScale() {
    doGraying();
    IplImage *grayScaleImg = cvCreateImage(cvGetSize(gGrayImage), gGrayImage->depth, gGrayImage->nChannels);
    cvEqualizeHist(gGrayImage, grayScaleImg);

    if (gOutputBmp) {
        writeImageToBmp(grayScaleImg, gOutFilePath);
    }

    cvShowImage("Source", gSrcImage);
    cvShowImage("Gray", gGrayImage);
    cvShowImage("Gray Scaled", grayScaleImg);
    cvWaitKey(0);
}

void grayReverse() {
    doGraying();
    IplImage *grayReverseImg = cvCreateImage(cvGetSize(gGrayImage), gGrayImage->depth, gGrayImage->nChannels);
    for (int i = 0; i < gGrayImage->height; i++) {
        for (int j = 0; j < gGrayImage->width; j++) {
            CvScalar scalar = cvGet2D(gGrayImage, i, j);
            for (int k = 0; k < gGrayImage->nChannels; k++) {
                scalar.val[k] = 255 - scalar.val[k];
            }
            cvSet2D(grayReverseImg, i, j, scalar);
        }
    }

    if (gOutputBmp) {
        writeImageToBmp(grayReverseImg, gOutFilePath);
    }

    cvShowImage("Source", gSrcImage);
    cvShowImage("Gray", gGrayImage);
    cvShowImage("Gray Reversed", grayReverseImg);
    cvWaitKey(0);
}

void grayBinary() {
    doGraying();
    IplImage *grayBinImg = cvCreateImage(cvGetSize(gGrayImage), gGrayImage->depth, gGrayImage->nChannels);
    cvThreshold(gGrayImage, grayBinImg, 0, 255, CV_THRESH_OTSU | CV_THRESH_BINARY);

    if (gOutputBmp) {
        writeImageToBmp(grayBinImg, gOutFilePath);
    }

    cvShowImage("Source", gSrcImage);
    cvShowImage("Gray", gGrayImage);
    cvShowImage("Gray Binary", grayBinImg);
    cvWaitKey(0);
}

void splitRGB() {
    IplImage *b = cvCreateImage(cvGetSize(gSrcImage), gSrcImage->depth, 1);
    IplImage *g = cvCloneImage(b);
    IplImage *r = cvCloneImage(b);
    cvSplit(gSrcImage, b, g, r, NULL);
    cvShowImage("Source", gSrcImage);
    cvShowImage("Blue", b);
    cvShowImage("Green", g);
    cvShowImage("Red", r);
    cvWaitKey(0);
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
