#include "pre_process.h"

extern vector<Mat> pic_mat_vector;

void copy_chip2_separated_pic(Mat pic, vector<Rect> Rect_vector,int map_temp_width,int map_temp_height) //将一个图片中的多个芯片分离到单独的图像中去，并用vector保存
{
	int rows_begin;
	int rows_end;
	int cols_begin;
	int cols_end;
	for (size_t i = 0; i < Rect_vector.size(); i++)
	{
		rows_begin = Rect_vector.at(i).y - 40;
		if (rows_begin < 0)
			rows_begin = 0;
		rows_end = Rect_vector.at(i).y + Rect_vector.at(i).height + 40;
		if (rows_end > pic.rows - 1)
			rows_end = pic.rows - 1;
		cols_begin = Rect_vector.at(i).x - 40;
		if (cols_begin < 0)
			cols_begin = 0;
		cols_end = Rect_vector.at(i).x + Rect_vector.at(i).width + 40;
		if (cols_end > pic.cols - 1)
			cols_end = pic.cols - 1;
		Mat mat_temp(Size(map_temp_width,map_temp_height), CV_8UC3, Scalar(255, 255, 255));
		for (int sou_i = (rows_end + rows_begin) / 2,dst_i = map_temp_height / 2; sou_i >= rows_begin; sou_i--,dst_i--)
		{
			for (int sou_j = (cols_end + cols_begin) / 2,dst_j = map_temp_width / 2; sou_j >= cols_begin; sou_j--,dst_j--)
			{
				mat_temp.at<Vec3b>(dst_i, dst_j) = pic.at<Vec3b>(sou_i, sou_j);
			}
			for (int sou_k = (cols_end + cols_begin) / 2, dst_k = map_temp_width / 2; sou_k <= cols_end; sou_k++, dst_k++)
			{
				mat_temp.at<Vec3b>(dst_i, dst_k) = pic.at<Vec3b>(sou_i, sou_k);
			}
		}
		for (int sou_i = (rows_end + rows_begin) / 2, dst_i = map_temp_height / 2; sou_i <= rows_end; sou_i++, dst_i++)
		{
			for (int sou_j = (cols_end + cols_begin) / 2, dst_j = map_temp_width / 2; sou_j <= cols_end ; sou_j++, dst_j++)
			{
				mat_temp.at<Vec3b>(dst_i, dst_j) = pic.at<Vec3b>(sou_i, sou_j);
			}
			for (int sou_k = (cols_end + cols_begin) / 2, dst_k = map_temp_width / 2; sou_k >= cols_begin; sou_k--, dst_k--)
			{
				mat_temp.at<Vec3b>(dst_i, dst_k) = pic.at<Vec3b>(sou_i, sou_k);
			}
		}
		//imshow("mat", mat_temp);
		pic_mat_vector.push_back(mat_temp);
		//free(&mat_temp);
	}
	//cout << pic_mat_vector.size() << endl;
}

void spilt_product(Mat pic)  //分离产品
{
	Mat grayImage, binPic;
	vector<int> cols;
	vector<vector<Point>> contours;
	vector<Vec4i> hireachy;
	vector<Rect> Rect_vector;
	cvtColor(pic, grayImage, CV_BGR2GRAY);
	threshold(grayImage, binPic, 150, 255, 1);
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(30, 30));
	Mat openMat;
	Rect rect_temp;
	morphologyEx(binPic, openMat, MORPH_OPEN, element);
	//imshow("openMat", openMat);
	findContours(openMat, contours, hireachy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
	cout << "contours.size:" << contours.size()<<endl;
	for (size_t i = 0; i < contours.size(); i++)
	{
		rect_temp = boundingRect(Mat(contours[i]));
		if ((rect_temp.height >(binPic.rows / 3)) && (rect_temp.width < (binPic.cols / 2)))
			Rect_vector.push_back(rect_temp);
	}
	copy_chip2_separated_pic(pic, Rect_vector,600,400);
	cout << "size:" << pic_mat_vector.size() << endl;

}

