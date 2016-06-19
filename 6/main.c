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

void rotateGray();

void rotateOriginal();

void promptRotate(IplImage *src);

void horizontalFlipGray();

void promptHorizontalFlip(IplImage *src);

void horizontalFlipOriginal();

void promptVerticalFlip(IplImage *src);

void verticalFlipGray();

void verticalFlipOriginal();

int main(int argc, char **argv) {
    printf("\n\n---------数字图像处理 几何变换1 v1.0.0---------\n\n");

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
                       "1. 旋转灰度图像\n"
                       "2. 旋转彩色图像\n"
                       "3. 灰度水平镜像\n"
                       "4. 彩色水平镜像\n"
                       "5. 灰度垂直镜像\n"
                       "6. 彩色垂直镜像\n");
        printf("请输入选项: ");
        scanf("%d", &choice);
        switch (choice) {
            case 0:
                break;
            case 1:
                rotateGray();
                break;
            case 2:
                rotateOriginal();
                break;
            case 3:
                horizontalFlipGray();
                break;
            case 4:
                horizontalFlipOriginal();
                break;
            case 5:
                verticalFlipGray();
                break;
            case 6:
                verticalFlipOriginal();
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

void rotateGray() {
    doGraying();
    promptRotate(gGrayImage);
}

void rotateOriginal() {
    promptRotate(gSrcImage);
}

void horizontalFlipGray() {
    doGraying();
    promptHorizontalFlip(gGrayImage);
}

void horizontalFlipOriginal() {
    promptHorizontalFlip(gSrcImage);
}

void verticalFlipGray() {
    doGraying();
    promptVerticalFlip(gGrayImage);
}

void verticalFlipOriginal() {
    promptVerticalFlip(gSrcImage);
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

void promptRotate(IplImage *src) {
    double angle;
    printf("请输入要旋转的角度: ");
    scanf("%lf", &angle);

    float m[6];
    CvMat rotMat = cvMat(2, 3, CV_32F, m);
    CvPoint2D32f center;
    center.x = (float) (src->width / 2.0 + 0.5);
    center.y = (float) (src->height / 2.0 + 0.5);
    cv2DRotationMatrix(center, -angle, 1, &rotMat);
    IplImage *result = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    cvWarpAffine(src, result, &rotMat, CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS, cvScalarAll(0));

    if (gOutputBmp) {
        writeImageToBmp(result, gOutFilePath);
    }

    cvShowImage("Source", src);
    cvShowImage("Rotated", result);
    cvWaitKey(0);
}

void promptHorizontalFlip(IplImage *src) {
    float m[6] = {-1.0f, 0, (float) src->width,
                  0, 1.0f, 0};
    CvMat flipMat = cvMat(2, 3, CV_32F, m);
    IplImage *flipped = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    cvWarpAffine(src, flipped, &flipMat, CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS, cvScalarAll(0));

    if (gOutputBmp) {
        writeImageToBmp(flipped, gOutFilePath);
    }

    cvShowImage("Source", src);
    cvShowImage("Flipped", flipped);
    cvWaitKey(0);
}

void promptVerticalFlip(IplImage *src) {
    float m[6] = {1.0f, 0, 0,
                  0, -1.0f, (float) src->height};
    CvMat flipMat = cvMat(2, 3, CV_32F, m);
    IplImage *flipped = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    cvWarpAffine(src, flipped, &flipMat, CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS, cvScalarAll(0));

    if (gOutputBmp) {
        writeImageToBmp(flipped, gOutFilePath);
    }

    cvShowImage("Source", src);
    cvShowImage("Flipped", flipped);
    cvWaitKey(0);
}
