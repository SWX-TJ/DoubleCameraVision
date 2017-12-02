#include "local_algorithm.h"

Local_Algorithm::Local_Algorithm()
{
    leftimage_count = 0;//Left Input Image num
    rightimage_count = 0;//Right Input Image num
    LeftCaliCamFileLisrInfo = "LeftCaliImageInfo.txt";
    RightCaliCamFileLisrInfo = "RightCaliImageInfo.txt";
}

Mat Local_Algorithm::WhiteBalanceFunc(Mat &InputImage, Mat &outImage, vector<Vec3f> Gen_param)
{

}

Mat Local_Algorithm::ContrastBrightnessFunc(Mat &InputImage, Mat &outImage, vector<Vec2f> param)
{

}

Mat Local_Algorithm::BinaryFunc(Mat &InputImage, Mat &outImage, int binary_variety)
{

}

Mat Local_Algorithm::m_LeftCaliPrePoc(VideoCapture &left_cap)
{
    Mat returnFrame;
    ofstream savepreprocFile;
    savepreprocFile.open(LeftCaliCamFileLisrInfo,ios_base::out|ios_base::trunc);
    int imagecount = 0;
    int badresult = 0;
    while(1)
    {
        String ImagefileName = "Leftchess";
        String Imagefiletype = ".bmp";
        left_cap>>returnFrame;
        if(findChessboardCorners(returnFrame,board_size,leftimage_points_buf))
        {
            if(imagecount==6)
            {
                savepreprocFile.close();
                destroyWindow("leftprocImage");
                break;
            }
            ImagefileName +=to_string(imagecount);
            ImagefileName +=Imagefiletype;
            //cout<<ImagefileName<<endl;
            imwrite(ImagefileName,returnFrame);
            drawChessboardCorners(returnFrame,board_size,leftimage_points_buf,true);
            imwrite("Leftfindcornpoint.bmp",returnFrame);
            savepreprocFile<<ImagefileName<<endl;
            imagecount++;
            cout<<"next frame"<<endl;
        }
        else
        {
            if(badresult>=20)
            {
                destroyWindow("procImage");
                break;
            }
            cout<<"there is no corner point"<<endl;
            badresult++;
        }
          imshow("leftprocImage",returnFrame);
          waitKey(1000);

    }
    return returnFrame;
}

Mat Local_Algorithm::m_RightCaliPrePoc(VideoCapture &right_cap)
{
    Mat returnFrame;
    ofstream savepreprocFile;
    savepreprocFile.open(RightCaliCamFileLisrInfo,ios_base::out|ios_base::trunc);
    int imagecount = 0;
    int badresult = 0;
    while(1)
    {
        String ImagefileName = "Rightchess";
        String Imagefiletype = ".bmp";
        right_cap>>returnFrame;
        if(findChessboardCorners(returnFrame,board_size,rightimage_points_buf))
        {
            if(imagecount==6)
            {
                savepreprocFile.close();
                  destroyWindow("rightprocImage");
                break;
            }
            ImagefileName +=to_string(imagecount);
            ImagefileName +=Imagefiletype;
            cout<<ImagefileName<<endl;
            imwrite(ImagefileName,returnFrame);
            drawChessboardCorners(returnFrame,board_size,rightimage_points_buf,true);
            imwrite("Rightfindcornpoint.bmp",returnFrame);
            savepreprocFile<<ImagefileName<<endl;
            imagecount++;
             cout<<"next frame"<<endl;
        }
        else
        {
            if(badresult>=20)
            {
                break;
            }
            cout<<"there is no corner point"<<endl;
            badresult++;
        }
        imshow("rightprocImage",returnFrame);
        waitKey(1000);
    }
    return returnFrame;
}

