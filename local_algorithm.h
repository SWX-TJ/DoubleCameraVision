#ifndef LOCAL_ALGORITHM_H
#define LOCAL_ALGORITHM_H
#include<opencv.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "face_detection.h"
#include "face_alignment.h"
#include "face_identification.h"
using namespace cv;
using namespace std;
class Local_Algorithm
{
public:
    Local_Algorithm();
    /***Local image process Member***/
public:
    /****Calibrate image member****/
    ofstream saveLeftpreprocFile,saveRightpreprocFile;//left pre proc file txt
    // ofstream savepreprocFile;
    string LeftCaliCamFileLisrInfo;//pre proc imagelist file txt
    string RightCaliCamFileLisrInfo;
    string resultLeftCamCal;
    string resultRightCamCal;
    string LeftinnerMatrixXML;//result file xml
    string RightinnerMatrixXML;
    Size board_size;//chess board rows_cornPointNum,cols_cornPointNum
    Size leftImage_size;//left Input Image rows,cols
    Size rightImage_size;//right Input Image rows,cols
    Size real_square_size;//physic chess square size
    int leftimage_count;//Input Image num
    int rightimage_count;//Input Image num
    vector<Point2f> leftimage_points_buf;//cach per Image cornPoint Num
    vector<Point2f> rightimage_points_buf;//cach per Image cornPoint Num
    vector<vector<Point2f>> leftimage_points_seq;//save all cornPoint
    vector<vector<Point2f>> rightimage_points_seq;//save all cornPoint
    /**calibrate Image used member**/
    FileStorage leftinnerMat;//left camera innermat xml
    FileStorage rightinnerMat;
    vector<Point2f> leftcaliimage_points_buf;//cach per Image cornPoint Num
    vector<Point2f> rightcaliimage_points_buf;//cach per Image cornPoint Num
    vector<vector<Point2f>> leftcaliimage_points_seq;//save all cornPoint
    vector<vector<Point2f>> rightcaliimage_points_seq;//save all cornPoint
    int leftcornPoint_count;//caculate cornPoint_count
    int rightcornPoint_count;//caculate cornPoint_count
    Mat leftcameraMatrix=Mat(3,3,CV_32FC1,Scalar::all(0)); /* 摄像机内参数矩阵 */
    Mat rightcameraMatrix=Mat(3,3,CV_32FC1,Scalar::all(0)); /* 摄像机内参数矩阵 */
    vector<int> leftpoint_counts;  // 每幅图像中角点的数量
    vector<int> rightpoint_counts;  // 每幅图像中角点的数量
    Mat leftdistCoeffs=Mat(1,5,CV_32FC1,Scalar::all(0)); /* 摄像机的5个畸变系数：k1,k2,p1,p2,k3 */
    Mat rightdistCoeffs=Mat(1,5,CV_32FC1,Scalar::all(0)); /* 摄像机的5个畸变系数：k1,k2,p1,p2,k3 */
    vector<Mat> lefttvecsMat;  /* 每幅图像的旋转向量 */
    vector<Mat> leftrvecsMat; /* 每幅图像的平移向量 */
    vector<Mat> righttvecsMat;  /* 每幅图像的旋转向量 */
    vector<Mat> rightrvecsMat; /* 每幅图像的平移向量 */
    /***calbration end******/
    /***base image process member***/

    /*******/
    /**seeta face detection member***/
    const char* detection_model_path = "seeta_fd_frontal_v1.0.bin";
    const char* feature_model_path = "seeta_fa_v1.1.bin";
    const char* identification_model_path = "seeta_fr_v1.0.bin";
    vector<seeta::FacialLandmark*>face_feature;
    vector<float*> vilada_feature;
    string facedateBaseFile;


    vector<vector<float>>extral_face_feature_datebase;//从数据库读取的人脸特征数据
    vector<string>extral_face_name_datebase;//从数据库读取的人脸姓名数据
    /*****local image process function*****/
public:
    //set image whitebalance
    Mat WhiteBalanceFunc(Mat &InputImage,Mat &outImage,vector<Vec3f>Gen_param);
    //set image contast and brightness
    Mat ContrastBrightnessFunc(Mat &InputImage,Mat &outImage,vector<Vec2f>param);
    //image binary
    Mat BinaryFunc(Mat &InputImage,Mat &outImage,int binary_variety);
    //CalibrateCamera
    //left calibration image func v1.1
    bool m_LeftCaliPrePoc_1(Mat &InputImage,int &image_count,int max_calibranum);
    //Right calibration image func v1.1
    bool m_RightCaliPrePoc_1(Mat &InputImage,int &image_count,int max_calibranum);
    bool m_CalibrateCamera(bool OnlyLeftCam,bool OnlyRightCam);
    //set init board Size
    void set_chessBoardSize(int width,int height);
    //return use innerCammatrix
    vector<Mat> returnLeftCam(void);
    vector<Mat> returnRightCam(void);
 /**seeta face detection***/
public:
     bool get_facedateBase(vector<vector<float>>&,vector<string>&);
     bool public_getfacedateBase(void);
    Mat faceDetectionFunc(Mat &InputImage);
    bool FaceModule_FacePreTrain(Mat &InputImage,Mat &OutImage,float featureArray[]);
};

#endif // LOCAL_ALGORITHM_H
