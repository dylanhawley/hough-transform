#include <iostream>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
using namespace cv;
using namespace std;

int main(int argc, char** argv) {
  // Declare the output variables
  Mat dst, cdst;
  const char* default_file = "../cuberover-teleops/localization/data/render_set_5/0degrees.jpg";
  const char* filename = argc >=2 ? argv[1] : default_file;
  // Loads an image
  Mat src = imread( filename, IMREAD_GRAYSCALE );
  // Check if image is loaded fine
  if (src.empty()) {
    printf(" Error opening image\n");
    printf(" Program Arguments: [image_name -- default %s] \n", default_file);
    return -1;
  }
  // Edge detection
  Canny(src, dst, 350, 900, 3);
  imshow("Canny", dst);
  // Copy edges to the images that will display the results in BGR
  cvtColor(dst, cdst, COLOR_GRAY2BGR);
  Mat cdstP(cdst.rows, cdst.cols, CV_8UC3, Scalar(0,0, 0));
  // Standard Hough Line Transform
  vector<Vec2f> lines; // will hold the results of the detection
  HoughLines(dst, lines, 1, CV_PI/180, 40, 0, 0 ); // runs the actual detection
  // Draw the lines
  for (size_t i = 0; i < lines.size(); i++) {
    float rho = lines[i][0], theta = lines[i][1];
    Point pt1, pt2;
    double a = cos(theta), b = sin(theta);
    double x0 = a*rho, y0 = b*rho;
    pt1.x = cvRound(x0 + 1000*(-b));
    pt1.y = cvRound(y0 + 1000*(a));
    pt2.x = cvRound(x0 - 1000*(-b));
    pt2.y = cvRound(y0 - 1000*(a));
    line( cdst, pt1, pt2, Scalar(0,0,255), 3, LINE_AA);
  }
  // Probabilistic Line Transform
  vector<Vec4i> linesP; // will hold the results of the detection
  HoughLinesP(dst, linesP, 1, CV_PI/180, 10, 50, 10 ); // runs the actual detection
  // Draw the lines
  for (size_t i = 0; i < linesP.size(); i++) {
    Vec4i l = linesP[i];
    line(cdstP, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 1, LINE_AA);
        cout << "* line " << i << ": (" << l[0] << ", " << l[1] << "), (";
    cout << l[2] << ", " << l[3] << ")" << endl;
  }
  // Show results
  // imshow("Source", src);
  // imshow("Detected Lines (in red) - Standard Hough Line Transform", cdst);
  imshow("Detected Lines (in red) - Probabilistic Line Transform", cdstP);
  // Wait and Exit
  waitKey();
  return 0;
}