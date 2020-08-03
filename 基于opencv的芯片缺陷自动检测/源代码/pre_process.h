#ifndef _PRE_PROCESS_H
#define _PRE_PROCESS_H

#include "common.h"



Mat extract_chip(Mat Soupic);
int detect_rows(Mat pic, int mode);
int detect_cols(Mat pic, int mode);
Mat get_product(Mat pic);
void copy_chip2_separated_pic(Mat pic, vector<Rect> Rect_vector, vector<Mat> &pic_list); //��һ��ͼƬ�еĶ��оƬ���뵽������ͼ����ȥ������vector����
void spilt_product(Mat pic);  //�����Ʒ
Mat rotate_pic(Mat pic, float degree); //  ��תоƬ
Mat rotate_judge(Mat pic); //�жϵ�ǰоƬ�Ƿ���Ҫ��ת

#endif