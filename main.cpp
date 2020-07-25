#include "bmpimage.h"
#include <opencv2/opencv.hpp>

// using namespace cv;

const int screen_w = 640;
const int screen_h = 480;

int main(int argc, char** argv) {
	// BMPImage image(100, 100, BMPImage::RGB);
	BMPImage image(screen_w, screen_h, 24);
	// image.save("24bit_Black.bmp");
	// image.set(52, 41, red);
	// image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	// image.write_bmp_file("output.bmp");
	uchar b=0, g=0, r=0;
	for (int i = 0; i < screen_w; i++) {
		for (int j = 0; j < screen_h; j++) {
			b = (i * 255) / screen_w;
			g = (j * 255) / screen_h;
			image.at<BGR>(i, j) = BGR{b, g, r};
		}
	}
	for (int i=100; i<200; i++) {
		for (int j=100; j<200; j++) {
			image.at<BGR>(i, j) = BGR{255, 255, 255};
		}
	}
	image.save("images/24bit_block.bmp");

	cv::Mat cvImage = cv::imread("images/opencv-logo.png");
	cv::namedWindow("CV Window");
	cv::imshow("CV Window", cvImage);
	cv::waitKey(0);

	cvImage = cv::imread("images/24bit_block.bmp");
	cv::imshow("CV Window", cvImage);
	cv::waitKey(0);

	cv::destroyWindow("CV Window");


	return 0;
}

