#include "pre_process.h"
#include "product_judge.h"

vector<Mat> pic_mat_vector;

void final_judge_product(Mat pic)
{
	Mat product_pic = extract_chip(pic);
	//imshow("product_pic", product_pic);
	Mat product_final = get_product(product_pic);
	imshow("product_final", product_final);
	Mat graypic;
	cvtColor(product_final, graypic, CV_BGR2GRAY);
	//imshow("graypic", graypic);
	int th = myOtsu(graypic);
	Mat binImg1, binImg2;
	threshold(graypic, binImg1, th - 50, 255, THRESH_BINARY);
	threshold(graypic, binImg2, th, 255, THRESH_BINARY);
	imshow("binImg1", binImg1);
	imshow("binImg2", binImg2);
	judge_product(binImg1, binImg2);
}

int main()
{
	Mat SouPic = imread("C:\\Users\\Administrator\\Desktop\\product_pic\\more_product\\more_product1\\pic3.jpg");
	resize(SouPic, SouPic, Size(SouPic.cols / 4, SouPic.rows / 4), 0, 0);
	imshow("Soupic",SouPic);
	spilt_product(SouPic);
	Mat product_jud;
	cout << "mat vector size:" << pic_mat_vector.size() << endl;
	for (size_t i = 0; i < pic_mat_vector.size(); i++)
	{
		imshow("  ",pic_mat_vector.at(i));
	}
	for (size_t i = 0; i < pic_mat_vector.size(); i++)
	{
		product_jud = rotate_judge(pic_mat_vector.at(i));
		cout << "product  " << pic_mat_vector.size() - i << ":" << endl;
		final_judge_product(product_jud);
	}
	waitKey(0);

}