Mat rotate_pic(Mat pic, float degree) //  旋转芯片
{
	Mat out_img;

	// 旋转角度
	double angle = 90 - degree;

	// 计算旋转后输出图形的尺寸
	int rotated_width = ceil(pic.rows * fabs(sin(angle * CV_PI / 180)) + pic.cols * fabs(cos(angle * CV_PI / 180)));
	int rotated_height = ceil(pic.cols * fabs(sin(angle * CV_PI / 180)) + pic.rows * fabs(cos(angle * CV_PI / 180)));

	// 计算仿射变换矩阵
	Point2f center(pic.cols / 2, pic.rows / 2);
	Mat rotate_matrix = getRotationMatrix2D(center, angle, 1.0);

	// 防止切边，对平移矩阵B进行修改
	rotate_matrix.at<double>(0, 2) += (rotated_width - pic.cols) / 2;
	rotate_matrix.at<double>(1, 2) += (rotated_height - pic.rows) / 2;

	// 应用仿射变换
	warpAffine(pic, out_img, rotate_matrix, Size(rotated_width, rotated_height), INTER_LINEAR, 0, Scalar(255, 255, 255));
	return out_img;
}

Mat rotate_judge(Mat pic) //判断当前芯片是否需要旋转
{
	Mat grayImage, binPic;
	vector<int> cols;
	vector<vector<Point>> contours;
	vector<Vec4i> hireachy;
	RotatedRect rect;
	cvtColor(pic, grayImage, CV_BGR2GRAY);
	threshold(grayImage, binPic, 150, 255, 1);
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(25, 25));
	Mat openMat;
	morphologyEx(binPic, openMat, MORPH_OPEN, element);
	//imshow("    ", openMat);
	findContours(openMat, contours, hireachy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
	//cout << "size:" << contours.size() << endl;
	for (size_t i = 0; i < contours.size(); i++)
	{
		RotatedRect rect_temp = minAreaRect(Mat(contours[i]));
		if ((rect_temp.size.height >(binPic.rows / 2)) && (rect_temp.size.height < (binPic.rows - 5)))
			rect = rect_temp;
	}
	cout << rect.angle << endl;
	float angle = abs(rect.angle);
	if (((angle > 8) && (angle < 82)) || ((angle > 98) && (angle < 172))) //角度偏差超过8°则需要调整
		pic = rotate_pic(pic, angle);
	//imshow("pic",pic);
	return pic;
}

