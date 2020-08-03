#include <stdio.h>
#include <math.h>
#include "calculate.h"

int lag = 0; //偏移
int i;


//创建正弦函数表
void CREATE_SIN_TABLE(void)
{
	int i = 0;
	for (i = 0; i <= Npart4; i++)
	{
		SIN_TABLE[i] = sin(PI * i / Npart2); //SIN_TABLE[i] = sin(PI2*i/N);
	}
}

ElemType Sin_find(ElemType x)
{
	int i = (int)(N * x);
	i = i >> 1;
	if (i > Npart4)		//注意：i已经转化为0~N之间的整数了！
	{					//不会超过N/2
		i = Npart2 - i; //i = i - 2*(i-Npart4);
	}
	return SIN_TABLE[i];
}
ElemType Cos_find(ElemType x)
{
	int i = (int)(N * x);
	i = i >> 1;
	if (i < Npart4) //注意：i已经转化为0~N之间的整数了！
	{				//不会超过N/2
		//i = Npart4 - i;
		return SIN_TABLE[Npart4 - i];
	}
	else //i>Npart4 && i<N/2
	{
		//i = i - Npart4;
		return -SIN_TABLE[i - Npart4];
	}
}

//变址
void ChangeSeat(complex *DataInput)
{
	int nextValue, nextM, i, k, j = 0;
	complex temp;

	nextValue = N / 2; //变址运算，即把自然顺序变成倒位序，采用雷德算法
	nextM = N - 1;
	for (i = 0; i < nextM; i++)
	{
		if (i < j) //如果i<j,即进行变址
		{
			temp = DataInput[j];
			DataInput[j] = DataInput[i];
			DataInput[i] = temp;
		}
		k = nextValue; //求j的下一个倒位序
		while (k <= j) //如果k<=j,表示j的最高位为1
		{
			j = j - k; //把最高位变成0
			k = k / 2; //k/2，比较次高位，依次类推，逐个比较，直到某个位为0
		}
		j = j + k; //把0改为1
	}
}
//FFT运算函数
void FFT(complex *data)
{
	int L = 0, B = 0, J = 0, K = 0;
	int step = 0, KB = 0;
	//ElemType P=0;
	ElemType angle;
	complex W, Temp_XX;

	ChangeSeat(data); //变址
	for (L = 1; L <= M; L++)
	{
		step = 1 << L; //2^L
		B = step >> 1; //B=2^(L-1)
		for (J = 0; J < B; J++)
		{
			angle = (double)J / B;	 //这里还可以优化
			W.imag = -Sin_find(angle); //用C++该函数课声明为inline
			W.real = Cos_find(angle);  //用C++该函数课声明为inline
			for (K = J; K < N; K = K + step)
			{
				KB = K + B;
				Temp_XX.real = data[KB].real * W.real - data[KB].imag * W.imag;
				Temp_XX.imag = W.imag * data[KB].real + data[KB].imag * W.real;

				data[KB].real = data[K].real - Temp_XX.real;
				data[KB].imag = data[K].imag - Temp_XX.imag;

				data[K].real = data[K].real + Temp_XX.real;
				data[K].imag = data[K].imag + Temp_XX.imag;
			}
		}
	}
}
//IFFT运算函数
void IFFT(complex *data)
{
	int L = 0, B = 0, J = 0, K = 0;
	int step = 0, KB = 0;
	//ElemType P=0;
	ElemType angle;
	complex W, Temp_XX;

	ChangeSeat(data); //变址
	//CREATE_SIN_TABLE();
	for (L = 1; L <= M; L++)
	{
		step = 1 << L; //2^L
		B = step >> 1; //B=2^(L-1)
		for (J = 0; J < B; J++)
		{
			//P = (1<<(M-L))*J;//P=2^(M-L) *J
			angle = (double)J / B; //这里还可以优化

			W.imag = Sin_find(angle); //用C++该函数课声明为inline
			W.real = Cos_find(angle); //用C++该函数课声明为inline
			//W.real =  cos(angle*PI);
			//W.imag = -sin(angle*PI);
			for (K = J; K < N; K = K + step)
			{
				KB = K + B;
				//Temp_XX = XX_complex(data[KB],W);
				//用下面两行直接计算复数乘法，省去函数调用开销
				Temp_XX.real = data[KB].real * W.real - data[KB].imag * W.imag;
				Temp_XX.imag = W.imag * data[KB].real + data[KB].imag * W.real;

				data[KB].real = data[K].real - Temp_XX.real;
				data[KB].imag = data[K].imag - Temp_XX.imag;

				data[K].real = data[K].real + Temp_XX.real;
				data[K].imag = data[K].imag + Temp_XX.imag;
			}
		}
	}
}

void Init_FFT()
{

	//SIN_TABLE_of_N_FFT[Npart4+1];
	CREATE_SIN_TABLE(); //创建正弦函数表
}

//-----------------------------相关函数功能部分---------------//
//求信号的共轭
void conj(complex s[], int len) //长度
{
	for (i = 0; i < len; i++)
	{
		s[i].imag = 0.0f - s[i].imag;
	}
}
//返回偏移量
int getLag()
{
	return lag;
}

//求两个复数的乘法，结果返回到第一个输入
//---------------------长度问题-------------------//
void mul(complex s1[], complex s2[], int n)
{
	float temp11 = 0, temp12 = 0;
	float temp21 = 0, temp22 = 0;
	for (i = 0; i < n; i++)
	{
		temp11 = s1[i].real;
		temp12 = s1[i].imag;
		temp21 = s2[i].real;
		temp22 = s2[i].imag;
		s1[i].real = temp11 * temp21 - temp12 * temp22;
		s1[i].imag = temp11 * temp22 + temp21 * temp12;
	}
}

