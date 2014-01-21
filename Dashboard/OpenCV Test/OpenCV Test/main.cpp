#include "opencv/highgui.h" // Contains GUI elements

using namespace cv;

int main(int argc, char *argv[]) {
    Mat frame;  // Video Matrix
    namedWindow("video", 1);
    VideoCapture cap("http://192.168.1.130:8090/?action=stream.mjpg");
    
    while ( cap.isOpened() ) {
        cap >> frame;
        if(frame.empty()) break;
        
        imshow("video", frame);
        if(waitKey(30) >= 0) break;
    }
    
    return 0;
}