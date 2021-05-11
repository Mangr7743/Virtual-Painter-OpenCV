// Ravinder Mangat 
// 2021-02-02
// Virtual Painter using OpenCV
// Libraries needed from OpenCV
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

//////////////////// COLOUR DETECTION ///////////////////////////

cv::Mat imgHSV, mask;
int hmin = 0, smin = 0, vmin = 0;
int hmax = 179, smax = 255, vmax = 255;


int main() {

	// Path to webcam
	cv::VideoCapture cap(0);
	cv::Mat img;


	// Create a track bar so we can change the values of the HSV max and min in real time
	cv::namedWindow("Trackbars", (640, 200));
	cv::createTrackbar("Hue Min", "Trackbars", &hmin, 179);
	cv::createTrackbar("Hue Max", "Trackbars", &hmax, 179);
	cv::createTrackbar("Sat Min", "Trackbars", &smin, 255);
	cv::createTrackbar("Sat Max", "Trackbars", &smax, 255);
	cv::createTrackbar("Val Min", "Trackbars", &vmin, 255);
	cv::createTrackbar("Val Max", "Trackbars", &vmax, 255);

	while (true) {
		// Read image and store into img varaiable
		cap.read(img);

		// Convert picture to HSV space (hue saturation colour)
		cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);

		// Define lower nad uper limits of colors
		cv::Scalar lower(hmin, smin, vmin);
		cv::Scalar upper(hmax, smax, vmax);

		// inrange function to select color
		cv::inRange(imgHSV, lower, upper, mask);

		// output colours
		std::cout << hmin << ", " << smin << ", " << vmin << ", " << hmax << ", " << smax << ", " << vmax << ", " << std::endl;

		cv::imshow("Image", img);
		cv::imshow("Image Mask", mask);
		cv::waitKey(1);

	}

}