//相关函数
void correlation(float sig1[], int n1, float sig2[], int n2) //n1 和 n2 是数组长度
{
	int maxLen = n1 > n2 ? n1 : n2;
	maxLen = (int)(log(maxLen) / log(2.0)) + 1; //求出FFT的幂次
	maxLen = (int)(pow(2, maxLen));

	for (i = 0; i < n1; i++)
	{
		s1[i].real = sig1[i]; //16
		s1[i].imag = 0;
		//System.out.println(s1[i].real);
	}

	for (i = 0; i < n2; i++)
	{
		s2[i].real = sig2[i]; //16
		s2[i].imag = 0;
		//System.out.println(s2[i].real);
	}

	//求出信号的FFT
	//float[] rr = new float[maxLen];
	float rr[maxLen];
	//FFT fft = new FFT(maxLen);
	Init_FFT();

	FFT(s1);
	FFT(s2);
	//---------------------------长度问题--------------------//
	conj(s2, N);	//共轭
	mul(s1, s2, N); //相乘
	IFFT(s1);		//逆变换

	float max = s1[maxLen - 1].real;

	//--------------------注意错误-----------------//
	//for(int i=0;i>maxLen;i++) //峰值检测
	for (i = 0; i < maxLen; i++) //峰值检测
	{
		if (s1[i].real > max)
		{
			lag = i;
			max = s1[i].real;
		}
	}
}

//-------------------------------------计算位置功能部分-----------------------//

/**
*计算时间差
*/
void getTimeDiff(float str1[], float str2[], float str3[], float str4[])
{
	//时间差t_12
	correlation(str1, N, str2, N);
	timeDiff[0] = (getLag() / 3) * 1.0 / 16000;
	//时间差t_13
	correlation(str1, N, str3, N);
	timeDiff[1] = (getLag() / 3) * 1.0 / 16000;
	//时间差t_14
	correlation(str1, N, str4, N);
	timeDiff[2] = (getLag() / 3) * 1.0 / 16000;
}

/** 算出目标空间位置:距离三维坐标原点长度
* c:声音的速度
* timeDiff：基准麦克风相对其他麦克风时延差 ,此处默认长度n = 3
* 说明：此处只用于十字形麦克风阵列计算
*/
double getDistence()
{
	double m = timeDiff[0] * timeDiff[0] + timeDiff[2] * timeDiff[2] - timeDiff[1] * timeDiff[1]; //timeDiff[i]表示基准麦克风于第i+1个麦克风时延。
	double n = timeDiff[1] - timeDiff[0] - timeDiff[2];
	double R = c * m / (2 * n);
//	if(fabs(m)<0.000001)
//		return 0;
//	else
         	return R;
}
/** 算出目标空间位置 :于x轴正方向方位角
* r：目标到不同麦克风距离 ,此处默认长度n = 4
* 说明：此处只用于十字形麦克风阵列计算
*/
double getAngle_X()
{
	/*
	double m = (r[1]*r[1] - r[3]*r[3]) / (r[2]*r[2] - r[0]*r[0] );
	double x_angle = atan(m);
	return x_angle;
	*/
	/*
	double t_24 = timeDiff[2]-timeDiff[0];//time[3]-time[1]
	double t_31 = timeDiff[1];//time[2]-time[0]
	*/
	if (timeDiff[1] > 0 && (timeDiff[2] - timeDiff[0]) > 0)
		return (atan((timeDiff[2] - timeDiff[0]) / timeDiff[1]))*57.3;
	else if (timeDiff[1] > 0 && (timeDiff[2] - timeDiff[0]) < 0)
		return (atan((timeDiff[2] - timeDiff[0]) / timeDiff[1]) + 2 * PI)*57.3;
	else if (timeDiff[1] < 0 && (timeDiff[2] - timeDiff[0]) > 0)
		return (atan((timeDiff[2] - timeDiff[0]) / timeDiff[1]) + PI)*57.3;
	else if (timeDiff[1] < 0 && (timeDiff[2] - timeDiff[0]) < 0)
		return (atan((timeDiff[2] - timeDiff[0]) / timeDiff[1]) + PI)*57.3;

	else if (timeDiff[1] = 0 && (timeDiff[2] - timeDiff[0]) > 0)
		return PI*57.3 / 2;
	else if (timeDiff[1] = 0 && (timeDiff[2] - timeDiff[0]) < 0)
		return 0.75 * PI*57.3;
}

/** 算出目标空间位置 :于z轴正方向方位角
*  a:麦克风到原点的距离，默认麦克风在每个坐标轴上
** timeDiff：基准麦克风相对其他麦克风时延差 ,此处默认长度n = 3
* 说明：此处只用于十字形麦克风阵列计算
*/
double getAngle_Z()
{
	double m = timeDiff[1] * timeDiff[1] - (timeDiff[2] - timeDiff[0]) * (timeDiff[2] - timeDiff[0]);
	printf("m:%f",m);
/*	if(fabs(m)<0.000001)
		return 0;*/
	//double z_angle = ( c* sqrt(m) ) / (2*a);
   //	else
	        return (asin((c * sqrt(fabs(m))) / (2 * a)))*57.3;
}

/*
*传入信号，打印位置
*/
void print_position(float str1[], float str2[], float str3[], float str4[])
{
	//获取四方信号 ，并计算时间差,更新数组timeDiff[]
	getTimeDiff(str1, str2, str3, str4);
	printf("the target location:\n");
	printf("Distance from the reference microphone:%f\n", getDistence());
	printf("Direction angle with the X axis:%f\n", getAngle_X());
	printf("Direction angle with the z axis:%f\n", getAngle_Z());
}