Mat extract_chip(Mat Soupic)
{
	Mat result_pic;
	Mat temppic;
	vector<vector<Point>> contours;
	vector<Vec4i> hireachy;

	Mat grayImage, binPic;
	cvtColor(Soupic, grayImage, CV_BGR2GRAY);
	threshold(grayImage, binPic, 150, 255, 1);
	imshow("binPic", binPic);/*
	int edgeThresh = 80;
	Canny(grayImage, binPic, edgeThresh, edgeThresh * 2, 3, false);
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(1, 1), Point(-1, -1));
	Mat closeMat;
	morphologyEx(binPic, closeMat, MORPH_CLOSE, element);
	findContours(closeMat, contours, hireachy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
	imshow("closemat", closeMat);
	*/
	findContours(binPic, contours, hireachy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
	Mat hole(binPic.size(), CV_8U, Scalar(0));
	drawContours(hole, contours, -1, Scalar(255), CV_FILLED);
	//imshow("hole",hole);
	for (int i = 0; i < Soupic.rows; i++) {
		for (int j = 0; j < Soupic.cols; j++) {
			if (hole.ptr<uchar>(i)[j] == 0)
			{
				Soupic.ptr<uchar>(i)[Soupic.channels()*j] = 255;
				Soupic.ptr<uchar>(i)[Soupic.channels()*j + 1] = 255;
				Soupic.ptr<uchar>(i)[Soupic.channels()*j + 2] = 255;
			}
		}
	}
	//imshow("extract_chip",Soupic);
	return Soupic;
}

int detect_rows(Mat pic, int mode) //0代表找上边界，1代表找下边界
{
	int i;
	int j;
	size_t count = 0;
	int temp;
	if (mode == 0)//正序查找
	{
		for (i = 0; i < pic.rows; i = i + 5)//5行扫描一次
		{
			for (j = 0; j < pic.cols; j = j + 10)//10列扫描一次
			{
				if ((pic.at<Vec3b>(i, j) != Vec3b(255, 255, 255)) && (pic.at<Vec3b>(i, j) != Vec3b(0, 0, 0)))
					count++;
			}
			if (count >= 4)
				break;
			count = 0;

		}
		temp = i;
		i = i - 4;
		j = 0;
		count = 0;
		while (i < temp)
		{
			for (j = 0; j < pic.cols; j++)
			{
				if ((pic.at<Vec3b>(i, j) != Vec3b(255, 255, 255)) && (pic.at<Vec3b>(i, j) != Vec3b(0, 0, 0)))
					count++;
			}
			if (count >= 50)
				break;
			i++;
		}
	}
	if (mode == 1)//反序查找
	{
		for (i = pic.rows - 1; i > 0; i = i - 5)//5行扫描一次
		{
			for (j = 0; j < pic.cols; j = j + 10)//10列扫描一次
			{
				if ((pic.at<Vec3b>(i, j) != Vec3b(255, 255, 255)) && (pic.at<Vec3b>(i, j) != Vec3b(0, 0, 0)))
					count++;
			}
			if (count >= 4)
				break;
			count = 0;

		}
		temp = i;
		i = i + 4;
		j = 0;
		count = 0;
		while (i > temp)
		{
			for (j = 0; j < pic.cols; j++)
			{
				if ((pic.at<Vec3b>(i, j) != Vec3b(255, 255, 255)) && (pic.at<Vec3b>(i, j) != Vec3b(0, 0, 0)))
					count++;
			}
			if (count >= 50)
				break;
			i--;
		}
	}
	return i;
}

int detect_cols(Mat pic, int mode) //0代表找左边界，1代表找右边界
{
	int i;
	int rows_seq_temp = detect_rows(pic, 0);
	int rows_rev_temp = detect_rows(pic, 1);
	int j;
	size_t count = 0;
	int temp;
	if (mode == 0)
	{
		for (j = 0; j < pic.cols - 1; j = j + 5)//每5列扫描一次
		{
			for (i = rows_seq_temp; i < rows_rev_temp; i = i + 10)
			{
				if ((pic.at<Vec3b>(i, j) != Vec3b(255, 255, 255)) && (pic.at<Vec3b>(i, j) != Vec3b(0, 0, 0)))
					count++;
			}
			if (count >= 6)
				break;
		}
		temp = j;
		j = j - 4;
		i = rows_seq_temp;
		count = 0;
		while (j < temp)
		{
			for (; i < rows_rev_temp; i++)
			{
				if ((pic.at<Vec3b>(i, j) != Vec3b(255, 255, 255)) && (pic.at<Vec3b>(i, j) != Vec3b(0, 0, 0)))
					count++;
			}
			if (count >= 200)
				break;
			j++;
		}
	}
	if (mode == 1)
	{
		for (j = pic.cols - 1; j > 0; j = j - 5)//每5列扫描一次
		{
			for (i = rows_seq_temp; i < rows_rev_temp; i = i + 10)
			{
				if ((pic.at<Vec3b>(i, j) != Vec3b(255, 255, 255)) && (pic.at<Vec3b>(i, j) != Vec3b(0, 0, 0)))
					count++;
			}
			if (count >= 6)
				break;
		}
		temp = j;
		j = j + 4;
		i = rows_seq_temp;
		count = 0;
		while (j > temp)
		{
			for (; i < rows_rev_temp; i++)
			{
				if ((pic.at<Vec3b>(i, j) != Vec3b(255, 255, 255)) && (pic.at<Vec3b>(i, j) != Vec3b(0, 0, 0)))
					count++;
			}
			if (count >= 200)
				break;
			j--;
		}
	}
	return j;
}

Mat get_product(Mat pic)  //得到产品的剪切图
{
	int product_upedge = detect_rows(pic,0);
	int product_downedge = detect_rows(pic, 1);
	int product_leftedge = detect_cols(pic, 0);
	int product_rightedge = detect_cols(pic, 1);
	Mat product_pic_final = Mat(Size(product_rightedge - product_leftedge + 20, product_downedge - product_upedge+20),CV_8UC3,Scalar(255,255,255));
	for (int i = product_upedge,i_temp = 10; i <= product_downedge; i++,i_temp++)
	{
		for (int j = product_leftedge,j_temp = 10; j <= product_rightedge; j++,j_temp++)
		{
			product_pic_final.at<Vec3b>(i_temp, j_temp) = pic.at<Vec3b>(i, j);
		}
	}
	return product_pic_final;
}
