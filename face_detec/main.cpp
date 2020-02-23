/************************* ***/
/****Author: 28-ChenJingyi****/
/****         18011210452 ****/
/*****************************/

#include<stdio.h>
#include<iostream> 
#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<vector>

using namespace std;
using namespace cv;

bool elliptical_judgement(float Cb_, float Cr_) {
	float theta_r, cx, cy, ecx, ecy, a, b; //theta in radien, 7 veriables
	float x, y; 
	theta_r = 2.53;
	cx = 109.38;
	cy = 152.02;
	ecx = 1.60;
	ecy = 2.41;
	a = 25.39;
	b = 14.03;

	x = cos(theta_r)*(Cb_ - cx) + sin(theta_r)*(Cr_ - cy);
	y = -sin(theta_r)*(Cb_ - cx) + cos(theta_r)*(Cr_ - cy);

	if ((((x - ecx)*(x - ecx)) / (a * a)) + (((y - ecy)*(y - ecy)) / (b * b)) <= 1) return true;
	else return false;
}



vector<float> cul_CbCrs(Mat img_YCbCr, int rw, int cl) {
	vector<float> Cx;
	float Y, Cb, Cr, Cb_ave, Cr_ave, Wcb_, Wcr_, Cb_, Cr_;
	float Wcb = 46.97, Wcr = 38.76;
	float Kl = 125, 
		  Kh = 188, 
		  WLcb = 23, 
		  WLcr = 20, 
		  WHcb = 14, 
		  WHcr = 10, 
		  Ymin = 16, 
		  Ymax = 235, 
		  CbKh_ave =108, 
		  CrKh_ave = 154;	

	Y = float(img_YCbCr.at<Vec3b>(rw, cl)[0]);
	Cb = float(img_YCbCr.at<Vec3b>(rw, cl)[1]);
	Cr = float(img_YCbCr.at<Vec3b>(rw, cl)[2]);

	if (Y < Kl) {
		Cb_ave = 108 + ((Kl - Y)*(118 - 108)) / (Kl - Ymin);
		Wcb_ = WLcb + ((Y - Ymin)*(Wcb - WLcb) / (Kl - Ymin));
		Cb_ = (Cb - Cb_ave)*(Wcb / Wcb_) + CbKh_ave;

		Cr_ave = 154 - ((Kl - Y)*(154 - 144)) / (Kl - Ymin);
		Wcr_ = WLcr + ((Y - Ymin)*(Wcr - WLcr) / (Kl - Ymin));
		Cr_ = (Cr - Cr_ave)*(Wcr / Wcr_) + CrKh_ave;
	}
	else if (Y > Kh) {
			Cb_ave = 108 + ((Y - Kh)*(118 - 108)) / (Ymax - Kh);
			Wcb_ = WHcb + ((Ymax - Y)*(Wcb - WHcb) / (Ymax - Kh));
			Cb_ = (Cb - Cb_ave)*(Wcb / Wcb_) + CbKh_ave;

			Cr_ave = 154 + ((Y - Kh)*(154 - 132)) / (Ymax - Kh);
			Wcr_ = WHcr + ((Ymax - Y)*(Wcr - WHcr) / (Ymax - Kh));
			Cr_ = (Cr - Cr_ave)*(Wcr / Wcr_) + CrKh_ave;
	}
	else {
		Cb_ = Cb;
		Cr_ = Cr;
	}
	Cx.push_back(Cb_);
	Cx.push_back(Cr_);
	return Cx;
}

int main() {
	
	float Cb, Cr;
	int row, col,row_f,col_f;
	Mat img_test, img_test_cvt, frame_cvt;

	
	img_test = imread("./hj.jpg", IMREAD_COLOR);
	cvtColor(img_test, img_test_cvt, COLOR_RGB2YCrCb);
	

	row = img_test_cvt.rows;
	col = img_test_cvt.cols;
	//channel = img_test_cvt.channels();

	Mat M(row, col, CV_8UC1, Scalar::all(0));
	imshow("ori", img_test);
	//imshow("mask", M);
	//waitKey();

	for (int rw = 0; rw < row; rw++) {
		for (int cl = 0; cl < col; cl++) {
			Cb = cul_CbCrs(img_test_cvt, rw, cl)[0];
			Cr = cul_CbCrs(img_test_cvt, rw, cl)[1];
			if (elliptical_judgement(Cb, Cr)) {
				M.at<uchar>(rw, cl) = 255;
				//cout << 0;
				//flag = -1;
			}
			//else cout << 1;
			//flag = -1;
		}
		    //cout << endl;
	}
			//cul_CbCrs(img_test_cvt, );
			//cout << flag;
	imshow("res", M);
	waitKey();
	return 0;
}