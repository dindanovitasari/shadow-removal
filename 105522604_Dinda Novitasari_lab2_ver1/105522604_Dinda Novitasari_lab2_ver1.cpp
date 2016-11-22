// 105522604_Dinda Novitasari_lab2_ver1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include <iostream>
using namespace std;
using namespace cv;


int main()
{
	Mat frame;
	Ptr<BackgroundSubtractorMOG2> pMOG2;
	//length of history
	int history = 20;
	/*Threshold on the squared Mahalanobis distance between the pixel and 
	the model to decide whether a pixel 
	is well described by the background model. */
	float thresholdValue = 12;
	bool shadowDetection = true;

	pMOG2 = new BackgroundSubtractorMOG2(history, thresholdValue, shadowDetection);

	string filename = "video.avi";
	VideoCapture capture(filename);

	if (!capture.isOpened())
		throw "Error cannot find the video";

	namedWindow("Original video", 1);
	for (;;)
	{
		Mat fgMaskMOG2, foregroundImage, dilateImage, erodeImage, removeShadow;

		//Stream operator to read the next video frame.
		capture >> frame;
		if (frame.empty())
			break;

		//0-based index of the frame to be decoded/captured next.
		int frameOf = capture.get(CV_CAP_PROP_POS_FRAMES);

		//number one
		//Updates the background model and computes the foreground mask
		pMOG2->operator()(frame, fgMaskMOG2, 0.01);

		//removing shadow
		threshold(fgMaskMOG2, removeShadow, 130, 255, THRESH_BINARY);

		//morphology
		erode(removeShadow, erodeImage, Mat());
		dilate(erodeImage, dilateImage, Mat());

		//copy image to get the original one
		foregroundImage = frame.clone();

		for (int row = 0; row<foregroundImage.rows; row++) {
			for (int col = 0; col<foregroundImage.cols; col++) {
				if (removeShadow.at<uchar>(row, col) <= 5) {
					foregroundImage.at<Vec3b>(row, col)[0] = 0;
					foregroundImage.at<Vec3b>(row, col)[1] = 0;
					foregroundImage.at<Vec3b>(row, col)[2] = 0;
				}

			}
		}

		imshow("Original video", foregroundImage);

		if (frameOf == 150) {
			imwrite("resultremoveshadow150.jpg", removeShadow);
			imwrite("resultMorphology150.jpg", dilateImage);
			imwrite("resultFinal150.jpg", foregroundImage);
		}
		else if (frameOf == 200) {
			imwrite("resultremoveshadow200.jpg", removeShadow);
			imwrite("resultMorphology200.jpg", dilateImage);
			imwrite("resultFinal200.jpg", foregroundImage);
		}


		waitKey(20);
	}

	waitKey(0);
	return 0;
}

