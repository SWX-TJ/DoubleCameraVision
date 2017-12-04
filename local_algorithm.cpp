#include "local_algorithm.h"

Local_Algorithm::Local_Algorithm()
{
    leftimage_count = 0;//Left Input Image num
    rightimage_count = 0;//Right Input Image num
    LeftCaliCamFileLisrInfo = "LeftCaliImageInfo.txt";
    RightCaliCamFileLisrInfo = "RightCaliImageInfo.txt";
    LeftinnerMatrixXML = "leftcaminnerMat.xml";
    RightinnerMatrixXML = "rightcaminnerMat.xml";
    resultLeftCamCal = "left_caliberation_result.txt";
    resultRightCamCal = "right_caliberation_result.txt";

}

Mat Local_Algorithm::WhiteBalanceFunc(Mat &InputImage, Mat &outImage, vector<Vec3f> Gen_param)
{

    return outImage;
}

Mat Local_Algorithm::ContrastBrightnessFunc(Mat &InputImage, Mat &outImage, vector<Vec2f> param)
{
return outImage;
}

Mat Local_Algorithm::BinaryFunc(Mat &InputImage, Mat &outImage, int binary_variety)
{
return outImage;
}

bool Local_Algorithm::m_LeftCaliPrePoc_1(Mat &InputImage, int &image_count,int max_calibranum)
{

    if(image_count==max_calibranum)
    {
        return true;
    }
    if(image_count ==0)
    {
        saveLeftpreprocFile.open(LeftCaliCamFileLisrInfo,ios_base::out|ios_base::trunc);
        String ImagefileName = "Leftchess";
        String Imagefiletype = ".bmp";
        if(findChessboardCorners(InputImage,board_size,leftimage_points_buf))
        {
            ImagefileName +=to_string(image_count);
            ImagefileName +=Imagefiletype;
            imwrite(ImagefileName,InputImage);
            drawChessboardCorners(InputImage,board_size,leftimage_points_buf,true);
            imwrite("Leftfindcornpoint.bmp",InputImage);
            saveLeftpreprocFile<<ImagefileName<<endl;
            image_count++;
            return false;
        }
        else
        {
            return false;
        }
    }
    else
    {
        String ImagefileName = "Leftchess";
        String Imagefiletype = ".bmp";
        if(findChessboardCorners(InputImage,board_size,leftimage_points_buf))
        {
            ImagefileName +=to_string(image_count);
            ImagefileName +=Imagefiletype;
            imwrite(ImagefileName,InputImage);
            drawChessboardCorners(InputImage,board_size,leftimage_points_buf,true);
            imwrite("Leftfindcornpoint.bmp",InputImage);
            saveLeftpreprocFile<<ImagefileName<<endl;
            image_count++;
            return false;
        }
        else
        {
            return false;
        }
    }
    return false;
}


bool Local_Algorithm::m_RightCaliPrePoc_1(Mat &InputImage, int &image_count, int max_calibranum)
{
    if(image_count==max_calibranum)
    {
        return true;
    }
    if(image_count ==0)
    {
        saveRightpreprocFile.open(RightCaliCamFileLisrInfo,ios_base::out|ios_base::trunc);
        String ImagefileName = "Rightchess";
        String Imagefiletype = ".bmp";
        if(findChessboardCorners(InputImage,board_size,rightimage_points_buf))
        {
            ImagefileName +=to_string(image_count);
            ImagefileName +=Imagefiletype;
            imwrite(ImagefileName,InputImage);
            drawChessboardCorners(InputImage,board_size,rightimage_points_buf,true);
            imwrite("Rightfindcornpoint.bmp",InputImage);
            saveRightpreprocFile<<ImagefileName<<endl;
            image_count++;
            return false;
        }
        else
        {
            return false;
        }
    }
    else
    {
        String ImagefileName = "Rightchess";
        String Imagefiletype = ".bmp";
        if(findChessboardCorners(InputImage,board_size,rightimage_points_buf))
        {
            ImagefileName +=to_string(image_count);
            ImagefileName +=Imagefiletype;
            imwrite(ImagefileName,InputImage);
            drawChessboardCorners(InputImage,board_size,rightimage_points_buf,true);
            imwrite("Rightfindcornpoint.bmp",InputImage);
            saveRightpreprocFile<<ImagefileName<<endl;
            image_count++;
            return false;
        }
        else
        {
            return false;
        }
    }
    return false;
}



