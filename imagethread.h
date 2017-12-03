#ifndef IMAGETHREAD_H
#define IMAGETHREAD_H
#include <QObject>
#include <QThread>
#include <QImage>
#include <cameradevice.h>
#include <opencv.hpp>
#include <local_algorithm.h>
#define DEFAULT_CAMERA 0
#define LEFT_CAMERA  1
#define RIGHT_CAMERA 2
#define ALL_CAMERA   3
#define Mode_CALICAM 0
#define Mode_ProcCAM 1
class ImageThread : public QThread
{
     Q_OBJECT
public:
    ImageThread();
    //public Member
public:
    Local_Algorithm *m_local_algro;
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
 /*set how use this thread*/
 /*@parm: CaliMode*/
/*        ProcMode*/
    int ControlCamSet;
    bool isCaliCam;
    int leftcalibretecamnum = 0;
    int rightcalibretecamnum = 0;

 //public class
public:
    QImage convertMatToQImage(cv::Mat &mat);
signals:
    void send_leftImageDisp(QImage);//disp left frame
    void send_rightImageDisp(QImage);//disp right frame
    void send_allImageDisp(QImage,QImage);//disp all frame
    void send_leftCaliImageDisp(QImage);//disp cali left frame
    void send_rightCaliImageDisp(QImage);//disp cali right frame
    void send_allCaliImageDisp(QImage,QImage);//disp cali all frame
    void send_isdownCalibration(bool);//show is finish calibration or not
public slots:
    //accept camera set info
    void accept_CamSetInfo(int leftCamindex = 0,bool isleftSelect = false,int rightCamindex =0,bool isrightSelect = false,bool AllSelect = false,int camMode =1);
    void accept_ControlCaminfo(bool leftCamrelea,bool rightCamrelea,bool allCamrelea);
    void accept_CloseCaminfo(bool leftCamrelea, bool rightCamrelea, bool allCamrelea);
    void accept_CaliCaminfo(bool isCaliCame,int board_width,int board_height,int per_board_width,int per_board_height);
protected:
    void run();
};

#endif // IMAGETHREAD_H
