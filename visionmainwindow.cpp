#include "visionmainwindow.h"
#include "ui_visionmainwindow.h"

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
    facedetect_action = new QAction(QString::fromLocal8Bit("人脸检测"),this);
    reset_action      = new QAction(QString::fromLocal8Bit("重置"),this);
    addAction(facedetect_action);
    addAction(reset_action);
    connect(facedetect_action,SIGNAL(triggered(bool)),this,SLOT(process_facedetectionaction(bool)));
    setContextMenuPolicy(Qt::ActionsContextMenu);
    connect(m_facecollect,SIGNAL(returnSignal(int)),this,SLOT(shutSlaveWindowSlot(int)));
    connect(m_camSet,SIGNAL(send_CamSetInfo(int,bool,int,bool,bool,int)),m_facecollect->m_facecollectThread,SLOT(accept_CamSetInfo(int,bool,int,bool,bool,int)));
    connect(m_camSet,SIGNAL(returnSignal(int)),this,SLOT(shutSlaveWindowSlot(int)));
    connect(m_camSet,SIGNAL(send_CamSetInfo(int,bool,int,bool,bool,int)),m_imgThread,SLOT(accept_CamSetInfo(int,bool,int,bool,bool,int)));
    connect(m_imgThread,SIGNAL(new_send_leftImageDisp(Mat)),this,SLOT(new_accept_leftImageDisp(Mat)),Qt::BlockingQueuedConnection);
    connect(m_imgThread,SIGNAL(new_send_rightImageDisp(Mat)),this,SLOT(new_accept_rightImageDisp(Mat)),Qt::BlockingQueuedConnection);
    connect(m_imgThread,SIGNAL(new_send_allImageDisp(Mat,Mat)),this,SLOT(new_accept_allImageDisp(Mat,Mat)),Qt::BlockingQueuedConnection);
    connect(this,SIGNAL(send_ControlCamInfo(bool,bool,bool)),m_imgThread,SLOT(accept_ControlCaminfo(bool,bool,bool)));
    connect(this,SIGNAL(send_CloseCamInfo(bool,bool,bool)),m_imgThread,SLOT(accept_CloseCaminfo(bool,bool,bool)));
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
 leftImage = m_imgThread->convertMatToQImage(leftMatImage);
 ui->left_disp->setPixmap(QPixmap::fromImage(leftImage));
}

void VisionMainWindow::new_accept_rightImageDisp(Mat rightMatImage)
{
   rightImage = m_imgThread->convertMatToQImage(rightMatImage);
   ui->right_disp->setPixmap(QPixmap::fromImage(rightImage));
}

void VisionMainWindow::new_accept_allImageDisp(Mat leftMatImage, Mat rightMatImage)
{
    leftImage = m_imgThread->convertMatToQImage(leftMatImage);
    ui->left_disp->setPixmap(QPixmap::fromImage(leftImage));
    rightImage = m_imgThread->convertMatToQImage(rightMatImage);
    ui->right_disp->setPixmap(QPixmap::fromImage(rightImage));
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
            m_imgThread->start();

        }
        send_ControlCamInfo(false,false,false);
        ui->openVisonbtn->setText(QString::fromLocal8Bit("视觉加载"));
    }
    else
    {
        send_ControlCamInfo(true,true,true);
        ui->openVisonbtn->setText(QString::fromLocal8Bit("视觉关闭"));
    }
}

void VisionMainWindow::on_actionFaceCollection_triggered()
{
    m_facecollect->show();
}
