#ifndef LOCAL_ALGORITHM_H
#define LOCAL_ALGORITHM_H
#include<opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
class Local_Algorithm
{
public:
    Local_Algorithm();
    //local image process function
public:
    //set image whitebalance
Mat WhiteBalanceFunc(Mat &InputImage,Mat &outImage,vector<Vec3f>Gen_param);
//set image contast and brightness
Mat ContrastBrightnessFunc(Mat &InputImage,Mat &outImage,vector<Vec2f>param);
//image binary
Mat BinaryFunc(Mat &InputImage,Mat &outImage,int binary_variety);
};

#endif // LOCAL_ALGORITHM_H
