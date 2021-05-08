#include <opencv2\opencv.hpp>
#include <iostream>
#include <math.h>
#include <time.h>

using namespace cv;
using namespace std;

void  FMF(Mat& image_input, Mat& image_output, int diameter);
void MF(Mat& image_input, Mat& image_output, int diameter);
int main(int argc, char** argv)
{
	double start, end;
	Mat in = imread("E:\\vs_work\\test.jpg");
	Mat src;

	cvtColor(in, src, COLOR_BGR2GRAY);
	printf("the picture width = %d \n", src.cols);
	printf("the picture height = %d \n", src.rows);
	printf("the picture channals = %d \n", src.channels());

	Mat dst = Mat::zeros(src.size(),CV_8UC1);
	start = clock();
	FMF(src, dst, 3);
	end = clock();
	start = (end - start) / CLOCKS_PER_SEC;
	printf("the fast filter consume time is %f s\n", start);
	namedWindow("fast filter", WINDOW_AUTOSIZE);
	imshow("fast filter", dst);
	dst = Mat::zeros(src.size(), CV_8UC1);

	start = clock();
	MF(src, dst, 3);
	end = clock();
	start = (end - start) / CLOCKS_PER_SEC;
	printf("the median filter consume time is %f s\n", start);
	namedWindow("median filter", WINDOW_AUTOSIZE);
	imshow("median filter", dst);
	dst = Mat::zeros(src.size(), CV_8UC1);

	start = clock();
	medianBlur(src, dst, 3);
	end = clock();
	start = (end - start) / CLOCKS_PER_SEC;
	printf("the OpencvAPI filter consume time is %f s\n", start);
	namedWindow("OpencvAPI median filter", WINDOW_AUTOSIZE);
	imshow("OpencvAPI median filter", dst);

	waitKey(0);
	return 0;
}

void  FMF(Mat& image_input, Mat& image_output,int diameter)  //������ȡ��Ե����������
{
	int  radius = (diameter-1)/2;//����ȡ��
	int i, j, p, q, s, t;
	uchar val;
	int sum = 0;
	int Hist[256] = { 0 };
	int Threshold = (diameter*diameter + 1) / 2;
	int median;

	for (i = 0; i < image_input.rows; i++)
	{
		for (j = 0; j <radius; j++)
		{
			image_output.at<uchar>(i,j) = image_input.at<uchar>(i, j);
			image_output.at<uchar>(i,(image_input.cols-j-1)) = image_input.at<uchar>(i, (image_input.cols - j - 1));
		}
	}
	for (j = 0; j < image_input.cols; j++)
	{
		for (i = 0; i < radius; i++)
		{
			image_output.at<uchar>(i,j) = image_input.at<uchar>(i, j);
			image_output.at<uchar>((image_input.rows-i-1),j) = image_input.at<uchar>((image_input.rows - i - 1), j);
		}
	}
	//�������˱�Ե����
   
	for (i =  radius; i < (image_input.rows - radius); i++)
	{
		for (j =  radius; j < (image_input.cols - radius); j++)
		{
			if (j == radius)  //��ʾÿ�е�һ����Ҫ�˲���Ԫ�أ���Ҫ��������ֱ��ͼ����
			{
				memset(Hist, 0, sizeof(Hist));  //��ʼ��ֱ��ͼ��
				for ( p = i - radius; p <=i + radius; p++)
				{
					for ( q = j - radius; q <= j + radius; q++)
					{
					    val = image_input.at<uchar>(p, q);
						Hist[val]++;
					}
				}
			}//�жϽ�����ʾֱ��ͼ������ϣ�����Ҫ�����Ǹ���ֱ��ͼ
			//���j != radius��ʾֱ��ͼ���ڸ�����
			if (j != radius)
			{
				for (s = i - radius; s <= i + radius; s++)
				{
					val = image_input.at<uchar>(s, j - radius - 1);
					Hist[val]--;
					val = image_input.at<uchar>(s, j + radius);
					Hist[val]++;
				}//ֱ��ͼ�������
			}
			//Ѱ����ֵ
			for ( t = 0; t < 256; t++)
			{
				sum = sum + Hist[t];
				if (sum >= Threshold)
				{
					sum = 0;
					median = t;
					break;
				}
			}
         //��ֵ�ҵ�����Ҫ�滻
			image_output.at<uchar>(i, j) = (uchar)median;
		}
	}
}

void MF(Mat& image_input, Mat& image_output, int diameter)
{
	int  radius = (diameter - 1) / 2;//����ȡ��
	int i, j, p, q, t,sum = 0,median;
	uchar val;
	int Hist[256] = { 0 };
	int Threshold = (diameter*diameter + 1) / 2;

	for (i = 0; i < image_input.rows; i++)
	{
		for (j = 0; j <radius; j++)
		{
			image_output.at<uchar>(i, j) = image_input.at<uchar>(i, j);
			image_output.at<uchar>(i, (image_input.cols - j - 1)) = image_input.at<uchar>(i, (image_input.cols - j - 1));
		}
	}
	for (j = 0; j < image_input.cols; j++)
	{
		for (i = 0; i < radius; i++)
		{
			image_output.at<uchar>(i, j) = image_input.at<uchar>(i, j);
			image_output.at<uchar>((image_input.rows - i - 1), j) = image_input.at<uchar>((image_input.rows - i - 1), j);
		}
	}
	//�������˱�Ե����
	for (i = radius; i < (image_input.rows - radius); i++)
	{
		for (j = radius; j < (image_input.cols - radius); j++)
		{
			memset(Hist, 0, sizeof(Hist));  //��ʼ��ֱ��ͼ��
			for (p = i - radius; p <= i + radius; p++)
			{
				for (q = j - radius; q <= j + radius; q++)
				{
					val = image_input.at<uchar>(p, q);
					Hist[val]++;
				}
			}
			for (t = 0; t < 256; t++)
			{
				sum = sum + Hist[t];
				if (sum >= Threshold)
				{
					sum = 0;
					median = t;
					break;
				}
			}
			//��ֵ�ҵ�����Ҫ�滻
			image_output.at<uchar>(i, j) = (uchar)median;
		}
	}
}