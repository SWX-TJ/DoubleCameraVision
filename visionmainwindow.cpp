#include "visionmainwindow.h"
#include "ui_visionmainwindow.h"
#include <QMessageBox>
#include <QDebug>
VisionMainWindow::VisionMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VisionMainWindow)
{
    ui->setupUi(this);
    m_camSet    = new CameraSetting_Dialog;
    m_imgThread = new ImageThread;
    m_facecollect = new FaceCollection;
    isLoadCameraPressed = false;
    isprocFaceDetetion  = false;
    isfirstloadCameraThread = true;
    qRegisterMetaType<Mat>("Mat");
    m_local = new Local_Algorithm;
    facedetect_action = new QAction(QString::fromLocal8Bit("人脸检测"),this);
    reset_action      = new QAction(QString::fromLocal8Bit("重置"),this);
    addAction(facedetect_action);
    addAction(reset_action);
    connect(facedetect_action,SIGNAL(triggered(bool)),this,SLOT(process_facedetectionaction(bool)));
    setContextMenuPolicy(Qt::ActionsContextMenu);
    connect(m_facecollect,SIGNAL(returnSignal(int)),this,SLOT(shutSlaveWindowSlot(int)));
    connect(m_camSet,SIGNAL(send_CamSetInfo(int,bool,int,bool,bool,int)),m_facecollect->m_facecollectThread,SLOT(accept_CamSetInfo(int,bool,int,bool,bool,int)));
    connect(m_facecollect,SIGNAL(send_ResetInfo()),m_camSet,SLOT(accept_returnResetSignals()));
    connect(m_camSet,SIGNAL(returnSignal(int)),this,SLOT(shutSlaveWindowSlot(int)));
    connect(m_camSet,SIGNAL(send_CamSetInfo(int,bool,int,bool,bool,int)),m_imgThread,SLOT(accept_CamSetInfo(int,bool,int,bool,bool,int)));
    connect(m_imgThread,SIGNAL(new_send_leftImageDisp(Mat)),this,SLOT(new_accept_leftImageDisp(Mat)),Qt::BlockingQueuedConnection);
    connect(m_imgThread,SIGNAL(new_send_rightImageDisp(Mat)),this,SLOT(new_accept_rightImageDisp(Mat)),Qt::BlockingQueuedConnection);
    connect(m_imgThread,SIGNAL(new_send_allImageDisp(Mat,Mat)),this,SLOT(new_accept_allImageDisp(Mat,Mat)),Qt::BlockingQueuedConnection);
    connect(this,SIGNAL(send_ControlCamInfo(bool,bool,bool)),m_imgThread,SLOT(accept_ControlCaminfo(bool,bool,bool)));
    connect(this,SIGNAL(send_CloseCamInfo(bool,bool,bool)),m_imgThread,SLOT(accept_CloseCaminfo(bool,bool,bool)));
    connect(this,SIGNAL(send_isneeedCamCali(bool)),m_imgThread,SLOT(accept_isneedCamcali(bool)));
}

VisionMainWindow::~VisionMainWindow()
{
    delete ui;
}

void VisionMainWindow::shutSlaveWindowSlot(int num)
{
    switch(num)
    {
    //camsetDialog close operation
    case 1:
        m_camSet->close();
        break;
    case 2:
        m_facecollect->close();
    default:
        break;

    }
}


void VisionMainWindow::process_facedetectionaction(bool arg)
{
    arg = arg;
    isprocFaceDetetion = !isprocFaceDetetion;
}

void VisionMainWindow::new_accept_leftImageDisp(Mat leftMatImage)
{
    if(isprocFaceDetetion)
    {
        Mat templeftFaceImage;
        templeftFaceImage = m_local->faceDetectionFunc(leftMatImage);
        leftImage = m_imgThread->convertMatToQImage(templeftFaceImage);
        ui->left_disp->setPixmap(QPixmap::fromImage(leftImage));
    }
    else
    {
        leftImage = m_imgThread->convertMatToQImage(leftMatImage);
        ui->left_disp->setPixmap(QPixmap::fromImage(leftImage));
    }

}

void VisionMainWindow::new_accept_rightImageDisp(Mat rightMatImage)
{
    if(isprocFaceDetetion)
    {
        Mat temprightFaceImage;
        temprightFaceImage = m_local->faceDetectionFunc(rightMatImage);
        rightImage = m_imgThread->convertMatToQImage(temprightFaceImage);
        ui->right_disp->setPixmap(QPixmap::fromImage(rightImage));
    }
    else
    {
        rightImage = m_imgThread->convertMatToQImage(rightMatImage);
        ui->right_disp->setPixmap(QPixmap::fromImage(rightImage));
    }
}

void VisionMainWindow::new_accept_allImageDisp(Mat leftMatImage, Mat rightMatImage)
{
    if(isprocFaceDetetion)
    {
        Mat templeftFaceImage;
        templeftFaceImage = m_local->faceDetectionFunc(leftMatImage);
        leftImage = m_imgThread->convertMatToQImage(templeftFaceImage);
        Mat temprightFaceImage;
        temprightFaceImage = m_local->faceDetectionFunc(rightMatImage);
        rightImage = m_imgThread->convertMatToQImage(temprightFaceImage);
        ui->left_disp->setPixmap(QPixmap::fromImage(leftImage));
        ui->right_disp->setPixmap(QPixmap::fromImage(rightImage));
    }
    else
    {
        leftImage = m_imgThread->convertMatToQImage(leftMatImage);
        ui->left_disp->setPixmap(QPixmap::fromImage(leftImage));
        rightImage = m_imgThread->convertMatToQImage(rightMatImage);
        ui->right_disp->setPixmap(QPixmap::fromImage(rightImage));
    }
}

void VisionMainWindow::on_actionCamDevSet_triggered()
{
    m_camSet->show();
}

void VisionMainWindow::on_ExitBtn_clicked()
{
    send_CloseCamInfo(true,true,true);
    m_imgThread->quit();
    m_imgThread->wait();
    exit(EXIT_SUCCESS);
}

void VisionMainWindow::on_openVisonbtn_clicked()
{
    isLoadCameraPressed = !isLoadCameraPressed;
    if(isLoadCameraPressed)
    {
        if(isfirstloadCameraThread)
        {
            isfirstloadCameraThread = false;
            QMessageBox::StandardButton rb = QMessageBox::question(this,QString::fromLocal8Bit("通知"),QString::fromLocal8Bit("您的相机是否已经标定，电脑摄像头，网络免驱摄像头或者手机摄像头一般已经标记过，请选择yes,其他选择no"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
            if(rb == QMessageBox::No)
            {
                send_isneeedCamCali(true);
            }
            else
            {
                 send_isneeedCamCali(false);
            }
            m_imgThread->start();
        }
        m_local->public_getfacedateBase();
        send_ControlCamInfo(false,false,false);
        ui->openVisonbtn->setText(QString::fromLocal8Bit("视觉关闭"));
        QMessageBox::information(this,QString::fromLocal8Bit("通知"),QString::fromLocal8Bit("摄像头已经打开"));
    }
    else
    {
        send_ControlCamInfo(true,true,true);
        QMessageBox::information(this,QString::fromLocal8Bit("通知"),QString::fromLocal8Bit("摄像头已经关闭"));
        ui->openVisonbtn->setText(QString::fromLocal8Bit("视觉开启"));
    }
}

void VisionMainWindow::on_actionFaceCollection_triggered()
{
    m_facecollect->show();
}

void VisionMainWindow::on_LoadDllBtn_clicked()
{
}
