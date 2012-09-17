
#include <stdlib.h>
#include <iostream>
#include <stdio.h>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "toolbarconfig.h"


using namespace cv;


// Global variables
const char* window_name = "Hough Line Detection";

ToolbarConfig
    gaussian = ToolbarConfig(0, 15, 1, 6),
    canny = ToolbarConfig(20, 150, 2, 40),
    hough = ToolbarConfig(50, 400, 10, 200);

Mat input;

// Function prototypes
void update(int, void*);
void chromakey(const Mat under, const Mat over, Mat *dst, const Scalar& color);
void help();


/**
 * Creates an interactive example of running hough line detection on a
 * sample image
 */
int main( int argc, char** argv ) {
    const char* filename = argc >= 2 ? argv[1] : "pic1.png";

    input = imread(filename, CV_LOAD_IMAGE_COLOR); if(input.empty()) {
        help();

        std::cout << "Can not open " << filename << std::endl;
        return -1;
    }

    // Convert the image to grayscale
    cvtColor(input, input, CV_BGR2GRAY);

    // Create a window
    namedWindow(window_name, CV_WINDOW_AUTOSIZE);

    // Create trackbars for the user to enter thresholds
    createTrackbar("Gaussian Kernel Size", window_name, &(gaussian.t_current), gaussian.tmax(), update);
    createTrackbar("Canny Min Threshold", window_name, &(canny.t_current), canny.tmax(), update);
    createTrackbar("Hough Line Threshold", window_name, &(hough.t_current), hough.tmax(), update);

    // Show the image
    update(NULL, NULL);
    
    // Wait until user exit program by pressing a key
    waitKey(0);

    return 0;
}


/**
 * Trackbar callback - updates the display
 */
void update(int, void*) {
    const int CANNY_RATIO = 3, CANNY_KERNEL_SIZE = 3;

    Mat blurred_input, canny_edges, hough_lines;

    // Reduce noise with a gaussian kernel
    if(gaussian.current() != 0) {
        blur(input, blurred_input, Size(gaussian.current(), gaussian.current()));
    } else {
        blurred_input = input;
    }

    // Run Canny edge detector
    Canny(blurred_input, canny_edges, canny.current(), canny.current()*CANNY_RATIO, CANNY_KERNEL_SIZE);

    // ==== Begin Hough line detector phase

    // Create a vector to store the located lines in
    vector<Vec2f> line_vector;

    // Run the transform
    HoughLines(canny_edges, line_vector, 1, CV_PI/180, hough.current(), 0, 0);
    
    //std::cout << lines.size() << " lines detected" << std::endl;

    // Prepare the hough_lines image
    hough_lines = Mat::zeros(canny_edges.rows, canny_edges.cols, CV_8UC3);

    // Draw detected lines into an image
    for(size_t i = 0; i < line_vector.size(); i++) {
        float rho = line_vector[i][0], theta = line_vector[i][1];
        Point pt1, pt2;
        
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        
        line(hough_lines, pt1, pt2, Scalar(0, 0, 255), 3, CV_AA);
    }

    // Overlay the hough lines onto the original blurred image
    Mat blurred_input_color, canny_edges_color, input_with_canny, combined_images;
    cvtColor(blurred_input, blurred_input_color, CV_GRAY2BGR);
    cvtColor(canny_edges, canny_edges_color, CV_GRAY2BGR);

    chromakey(blurred_input_color, canny_edges_color, &input_with_canny, Scalar(0, 0, 0));
    chromakey(input_with_canny, hough_lines, &combined_images, Scalar(0, 0, 0));

    // Display the result
    imshow(window_name, combined_images);
}

/**
 * Takes two images and overlays them, using color as a chroma-key
 * Any pixels in the 'over' image that match the given color value will
 * effectively be transparent - the 'under' image will show through
 *
 * @precondition: All passed images must first be in BGR format
 */
void chromakey(const Mat under, const Mat over, Mat *dst, const Scalar& color) {
    // Mats must be the same size
    if(under.rows != over.rows || under.cols != over.cols) {
        std::cout << "Error, image dimensions must match" << std::endl;
        return;
    }
    
    // Create the destination matrix
    *dst = Mat(under.rows, under.cols, CV_8UC3, CV_RGB(0,0,0));

    for(int y=0; y<under.rows; y++) {
        for(int x=0; x<under.cols; x++) {
            if(over.at<Vec3b>(y,x)[0] == color[0] && over.at<Vec3b>(y,x)[1] == color[1] && over.at<Vec3b>(y,x)[2] == color[2]) {
                dst->at<Vec3b>(y,x)[0] = under.at<Vec3b>(y,x)[0];
                dst->at<Vec3b>(y,x)[1] = under.at<Vec3b>(y,x)[1];
                dst->at<Vec3b>(y,x)[2] = under.at<Vec3b>(y,x)[2];
            } else {
                dst->at<Vec3b>(y,x)[0] = over.at<Vec3b>(y,x)[0];
                dst->at<Vec3b>(y,x)[1] = over.at<Vec3b>(y,x)[1];
                dst->at<Vec3b>(y,x)[2] = over.at<Vec3b>(y,x)[2];
            }
        }
    }
}


/**
 * Prints usage information
 */
void help() {
    std::cout << "\nThis program demonstrates line finding with the Hough transform.\n" "Usage:\n"
    "./houghlines <image_name>, Default is pic1.png\n" << std::endl;
}

