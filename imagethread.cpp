#include "imagethread.h"
#include <QDebug>
ImageThread::ImageThread()
{
    m_leftCamDev  = new CameraDevice;
    m_rightCamDev = new CameraDevice;
    CamSelect     = DEFAULT_CAMERA;
    isOpenLeftCam   =  false;
    isOpenRightCam  =  false;
    isOpenAllCam    =  false;
    iscloseLeftCam  =  false;
    iscloseRightCam = false;
    iscloseAllCam   =  false;
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

void ImageThread::accept_CamSetInfo(int leftCamindex, bool isleftSelect, int rightCamindex, bool isrightSelect, bool AllSelect)
{
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

void ImageThread::run()
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
                cv::Mat templeftframe;
                cv::resize(oriLeftFrame,templeftframe,cv::Size(320,240));
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
                cv::Mat temprightframe;
                cv::resize(oriRightFrame,temprightframe,cv::Size(320,240));
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
                cv::Mat templeftframe;
                cv::resize(oriLeftFrame,templeftframe,cv::Size(320,240));
                cv::Mat temprightframe;
                cv::resize(oriRightFrame,temprightframe,cv::Size(320,240));
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