Mat Local_Algorithm::m_AllCaliPrePoc(VideoCapture &left_cap, VideoCapture &right_cap)
{
    Mat left_returnFrame,right_returnFrame;
    ofstream saveLeftpreprocFile,saveRightpreprocFile;
    saveLeftpreprocFile.open(LeftCaliCamFileLisrInfo,ios_base::out|ios_base::trunc);
    saveRightpreprocFile.open(RightCaliCamFileLisrInfo,ios_base::out|ios_base::trunc);
    int left_imagecount = 0;
    int right_imagecount = 0;
    int left_badresult = 0;
    int right_badresult = 0;
    while(1)
    {
        String leftImagefileName = "Leftchess";
        String leftImagefiletype = ".bmp";
        String rightImagefileName = "Rightchess";
        String rightImagefiletype = ".bmp";
        left_cap>>left_returnFrame;
        right_cap>>right_returnFrame;
        if(findChessboardCorners(left_returnFrame,board_size,leftimage_points_buf)&&findChessboardCorners(right_returnFrame,board_size,rightimage_points_buf))
        {
            if(left_imagecount==6&&right_imagecount==6)
            {
                saveLeftpreprocFile.close();
                saveRightpreprocFile.close();
                destroyWindow("leftprocImage");
                destroyWindow("rightprocImage");
                break;
            }
            leftImagefileName +=to_string(left_imagecount);
            leftImagefileName +=leftImagefiletype;
            rightImagefileName +=to_string(right_imagecount);
            rightImagefileName +=rightImagefiletype;
            imwrite(leftImagefileName,left_returnFrame);
            imwrite(rightImagefileName,right_returnFrame);
            drawChessboardCorners(left_returnFrame,board_size,leftimage_points_buf,true);
            imwrite("Leftfindcornpoint.bmp",left_returnFrame);
            drawChessboardCorners(right_returnFrame,board_size,rightimage_points_buf,true);
            imwrite("rightfindcornpoint.bmp",right_returnFrame);
            saveLeftpreprocFile<<leftImagefileName<<endl;
            saveRightpreprocFile<<rightImagefileName<<endl;
            left_imagecount++;
            right_imagecount++;
            imshow("leftprocImage",left_returnFrame);
            imshow("rightprocImage",right_returnFrame);
            waitKey(2000);
        }
        else
        {
            if(left_badresult>=20||right_badresult>=20)
            {
                break;
            }
            cout<<"there is no corner point"<<endl;
            left_badresult++;
            right_badresult++;
        }
    }
    return left_returnFrame;
}

