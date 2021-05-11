// Ravinder Mangat 
// 2021-02-02
// Virtual Painter using OpenCV
// Libraries needed from OpenCV
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

// Virtual Painter using Webcam
// Detect colours using hsv space
// Using contours to find where the contours are and find hte x'y position
// Crate circles at these countours

// Global variables
cv::Mat img;
std::vector<std::vector<int>> newPoints; // {{x, y, colour}, {x2, y2, colour2}, ...}

// Create a vector of colours - This is for the colour detected for my markers on my webcam using colourpicker.cpp
std::vector<std::vector<int>> myColours{ {136, 98, 79, 179, 255, 255}, // red
											{78, 107, 0, 136, 255, 255} }; // blue

// These are the colours I want to output on the windows matching the colours from the webcam
std::vector<cv::Scalar> myColourValues { {0, 0, 255}, //red
											{255, 0, 0} }; //blue


// Declare functions
// function for contours
cv::Point getContours(cv::Mat imgDilate) {

	// contours vector
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;


	// Find countours using built in function opencv
	cv::findContours(imgDilate, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	std::vector<std::vector<cv::Point>> conPoly(contours.size());
	std::vector<cv::Rect> boundRect(contours.size()); // Vector to hold points from boundingRect functions

	cv::Point myPoint(0, 0);

	// Filter out noise
	for (int i = 0; i < contours.size(); i++) {

		// Area for concentration 
		int area = cv::contourArea(contours[i]);
		std::cout << area << std::endl;
		
		std::string objectType;

		// Perform drawing of only valid objects not noise
		if (area > 1000) {

			// Find bounding box around shape
			float peri = cv::arcLength(contours[i], true);

			// Approx number of curves
			cv::approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);



			std::cout << conPoly[i].size() << std::endl;

			// get bounding box around shapes

			boundRect[i] = cv::boundingRect(conPoly[i]);

			// Locate points to draw paint
			myPoint.x = boundRect[i].x + (boundRect[i].width / 2);
			myPoint.y = boundRect[i].y;

			// Draw contours on original image using opencv
			//cv::drawContours(img, conPoly, i, cv::Scalar(255, 0, 255), 2);
			//rectangle(img, boundRect[i].tl(), boundRect[i].br(), cv::Scalar(0, 255, 255), 5);
		}
	}

	return myPoint;
}


std::vector<std::vector<int>> findColour(cv::Mat img) {
	cv::Mat imgHSV;

	// Convert picture to HSV space (hue saturation colour)
	cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);

	for (int i = 0; i < myColours.size(); i++) {

		// Define lower and upper limits of colors
		// The first 3 parameters will be the minimum bounds of the colour (hue min, saturation min, value min)
		cv::Scalar lower(myColours[i][0], myColours[i][1], myColours[i][2]);
		// Last 3 parameters are the upper bounds of the colour
		cv::Scalar upper(myColours[i][3], myColours[i][4], myColours[i][5]);

		cv::Mat mask;

		// inrange function to select color
		cv::inRange(imgHSV, lower, upper, mask);

		// test by showing windows
		//cv::imshow(std::to_string(i), mask);

		cv::Point myPoint = getContours(mask);

		// If no colours of interest are visible
		if (myPoint.x != 0 && myPoint.y != 0) {
			newPoints.push_back({ myPoint.x, myPoint.y, i });
		}
	}

	return newPoints;
}

void drawOnWindow(std::vector<std::vector<int>> newPoints, std::vector<cv::Scalar> myColourValues) {

	for (int i = 0; i < newPoints.size(); i++) {
		circle(img, (cv::Point(newPoints[i][0], newPoints[i][1])), 10, myColourValues[newPoints[i][2]], cv::FILLED);
	}
}

int main() {

	// Path to webcam
	cv::VideoCapture cap(0);

	while (true) {

		// Read image and store into img varaiable
		cap.read(img);

		// Find colour function
		newPoints = findColour(img);

		drawOnWindow(newPoints, myColourValues);

		// Show image but we must add delay so it does not instantly close
		cv::imshow("Image", img);
		cv::waitKey(1);
	}

	return 0;
}