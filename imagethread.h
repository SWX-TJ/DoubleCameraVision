#ifndef IMAGETHREAD_H
#define IMAGETHREAD_H
#include <QObject>
#include <QThread>
#include <QImage>
#include <cameradevice.h>
#include <opencv.hpp>
#include <local_algorithm.h>
#include <QMetaType>
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
    bool isneedCamCali;
    int CamSelect;
 /*set how use this thread*/
 /*@parm: CaliMode*/
/*        ProcMode*/
    int ControlCamSet;
    bool isCaliCam;
    int leftcalibretecamnum = 0;
    int rightcalibretecamnum = 0;
    int cont = 0;
 //public class
public:
    QImage convertMatToQImage(cv::Mat &mat);
signals:
   void send_isdownCalibration(bool);//show is finish calibration or not
// send_disp func v1.0
    void new_send_leftImageDisp(Mat);//disp left frame
    void new_send_rightImageDisp(Mat);//disp right frame
    void new_send_allImageDisp(Mat,Mat);//disp all frame
    void new_send_leftCaliImageDisp(Mat);//disp cali left frame
    void new_send_rightCaliImageDisp(Mat);//disp cali right frame
    void new_send_allCaliImageDisp(Mat,Mat);//disp cali all frame
public slots:
    //accept camera set info
    void accept_isneedCamcali(bool);
    void accept_CamSetInfo(int leftCamindex = 0,bool isleftSelect = false,int rightCamindex =0,bool isrightSelect = false,bool AllSelect = false,int camMode =1);
    void accept_ControlCaminfo(bool leftCamrelea,bool rightCamrelea,bool allCamrelea);
    void accept_CloseCaminfo(bool leftCamrelea, bool rightCamrelea, bool allCamrelea);
    void accept_CaliCaminfo(bool isCaliCame,int board_width,int board_height,int per_board_width,int per_board_height);
protected:
    void run();
};

#endif // IMAGETHREAD_H
