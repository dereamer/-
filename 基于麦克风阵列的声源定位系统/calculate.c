#include <stdio.h>
#include <math.h>
#include "calculate.h"

int lag = 0; //ƫ��
int i;


//�������Һ�����
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
	if (i > Npart4)		//ע�⣺i�Ѿ�ת��Ϊ0~N֮��������ˣ�
	{					//���ᳬ��N/2
		i = Npart2 - i; //i = i - 2*(i-Npart4);
	}
	return SIN_TABLE[i];
}
ElemType Cos_find(ElemType x)
{
	int i = (int)(N * x);
	i = i >> 1;
	if (i < Npart4) //ע�⣺i�Ѿ�ת��Ϊ0~N֮��������ˣ�
	{				//���ᳬ��N/2
		//i = Npart4 - i;
		return SIN_TABLE[Npart4 - i];
	}
	else //i>Npart4 && i<N/2
	{
		//i = i - Npart4;
		return -SIN_TABLE[i - Npart4];
	}
}

//��ַ
void ChangeSeat(complex *DataInput)
{
	int nextValue, nextM, i, k, j = 0;
	complex temp;

	nextValue = N / 2; //��ַ���㣬������Ȼ˳���ɵ�λ�򣬲����׵��㷨
	nextM = N - 1;
	for (i = 0; i < nextM; i++)
	{
		if (i < j) //���i<j,�����б�ַ
		{
			temp = DataInput[j];
			DataInput[j] = DataInput[i];
			DataInput[i] = temp;
		}
		k = nextValue; //��j����һ����λ��
		while (k <= j) //���k<=j,��ʾj�����λΪ1
		{
			j = j - k; //�����λ���0
			k = k / 2; //k/2���Ƚϴθ�λ���������ƣ�����Ƚϣ�ֱ��ĳ��λΪ0
		}
		j = j + k; //��0��Ϊ1
	}
}
//FFT���㺯��
void FFT(complex *data)
{
	int L = 0, B = 0, J = 0, K = 0;
	int step = 0, KB = 0;
	//ElemType P=0;
	ElemType angle;
	complex W, Temp_XX;

	ChangeSeat(data); //��ַ
	for (L = 1; L <= M; L++)
	{
		step = 1 << L; //2^L
		B = step >> 1; //B=2^(L-1)
		for (J = 0; J < B; J++)
		{
			angle = (double)J / B;	 //���ﻹ�����Ż�
			W.imag = -Sin_find(angle); //��C++�ú���������Ϊinline
			W.real = Cos_find(angle);  //��C++�ú���������Ϊinline
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
//IFFT���㺯��
void IFFT(complex *data)
{
	int L = 0, B = 0, J = 0, K = 0;
	int step = 0, KB = 0;
	//ElemType P=0;
	ElemType angle;
	complex W, Temp_XX;

	ChangeSeat(data); //��ַ
	//CREATE_SIN_TABLE();
	for (L = 1; L <= M; L++)
	{
		step = 1 << L; //2^L
		B = step >> 1; //B=2^(L-1)
		for (J = 0; J < B; J++)
		{
			//P = (1<<(M-L))*J;//P=2^(M-L) *J
			angle = (double)J / B; //���ﻹ�����Ż�

			W.imag = Sin_find(angle); //��C++�ú���������Ϊinline
			W.real = Cos_find(angle); //��C++�ú���������Ϊinline
			//W.real =  cos(angle*PI);
			//W.imag = -sin(angle*PI);
			for (K = J; K < N; K = K + step)
			{
				KB = K + B;
				//Temp_XX = XX_complex(data[KB],W);
				//����������ֱ�Ӽ��㸴���˷���ʡȥ�������ÿ���
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
	CREATE_SIN_TABLE(); //�������Һ�����
}

//-----------------------------��غ������ܲ���---------------//
//���źŵĹ���
void conj(complex s[], int len) //����
{
	for (i = 0; i < len; i++)
	{
		s[i].imag = 0.0f - s[i].imag;
	}
}
//����ƫ����
int getLag()
{
	return lag;
}

//�����������ĳ˷���������ص���һ������
//---------------------��������-------------------//
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

//��غ���
void correlation(float sig1[], int n1, float sig2[], int n2) //n1 �� n2 �����鳤��
{
	int maxLen = n1 > n2 ? n1 : n2;
	maxLen = (int)(log(maxLen) / log(2.0)) + 1; //���FFT���ݴ�
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

	//����źŵ�FFT
	//float[] rr = new float[maxLen];
	float rr[maxLen];
	//FFT fft = new FFT(maxLen);
	Init_FFT();

	FFT(s1);
	FFT(s2);
	//---------------------------��������--------------------//
	conj(s2, N);	//����
	mul(s1, s2, N); //���
	IFFT(s1);		//��任

	float max = s1[maxLen - 1].real;

	//--------------------ע�����-----------------//
	//for(int i=0;i>maxLen;i++) //��ֵ���
	for (i = 0; i < maxLen; i++) //��ֵ���
	{
		if (s1[i].real > max)
		{
			lag = i;
			max = s1[i].real;
		}
	}
}

//-------------------------------------����λ�ù��ܲ���-----------------------//

/**
*����ʱ���
*/
void getTimeDiff(float str1[], float str2[], float str3[], float str4[])
{
	//ʱ���t_12
	correlation(str1, N, str2, N);
	timeDiff[0] = (getLag() / 3) * 1.0 / 16000;
	//ʱ���t_13
	correlation(str1, N, str3, N);
	timeDiff[1] = (getLag() / 3) * 1.0 / 16000;
	//ʱ���t_14
	correlation(str1, N, str4, N);
	timeDiff[2] = (getLag() / 3) * 1.0 / 16000;
}

/** ���Ŀ��ռ�λ��:������ά����ԭ�㳤��
* c:�������ٶ�
* timeDiff����׼��˷����������˷�ʱ�Ӳ� ,�˴�Ĭ�ϳ���n = 3
* ˵�����˴�ֻ����ʮ������˷����м���
*/
double getDistence()
{
	double m = timeDiff[0] * timeDiff[0] + timeDiff[2] * timeDiff[2] - timeDiff[1] * timeDiff[1]; //timeDiff[i]��ʾ��׼��˷��ڵ�i+1����˷�ʱ�ӡ�
	double n = timeDiff[1] - timeDiff[0] - timeDiff[2];
	double R = c * m / (2 * n);
//	if(fabs(m)<0.000001)
//		return 0;
//	else
         	return R;
}
/** ���Ŀ��ռ�λ�� :��x��������λ��
* r��Ŀ�굽��ͬ��˷���� ,�˴�Ĭ�ϳ���n = 4
* ˵�����˴�ֻ����ʮ������˷����м���
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

/** ���Ŀ��ռ�λ�� :��z��������λ��
*  a:��˷絽ԭ��ľ��룬Ĭ����˷���ÿ����������
** timeDiff����׼��˷����������˷�ʱ�Ӳ� ,�˴�Ĭ�ϳ���n = 3
* ˵�����˴�ֻ����ʮ������˷����м���
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
*�����źţ���ӡλ��
*/
void print_position(float str1[], float str2[], float str3[], float str4[])
{
	//��ȡ�ķ��ź� ��������ʱ���,��������timeDiff[]
	getTimeDiff(str1, str2, str3, str4);
	printf("the target location:\n");
	printf("Distance from the reference microphone:%f\n", getDistence());
	printf("Direction angle with the X axis:%f\n", getAngle_X());
	printf("Direction angle with the z axis:%f\n", getAngle_Z());
}
