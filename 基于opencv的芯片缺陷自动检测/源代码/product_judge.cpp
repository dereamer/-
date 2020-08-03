#include "product_judge.h"
int myOtsu(Mat & src)
{
	int th;
	const int GrayScale = 256;	//单通道图像总灰度256级
	int pixCount[GrayScale] = { 0 };//每个灰度值所占像素个数
	int pixSum = src.cols * src.rows;//图像总像素点
	float pixPro[GrayScale] = { 0 };//每个灰度值所占总像素比例
	float w0, w1, u0tmp, u1tmp, u0, u1, deltaTmp, deltaMax = 0;
	for (int i = 0; i < src.cols; i++)
	{
		for (int j = 0; j < src.rows; j++)
		{
			pixCount[src.at<uchar>(j, i)]++;//统计每个灰度级中像素的个数  
		}
	}

	for (int i = 0; i < GrayScale; i++)
	{
		pixPro[i] = pixCount[i] * 1.0 / pixSum;//计算每个灰度级的像素数目占整幅图像的比例  
	}

	for (int i = 0; i < GrayScale; i++)//遍历所有从0到255灰度级的阈值分割条件，测试哪一个的类间方差最大
	{
		w0 = w1 = u0tmp = u1tmp = u0 = u1 = deltaTmp = 0;
		for (int j = 0; j < GrayScale; j++)
		{
			if (j <= i)//背景
			{
				w0 += pixPro[j];
				u0tmp += j * pixPro[j];
			}
			else//前景
			{
				w1 += pixPro[j];
				u1tmp += j * pixPro[j];
			}
		}
		u0 = u0tmp / w0;
		u1 = u1tmp / w1;
		deltaTmp = (float)(w0 *w1* pow((u0 - u1), 2)); //类间方差公式 g = w1 * w2 * (u1 - u2) ^ 2
		if (deltaTmp > deltaMax)
		{
			deltaMax = deltaTmp;
			th = i;
		}
	}
	//cout << th << endl;
	return th;
}

void judge_product(Mat pic1, Mat pic2)//对二值化后的两幅图进行分析  pic1为黑色芯片二值图，pic2为有黄色薄膜二值图
{
	vector<vector<Point>> contours1;
	vector<vector<Point>> contours2;
	vector<Vec4i> hireachy1;
	vector<Vec4i> hireachy2;
	Rect rect1;
	Rect rect2;
	int area;
	int i = 0;
	findContours(pic1, contours1, hireachy1, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	findContours(pic2, contours2, hireachy2, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	int area1_max = 0;
	int area2_max = 0;
	vector<Point> contours1_max = contours1[0];
	vector<Point> contours2_max = contours2[0];
	for (; i < contours1.size(); i++)
	{
		Rect rect = boundingRect(contours1[i]);
		if (rect.height>pic1.rows - 5) continue;
		area = contourArea(contours1[i]);
		if (area > area1_max)
		{
			area1_max = area;
			contours1_max = contours1[i];
		}
	}
	i = 0;
	for (; i < contours2.size(); i++)
	{
		Rect rect = boundingRect(contours2[i]);
		if (rect.height>pic2.rows - 5) continue;
		area = contourArea(contours2[i]);
		if (area > area2_max)
		{
			area2_max = area;
			contours2_max = contours2[i];
		}
	}
	rect1 = boundingRect(Mat(contours1_max));
	rect2 = boundingRect(Mat(contours2_max));
	Mat rect_pic1 = Mat::zeros(pic1.size(), CV_8UC3);
	Mat rect_pic2 = Mat::zeros(pic2.size(), CV_8UC3);
	rectangle(rect_pic1, rect1.tl(), rect1.br(), Scalar(0, 0, 255), 2, 8, 0);
	rectangle(rect_pic2, rect2.tl(), rect2.br(), Scalar(0, 0, 255), 2, 8, 0);
	//imshow("rect1_pic",rect_pic1);
	//imshow("rect2_pic", rect_pic2);
	int rect1_y = rect1.y + rect1.height; //矩形1的左下坐标
	int rect2_y = rect2.y + rect2.height;
	if (abs(rect1_y - rect2_y) > (rect1.height*0.03))
		cout << "bad product:Lower edge overflowed!" << endl;
	else if (rect2.width*0.95 > rect1.width)
		cout << "bad product:left and right sides overflowed" << endl;
	else
		cout << "good product" << endl;

}