void Local_Algorithm::m_CalibrateCamera(bool OnlyLeftCam,bool OnlyRightCam,bool OnlyAllCam)
{

    if(OnlyLeftCam)
    {
        ifstream openLeftCaliImgList;
        openLeftCaliImgList.open(LeftCaliCamFileLisrInfo);
      //  openRightCaliImgList.open(RightCaliCamFileLisrInfo);
        ofstream fout("left_caliberation_result.txt");  /* 保存标定结果的文件 */
        string leftfilename;
        while (getline(openLeftCaliImgList,leftfilename))
        {
            leftimage_count++;
            // 用于观察检验输出
            Mat imageInput=imread(leftfilename);
            if (leftimage_count == 1)  //读入第一张图片时获取图像宽高信息
            {
                leftImage_size.width = imageInput.cols;
                leftImage_size.height =imageInput.rows;
            }
            if (0 == findChessboardCorners(imageInput,board_size,leftcaliimage_points_buf))
            {
                cout<<"can not find chessboard corners!\n"; //找不到角点
            }
            else
            {
                Mat grayImage;
                cvtColor(imageInput,grayImage,CV_RGB2GRAY);
                /* 亚像素精确化 */
                find4QuadCornerSubpix(grayImage,leftcaliimage_points_buf,Size(5,5)); //对粗提取的角点进行精确化
                //cornerSubPix(grayImage,image_points_buf,Size(5,5),Size(-1,-1),TermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER,30,0.1));
                leftcaliimage_points_seq.push_back(leftcaliimage_points_buf);  //保存亚像素角点
                /* 在图像上显示角点位置 */
                drawChessboardCorners(grayImage,board_size,leftcaliimage_points_buf,true); //用于在图片中标记角点
                imshow("leftCamera Calibration",grayImage);//显示图片
                waitKey(500);//暂停0.5S
            }
        }
        vector<vector<Point3f>> object_points; /* 保存标定板上角点的三维坐标 */
        /* 初始化标定板上角点的三维坐标 */
        int i,j,t;
        for (t=0;t<leftimage_count;t++)
        {
            vector<Point3f> tempPointSet;
            for (i=0;i<board_size.height;i++)
            {
                for (j=0;j<board_size.width;j++)
                {
                    Point3f realPoint;
                    /* 假设标定板放在世界坐标系中z=0的平面上 */
                    realPoint.x = i* real_square_size.width;
                    realPoint.y = j* real_square_size.height;
                    realPoint.z = 0;
                    tempPointSet.push_back(realPoint);
                }
            }
            object_points.push_back(tempPointSet);
        }

        /* 初始化每幅图像中的角点数量，假定每幅图像中都可以看到完整的标定板 */
        for (i=0;i<leftimage_count;i++)
        {
            leftpoint_counts.push_back(board_size.width*board_size.height);
        }
        /* 开始标定 */
        calibrateCamera(object_points,leftcaliimage_points_seq,leftImage_size,leftcameraMatrix,leftdistCoeffs,leftrvecsMat,lefttvecsMat,0);
        //        //对标定结果进行评价
        double total_err = 0.0; /* 所有图像的平均误差的总和 */
        double err = 0.0; /* 每幅图像的平均误差 */
        vector<Point2f> image_points2; /* 保存重新计算得到的投影点 */
        // cout<<"No problem"<<endl;
        for (int i=0;i<leftimage_count;i++)
        {
            vector<Point3f> tempPointSet=object_points[i];
            /* 通过得到的摄像机内外参数，对空间的三维点进行重新投影计算，得到新的投影点 */
            projectPoints(tempPointSet,leftrvecsMat[i],lefttvecsMat[i],leftcameraMatrix,leftdistCoeffs,image_points2);
            //            /* 计算新的投影点和旧的投影点之间的误差*/
            vector<Point2f> tempImagePoint = leftcaliimage_points_seq[i];
            Mat tempImagePointMat = Mat(1,tempImagePoint.size(),CV_32FC2);
            Mat image_points2Mat = Mat(1,image_points2.size(), CV_32FC2);
            for (int j = 0 ; j < tempImagePoint.size(); j++)
            {
                image_points2Mat.at<Vec2f>(0,j) = Vec2f(image_points2[j].x, image_points2[j].y);
                tempImagePointMat.at<Vec2f>(0,j) = Vec2f(tempImagePoint[j].x, tempImagePoint[j].y);
            }
            err = norm(image_points2Mat, tempImagePointMat, NORM_L2);
            total_err += err/=  leftpoint_counts[i];
            fout<<"the"<<i+1<<"per img avr_err："<<err<<"pix"<<endl;
        }
        fout<<"总体平均误差："<<total_err/leftimage_count<<"pix"<<endl<<endl;
        Mat rotation_matrix = Mat(3,3,CV_32FC1, Scalar::all(0)); /* 保存每幅图像的旋转矩阵 */
        fout<<"相机内参数矩阵："<<endl;
        fout<<leftcameraMatrix<<endl<<endl;
        fout<<"畸变系数：\n";
        fout<<leftdistCoeffs<<endl<<endl<<endl;
        for (int i=0; i<leftimage_count; i++)
        {
            fout<<"第"<<i+1<<"幅图像的旋转向量："<<endl;
            fout<<lefttvecsMat[i]<<endl;
            /* 将旋转向量转换为相对应的旋转矩阵 */
            Rodrigues(lefttvecsMat[i],rotation_matrix);
            fout<<"第"<<i+1<<"幅图像的旋转矩阵："<<endl;
            fout<<rotation_matrix<<endl;
            fout<<"第"<<i+1<<"幅图像的平移向量："<<endl;
            fout<<leftrvecsMat[i]<<endl<<endl;
        }
        fout<<endl;
        destroyWindow("leftCamera Calibration");
    }
   if(OnlyRightCam)
    {
        ifstream openRightCaliImgList;
        //openLeftCaliImgList.open(LeftCaliCamFileLisrInfo);
        openRightCaliImgList.open(RightCaliCamFileLisrInfo);
        ofstream fout("right_caliberation_result.txt");  /* 保存标定结果的文件 */
        string rightfilename;
        while (getline(openRightCaliImgList,rightfilename))
        {
            rightimage_count++;
            // 用于观察检验输出
            Mat imageInput=imread(rightfilename);
            if (rightimage_count == 1)  //读入第一张图片时获取图像宽高信息
            {
                rightImage_size.width = imageInput.cols;
                rightImage_size.height =imageInput.rows;
            }
            if (0 == findChessboardCorners(imageInput,board_size,rightcaliimage_points_buf))
            {
                cout<<"can not find chessboard corners!\n"; //找不到角点
            }
            else
            {
                Mat grayImage;
                cvtColor(imageInput,grayImage,CV_RGB2GRAY);
                /* 亚像素精确化 */
                find4QuadCornerSubpix(grayImage,rightcaliimage_points_buf,Size(5,5)); //对粗提取的角点进行精确化
                //cornerSubPix(grayImage,image_points_buf,Size(5,5),Size(-1,-1),TermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER,30,0.1));
                rightcaliimage_points_seq.push_back(rightcaliimage_points_buf);  //保存亚像素角点
                /* 在图像上显示角点位置 */
                drawChessboardCorners(grayImage,board_size,rightcaliimage_points_buf,true); //用于在图片中标记角点
                imshow("rightCamera Calibration",grayImage);//显示图片
                waitKey(500);//暂停0.5S
            }
        }
        vector<vector<Point3f>> object_points; /* 保存标定板上角点的三维坐标 */
        /* 初始化标定板上角点的三维坐标 */
        int i,j,t;
        for (t=0;t<rightimage_count;t++)
        {
            vector<Point3f> tempPointSet;
            for (i=0;i<board_size.height;i++)
            {
                for (j=0;j<board_size.width;j++)
                {
                    Point3f realPoint;
                    /* 假设标定板放在世界坐标系中z=0的平面上 */
                    realPoint.x = i* real_square_size.width;
                    realPoint.y = j* real_square_size.height;
                    realPoint.z = 0;
                    tempPointSet.push_back(realPoint);
                }
            }
            object_points.push_back(tempPointSet);
        }

        /* 初始化每幅图像中的角点数量，假定每幅图像中都可以看到完整的标定板 */
        for (i=0;i<rightimage_count;i++)
        {
            rightpoint_counts.push_back(board_size.width*board_size.height);
        }
        /* 开始标定 */
        calibrateCamera(object_points,rightcaliimage_points_seq,rightImage_size,rightcameraMatrix,rightdistCoeffs,rightrvecsMat,righttvecsMat,0);
        //        //对标定结果进行评价
        double total_err = 0.0; /* 所有图像的平均误差的总和 */
        double err = 0.0; /* 每幅图像的平均误差 */
        vector<Point2f> image_points2; /* 保存重新计算得到的投影点 */
        // cout<<"No problem"<<endl;
        for (int i=0;i<rightimage_count;i++)
        {
            vector<Point3f> tempPointSet=object_points[i];
            /* 通过得到的摄像机内外参数，对空间的三维点进行重新投影计算，得到新的投影点 */
            projectPoints(tempPointSet,rightrvecsMat[i],righttvecsMat[i],rightcameraMatrix,rightdistCoeffs,image_points2);
            //            /* 计算新的投影点和旧的投影点之间的误差*/
            vector<Point2f> tempImagePoint = rightcaliimage_points_seq[i];
            Mat tempImagePointMat = Mat(1,tempImagePoint.size(),CV_32FC2);
            Mat image_points2Mat = Mat(1,image_points2.size(), CV_32FC2);
            for (int j = 0 ; j < tempImagePoint.size(); j++)
            {
                image_points2Mat.at<Vec2f>(0,j) = Vec2f(image_points2[j].x, image_points2[j].y);
                tempImagePointMat.at<Vec2f>(0,j) = Vec2f(tempImagePoint[j].x, tempImagePoint[j].y);
            }
            err = norm(image_points2Mat, tempImagePointMat, NORM_L2);
            total_err += err/=  rightpoint_counts[i];
            fout<<"the"<<i+1<<"per img avr_err："<<err<<"pix"<<endl;
        }
        fout<<"总体平均误差："<<total_err/rightimage_count<<"pix"<<endl<<endl;
        Mat rotation_matrix = Mat(3,3,CV_32FC1, Scalar::all(0)); /* 保存每幅图像的旋转矩阵 */
        fout<<"相机内参数矩阵："<<endl;
        fout<<rightcameraMatrix<<endl<<endl;
        fout<<"畸变系数：\n";
        fout<<rightdistCoeffs<<endl<<endl<<endl;
        for (int i=0; i<rightimage_count; i++)
        {
            fout<<"第"<<i+1<<"幅图像的旋转向量："<<endl;
            fout<<righttvecsMat[i]<<endl;
            /* 将旋转向量转换为相对应的旋转矩阵 */
            Rodrigues(righttvecsMat[i],rotation_matrix);
            fout<<"第"<<i+1<<"幅图像的旋转矩阵："<<endl;
            fout<<rotation_matrix<<endl;
            fout<<"第"<<i+1<<"幅图像的平移向量："<<endl;
            fout<<rightrvecsMat[i]<<endl<<endl;
        }
        fout<<endl;
        destroyWindow("rightCamera Calibration");
    }
     if(OnlyAllCam)
    {

    }

}



void Local_Algorithm::set_chessBoardSize(int width, int height)
{
    board_size.width = width;
    board_size.height = height;
}
