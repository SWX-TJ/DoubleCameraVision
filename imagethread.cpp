#include "imagethread.h"
#include <QDebug>
ImageThread::ImageThread()
{
    m_leftCamDev  = new CameraDevice;
    m_rightCamDev = new CameraDevice;
    m_local_algro = new Local_Algorithm;
    m_local_algro->returnLeftCam();
    m_local_algro->returnRightCam();
    CamSelect     = DEFAULT_CAMERA;
    isOpenLeftCam   =  false;
    isOpenRightCam  =  false;
    isOpenAllCam    =  false;
    iscloseLeftCam  =  false;
    iscloseRightCam =  false;
    iscloseAllCam   =  false;
    isCaliCam       =  false;
    ControlCamSet = Mode_ProcCAM;
}

QImage ImageThread::convertMatToQImage(cv::Mat &mat)
{
    QImage img;
    int nChannel=mat.channels();
    if(nChannel==3)
    {
        cv::cvtColor(mat,mat,CV_BGR2RGB);
        img = QImage((const unsigned char*)mat.data,mat.cols,mat.rows,QImage::Format_RGB888);
    }
    else if(nChannel==4)
    {
        img = QImage((const unsigned char*)mat.data,mat.cols,mat.rows,QImage::Format_ARGB32);
    }
    else if(nChannel==1)
    {
        img = QImage((const unsigned char*)mat.data,mat.cols,mat.rows,QImage::Format_Indexed8);
    }

    return img;
}

void ImageThread::accept_CamSetInfo(int leftCamindex, bool isleftSelect, int rightCamindex, bool isrightSelect, bool AllSelect,int camMode)
{
    ControlCamSet   = camMode;
    if(AllSelect)
    {
        CamSelect = ALL_CAMERA;
        m_leftCamDev->setCamName(leftCamindex);
        m_rightCamDev->setCamName(rightCamindex);
    }
    else
    {
        if(isleftSelect)
        {
            CamSelect = LEFT_CAMERA;
            m_leftCamDev->setCamName(leftCamindex);
        }
        if(isrightSelect)
        {
            CamSelect = RIGHT_CAMERA;
            m_rightCamDev->setCamName(rightCamindex);
        }
    }
}

void ImageThread::accept_ControlCaminfo(bool leftCamrelea, bool rightCamrelea, bool allCamrelea)
{
    isOpenLeftCam  = leftCamrelea;
    isOpenRightCam = rightCamrelea;
    isOpenAllCam   = allCamrelea;
}

void ImageThread::accept_CloseCaminfo(bool leftCamrelea, bool rightCamrelea, bool allCamrelea)
{
    iscloseLeftCam  = leftCamrelea;
    iscloseRightCam = rightCamrelea;
    iscloseAllCam   = allCamrelea;

}

void ImageThread::accept_CaliCaminfo(bool isCaliCame,int board_width,int board_height,int per_board_width,int per_board_height)
{
    isCaliCam = isCaliCame;
    m_local_algro->board_size.width = board_width;
    m_local_algro->board_size.height = board_height;
    m_local_algro->real_square_size.width = per_board_width;
    m_local_algro->real_square_size.height = per_board_height;
}



