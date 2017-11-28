#ifndef IMAGETHREAD_H
#define IMAGETHREAD_H
#include <QObject>
#include <QThread>
#include <QImage>
#include <cameradevice.h>
#include <opencv.hpp>
#define DEFAULT_CAMERA 0
#define LEFT_CAMERA  1
#define RIGHT_CAMERA 2
#define ALL_CAMERA   3
class ImageThread : public QThread
{
     Q_OBJECT
public:
    ImageThread();
    //public Member
public:
    CameraDevice *m_leftCamDev;
    CameraDevice *m_rightCamDev;
    cv::VideoCapture *m_leftCam;
    cv::VideoCapture *m_rightCam;
    cv::Mat oriLeftFrame;
    cv::Mat oriRightFrame;
    QImage dispLeftFrame;
    QImage dispRightFrame;
    bool isOpenLeftCam;
    bool isOpenRightCam;
    bool isOpenAllCam;
    bool iscloseLeftCam;
    bool iscloseRightCam;
    bool iscloseAllCam;
    int CamSelect;
 //public class
public:
    QImage convertMatToQImage(cv::Mat &mat);
signals:
    void send_leftImageDisp(QImage);//disp left frame
    void send_rightImageDisp(QImage);//disp right frame
    void send_allImageDisp(QImage,QImage);//disp all frame
public slots:
    //accept camera set info
    void accept_CamSetInfo(int leftCamindex = 0,bool isleftSelect = false,int rightCamindex =0,bool isrightSelect = false,bool AllSelect = false);
    void accept_ControlCaminfo(bool leftCamrelea,bool rightCamrelea,bool allCamrelea);
    void accept_CloseCaminfo(bool leftCamrelea, bool rightCamrelea, bool allCamrelea);
protected:
    void run();
};

#endif // IMAGETHREAD_H