bool Local_Algorithm::m_CalibrateCamera(bool OnlyLeftCam,bool OnlyRightCam)
{

    if(OnlyLeftCam)
    {
        ifstream openLeftCaliImgList;
        openLeftCaliImgList.open(LeftCaliCamFileLisrInfo);
        leftinnerMat.open(LeftinnerMatrixXML,FileStorage::WRITE);
        ofstream fout(resultLeftCamCal);
        fout<<"左相机标定结果"<<endl;
        string leftfilename;
        while (getline(openLeftCaliImgList,leftfilename))
        {
            leftimage_count++;
            Mat imageInput=imread(leftfilename);
            if (leftimage_count == 1)
            {
                leftImage_size.width = imageInput.cols;
                leftImage_size.height =imageInput.rows;
            }
            if (0 == findChessboardCorners(imageInput,board_size,leftcaliimage_points_buf))
            {
               return false;
            }
            else
            {
                Mat grayImage;
                cvtColor(imageInput,grayImage,CV_RGB2GRAY);
                find4QuadCornerSubpix(grayImage,leftcaliimage_points_buf,Size(5,5));
                leftcaliimage_points_seq.push_back(leftcaliimage_points_buf);
            }
        }
        vector<vector<Point3f>> object_points;
        int i,j,t;
        for (t=0;t<leftimage_count;t++)
        {
            vector<Point3f> tempPointSet;
            for (i=0;i<board_size.height;i++)
            {
                for (j=0;j<board_size.width;j++)
                {
                    Point3f realPoint;
                    realPoint.x = i* real_square_size.width;
                    realPoint.y = j* real_square_size.height;
                    realPoint.z = 0;
                    tempPointSet.push_back(realPoint);
                }
            }
            object_points.push_back(tempPointSet);
        }
        for (i=0;i<leftimage_count;i++)
        {
            leftpoint_counts.push_back(board_size.width*board_size.height);
        }
        calibrateCamera(object_points,leftcaliimage_points_seq,leftImage_size,leftcameraMatrix,leftdistCoeffs,leftrvecsMat,lefttvecsMat,0);
        double total_err = 0.0;
        double err = 0.0;
        vector<Point2f> image_points2;
        for (int i=0;i<leftimage_count;i++)
        {
            vector<Point3f> tempPointSet=object_points[i];
            projectPoints(tempPointSet,leftrvecsMat[i],lefttvecsMat[i],leftcameraMatrix,leftdistCoeffs,image_points2);
            vector<Point2f> tempImagePoint = leftcaliimage_points_seq[i];
            Mat tempImagePointMat = Mat(1,tempImagePoint.size(),CV_32FC2);
            Mat image_points2Mat = Mat(1,image_points2.size(), CV_32FC2);
            for (size_t j = 0 ; j < tempImagePoint.size(); j++)
            {
                image_points2Mat.at<Vec2f>(0,j) = Vec2f(image_points2[j].x, image_points2[j].y);
                tempImagePointMat.at<Vec2f>(0,j) = Vec2f(tempImagePoint[j].x, tempImagePoint[j].y);
            }
            err = norm(image_points2Mat, tempImagePointMat, NORM_L2);
            total_err += err/=  leftpoint_counts[i];
            fout<<"第"<<i+1<<"幅图像平均误差："<<err<<"像素"<<endl;
        }
        fout<<string("总体平均误差：")<<total_err/leftimage_count<<"pix"<<endl<<endl;
        Mat rotation_matrix = Mat(3,3,CV_32FC1, Scalar::all(0));
        fout<<string("相机内参数矩阵：")<<endl;
        leftinnerMat<<"LeftInnerCamMatrix"<<leftcameraMatrix;
        fout<<leftcameraMatrix<<endl<<endl;
        fout<<string("畸变系数：\n");
        leftinnerMat<<"LeftdistCoeffs"<<leftdistCoeffs;
        fout<<leftdistCoeffs<<endl<<endl<<endl;
        for (int i=0; i<leftimage_count; i++)
        {
            fout<<string("第")<<i+1<<"幅图像的旋转向量："<<endl;
            fout<<lefttvecsMat[i]<<endl;
            Rodrigues(lefttvecsMat[i],rotation_matrix);
            fout<<"第"<<i+1<<"幅图像的旋转矩阵："<<endl;
            fout<<rotation_matrix<<endl;
            fout<<"第"<<i+1<<"幅图像的平移向量："<<endl;
            fout<<leftrvecsMat[i]<<endl<<endl;
        }
        fout<<endl;
        leftinnerMat.release();
    }
    if(OnlyRightCam)
    {
        ifstream openRightCaliImgList;
        openRightCaliImgList.open(RightCaliCamFileLisrInfo);
        rightinnerMat.open( RightinnerMatrixXML,FileStorage::WRITE);
        ofstream fout(resultRightCamCal);
        fout<<"右相机标定结果"<<endl;
        string rightfilename;
        while (getline(openRightCaliImgList,rightfilename))
        {
            rightimage_count++;
            Mat imageInput=imread(rightfilename);
            if (rightimage_count == 1)
            {
                rightImage_size.width = imageInput.cols;
                rightImage_size.height =imageInput.rows;
            }
            if (0 == findChessboardCorners(imageInput,board_size,rightcaliimage_points_buf))
            {
             return false;
            }
            else
            {
                Mat grayImage;
                cvtColor(imageInput,grayImage,CV_RGB2GRAY);
                find4QuadCornerSubpix(grayImage,rightcaliimage_points_buf,Size(5,5)); //对粗提取的角点进行精确化
                rightcaliimage_points_seq.push_back(rightcaliimage_points_buf);  //保存亚像素角点
            }
        }
        vector<vector<Point3f>> object_points;
        int i,j,t;
        for (t=0;t<rightimage_count;t++)
        {
            vector<Point3f> tempPointSet;
            for (i=0;i<board_size.height;i++)
            {
                for (j=0;j<board_size.width;j++)
                {
                    Point3f realPoint;
                    realPoint.x = i* real_square_size.width;
                    realPoint.y = j* real_square_size.height;
                    realPoint.z = 0;
                    tempPointSet.push_back(realPoint);
                }
            }
            object_points.push_back(tempPointSet);
        }
        for (i=0;i<rightimage_count;i++)
        {
            rightpoint_counts.push_back(board_size.width*board_size.height);
        }
        calibrateCamera(object_points,rightcaliimage_points_seq,rightImage_size,rightcameraMatrix,rightdistCoeffs,rightrvecsMat,righttvecsMat,0);
        double total_err = 0.0;
        double err = 0.0;
        vector<Point2f> image_points2;
        for (int i=0;i<rightimage_count;i++)
        {
            vector<Point3f> tempPointSet=object_points[i];
            projectPoints(tempPointSet,rightrvecsMat[i],righttvecsMat[i],rightcameraMatrix,rightdistCoeffs,image_points2);
            vector<Point2f> tempImagePoint = rightcaliimage_points_seq[i];
            Mat tempImagePointMat = Mat(1,tempImagePoint.size(),CV_32FC2);
            Mat image_points2Mat = Mat(1,image_points2.size(), CV_32FC2);
            for (size_t j = 0 ; j < tempImagePoint.size(); j++)
            {
                image_points2Mat.at<Vec2f>(0,j) = Vec2f(image_points2[j].x, image_points2[j].y);
                tempImagePointMat.at<Vec2f>(0,j) = Vec2f(tempImagePoint[j].x, tempImagePoint[j].y);
            }
            err = norm(image_points2Mat, tempImagePointMat, NORM_L2);
            total_err += err/=  rightpoint_counts[i];
            fout<<"第"<<i+1<<"幅图像平均误差："<<err<<"像素"<<endl;
        }
        fout<<string("总体平均误差：")<<total_err/rightimage_count<<"pix"<<endl<<endl;
        Mat rotation_matrix = Mat(3,3,CV_32FC1, Scalar::all(0));
        fout<<"相机内参数矩阵："<<endl;
        rightinnerMat<<"RightInnerCamMatrix"<<rightcameraMatrix;
        fout<<rightcameraMatrix<<endl<<endl;
        fout<<"畸变系数：\n";
        rightinnerMat<<"RightdistCoeffs"<<rightdistCoeffs;
        fout<<rightdistCoeffs<<endl<<endl<<endl;
        for (int i=0; i<rightimage_count; i++)
        {
            fout<<"第"<<i+1<<"幅图像的旋转向量："<<endl;
            fout<<righttvecsMat[i]<<endl;
            Rodrigues(righttvecsMat[i],rotation_matrix);
            fout<<"第"<<i+1<<"幅图像的旋转矩阵："<<endl;
            fout<<rotation_matrix<<endl;
            fout<<"第"<<i+1<<"幅图像的平移向量："<<endl;
            fout<<rightrvecsMat[i]<<endl<<endl;
        }
        fout<<endl;
        rightinnerMat.release();
    }
     return true;
}



void Local_Algorithm::set_chessBoardSize(int width, int height)
{
    board_size.width = width;
    board_size.height = height;
}

vector<Mat> Local_Algorithm::returnLeftCam()
{
    vector<Mat>leftcammatrixs;
    leftinnerMat.open(LeftinnerMatrixXML,FileStorage::READ);
    leftinnerMat["LeftInnerCamMatrix"] >> leftcameraMatrix;
    leftinnerMat["LeftdistCoeffs"] >> leftdistCoeffs;
    leftcammatrixs.push_back(leftcameraMatrix);
    leftcammatrixs.push_back(leftdistCoeffs);
    leftinnerMat.release();
    return leftcammatrixs;
}

vector<Mat> Local_Algorithm::returnRightCam()
{
    vector<Mat>rightcammatrixs;
    rightinnerMat.open( RightinnerMatrixXML,FileStorage::READ);
    rightinnerMat["RightInnerCamMatrix"] >> rightcameraMatrix;
    rightinnerMat["RightdistCoeffs"] >> rightdistCoeffs;
    rightcammatrixs.push_back(rightcameraMatrix);
    rightcammatrixs.push_back(rightdistCoeffs);
    rightinnerMat.release();
    return rightcammatrixs;
}
