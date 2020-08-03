#ifndef _PRE_PROCESS_H
#define _PRE_PROCESS_H

#include "common.h"



Mat extract_chip(Mat Soupic);
int detect_rows(Mat pic, int mode);
int detect_cols(Mat pic, int mode);
Mat get_product(Mat pic);
void copy_chip2_separated_pic(Mat pic, vector<Rect> Rect_vector, vector<Mat> &pic_list); //将一个图片中的多个芯片分离到单独的图像中去，并用vector保存
void spilt_product(Mat pic);  //分离产品
Mat rotate_pic(Mat pic, float degree); //  旋转芯片
Mat rotate_judge(Mat pic); //判断当前芯片是否需要旋转

#endif