void ImageThread::run()
{
    switch(ControlCamSet)
    {
    case Mode_ProcCAM:
    {
        switch (CamSelect) {
        case LEFT_CAMERA:
            m_leftCam = new cv::VideoCapture(m_leftCamDev->returnCamdevIndex());
            while(1)
            {
                if(isOpenLeftCam)
                {
                    if(iscloseLeftCam)
                    {
                        m_leftCam->release();
                        break;
                    }
                }
                else
                {
                    m_leftCam->operator >>(oriLeftFrame);
                    cv::Mat leftcorrectImage;
                    undistort(oriLeftFrame,leftcorrectImage,m_local_algro->leftcameraMatrix,m_local_algro->leftdistCoeffs);

                    cv::Mat templeftframe;
                    cv::resize(leftcorrectImage,templeftframe,cv::Size(320,240));
                    dispLeftFrame = convertMatToQImage(templeftframe);
                    send_leftImageDisp(dispLeftFrame);
                }
            }
            break;
        case RIGHT_CAMERA:
            m_rightCam    = new cv::VideoCapture(m_rightCamDev->returnCamdevIndex());
            while(1)
            {
                if(isOpenRightCam)
                {
                    if(iscloseRightCam)
                    {
                        m_rightCam->release();
                        break;
                    }
                }
                else
                {
                    m_rightCam->operator >>(oriRightFrame);
                    cv::Mat rightcorrectImage;
                    undistort(oriRightFrame,rightcorrectImage,m_local_algro->rightcameraMatrix,m_local_algro->rightdistCoeffs);
                    cv::Mat temprightframe;
                    cv::resize(rightcorrectImage,temprightframe,cv::Size(320,240));
                    dispRightFrame = convertMatToQImage(temprightframe);
                    send_rightImageDisp(dispRightFrame);
                }
            }
            break;
        case ALL_CAMERA:
            m_leftCam  = new cv::VideoCapture(m_leftCamDev->returnCamdevIndex());
            m_rightCam = new cv::VideoCapture(m_rightCamDev->returnCamdevIndex());
            while(1)
            {
                if(isOpenAllCam)
                {

                    if(iscloseAllCam)
                    {
                        m_leftCam->release();
                        m_rightCam->release();
                        break;
                    }
                }
                else
                {
                    m_leftCam->operator >>(oriLeftFrame);
                    m_rightCam->operator >>(oriRightFrame);
                    cv::Mat leftcorrectImage;
                    undistort(oriLeftFrame,leftcorrectImage,m_local_algro->leftcameraMatrix,m_local_algro->leftdistCoeffs);
                    cv::Mat templeftframe;
                    cv::resize(leftcorrectImage,templeftframe,cv::Size(320,240));
                    cv::Mat rightcorrectImage;
                    undistort(oriRightFrame,rightcorrectImage,m_local_algro->rightcameraMatrix,m_local_algro->rightdistCoeffs);

                    cv::Mat temprightframe;
                    cv::resize(rightcorrectImage,temprightframe,cv::Size(320,240));
                    dispLeftFrame = convertMatToQImage(templeftframe);
                    dispRightFrame = convertMatToQImage(temprightframe);
                    send_allImageDisp(dispLeftFrame,dispRightFrame);
                }
            }
            break;
        default:
            break;
        }
    }
        break;
    case Mode_CALICAM:
    {
        switch (CamSelect) {
        case LEFT_CAMERA:
            m_leftCam = new cv::VideoCapture(m_leftCamDev->returnCamdevIndex());
            while(1)
            {
                if(isOpenLeftCam)
                {
                    if(iscloseLeftCam)
                    {
                        m_leftCam->release();
                        break;
                    }
                }
                else
                {
                    if(isCaliCam)
                    {
                        isCaliCam = false;
                        m_local_algro->m_LeftCaliPrePoc(*m_leftCam);
                        m_local_algro->m_CalibrateCamera(true,false,false);
                    }
                    else
                    {
                        m_leftCam->operator >>(oriLeftFrame);
                        if(m_local_algro->leftcameraMatrix.at<int>(0,0)==0)
                        {
                            cv::Mat templeftframe;
                            cv::resize(oriLeftFrame,templeftframe,cv::Size(320,240));
                            dispLeftFrame = convertMatToQImage(templeftframe);
                            send_leftCaliImageDisp(dispLeftFrame);
                        }
                        else
                        {
                            cv::Mat correctImage;
                            undistort(oriLeftFrame,correctImage,m_local_algro->leftcameraMatrix,m_local_algro->leftdistCoeffs);
                            cv::Mat templeftframe;
                            cv::resize(correctImage,templeftframe,cv::Size(320,240));
                            dispLeftFrame = convertMatToQImage(templeftframe);
                            send_leftCaliImageDisp(dispLeftFrame);
                        }
                    }
                }
            }
            break;
        case RIGHT_CAMERA:
            m_rightCam    = new cv::VideoCapture(m_rightCamDev->returnCamdevIndex());
            while(1)
            {
                if(isOpenRightCam)
                {
                    if(iscloseRightCam)
                    {
                        m_rightCam->release();
                        break;
                    }
                }
                else
                {
                    if(isCaliCam)
                    {
                        isCaliCam = false;
                        m_local_algro->m_RightCaliPrePoc(*m_rightCam);
                        m_local_algro->m_CalibrateCamera(false,true,false);
                    }
                    else
                    {
                        m_rightCam->operator >>(oriRightFrame);
                        if(m_local_algro->rightcameraMatrix.at<int>(0,0)==0)
                        {
                            cv::Mat temprightframe;
                            cv::resize(oriRightFrame,temprightframe,cv::Size(320,240));
                            dispRightFrame = convertMatToQImage(temprightframe);
                            send_rightCaliImageDisp(dispRightFrame);
                        }
                        cv::Mat correctImage;
                        undistort(oriRightFrame,correctImage,m_local_algro->rightcameraMatrix,m_local_algro->rightdistCoeffs);
                        cv::Mat temprightframe;
                        cv::resize(correctImage,temprightframe,cv::Size(320,240));
                        dispRightFrame = convertMatToQImage(temprightframe);
                        send_rightCaliImageDisp(dispRightFrame);
                    }
                }
            }
            break;
        case ALL_CAMERA:
            m_leftCam  = new cv::VideoCapture(m_leftCamDev->returnCamdevIndex());
            m_rightCam = new cv::VideoCapture(m_rightCamDev->returnCamdevIndex());
            while(1)
            {
                if(isOpenAllCam)
                {

                    if(iscloseAllCam)
                    {
                        m_leftCam->release();
                        m_rightCam->release();
                        break;
                    }
                }
                else
                {
                    if(isCaliCam)
                    {
                        isCaliCam = false;
                        m_local_algro->m_AllCaliPrePoc(*m_leftCam,*m_rightCam);
                        m_local_algro->m_CalibrateCamera(true,true,false);
                    }
                    {
                        m_leftCam->operator >>(oriLeftFrame);
                        m_rightCam->operator >>(oriRightFrame);
                        if(m_local_algro->leftcameraMatrix.at<int>(0,0) ==0&&m_local_algro->rightcameraMatrix.at<int>(0,0) ==0)
                        {
                            cv::Mat templeftframe;
                            cv::resize(oriLeftFrame,templeftframe,cv::Size(320,240));
                            cv::Mat temprightframe;
                            cv::resize(oriRightFrame,temprightframe,cv::Size(320,240));
                            dispLeftFrame = convertMatToQImage(templeftframe);
                            dispRightFrame = convertMatToQImage(temprightframe);
                            send_allCaliImageDisp(dispLeftFrame,dispRightFrame);
                        }
                        cv::Mat leftcorrectImage;
                        undistort(oriLeftFrame,leftcorrectImage,m_local_algro->leftcameraMatrix,m_local_algro->leftdistCoeffs);
                        cv::Mat templeftframe;
                        cv::resize(leftcorrectImage,templeftframe,cv::Size(320,240));
                        cv::Mat rightcorrectImage;
                        undistort(oriRightFrame,rightcorrectImage,m_local_algro->rightcameraMatrix,m_local_algro->rightdistCoeffs);
                        cv::Mat temprightframe;
                        cv::resize(rightcorrectImage,temprightframe,cv::Size(320,240));
                        dispLeftFrame = convertMatToQImage(templeftframe);
                        dispRightFrame = convertMatToQImage(temprightframe);
                        send_allCaliImageDisp(dispLeftFrame,dispRightFrame);
                    }
                }
            }
            break;
        default:
            break;
        }
    }
        break;
    }
}
