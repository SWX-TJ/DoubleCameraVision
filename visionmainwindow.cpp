#include "visionmainwindow.h"
#include "ui_visionmainwindow.h"

VisionMainWindow::VisionMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VisionMainWindow)
{
    ui->setupUi(this);
    m_camSet    = new CameraSetting_Dialog;
    m_imgThread = new ImageThread;
    isLoadCameraPressed = false;
    isfirstloadCameraThread = true;
    connect(m_camSet,SIGNAL(returnSignal(int)),this,SLOT(shutSlaveWindowSlot(int)));
    connect(m_camSet,SIGNAL(send_CamSetInfo(int,bool,int,bool,bool,int)),m_imgThread,SLOT(accept_CamSetInfo(int,bool,int,bool,bool,int)));
    connect(m_imgThread,SIGNAL(send_leftImageDisp(QImage)),this,SLOT(accept_leftImageDisp(QImage)),Qt::BlockingQueuedConnection);
    connect(m_imgThread,SIGNAL(send_rightImageDisp(QImage)),this,SLOT(accept_rightImageDisp(QImage)),Qt::BlockingQueuedConnection);
    connect(m_imgThread,SIGNAL(send_allImageDisp(QImage,QImage)),this,SLOT(accept_allImageDisp(QImage,QImage)),Qt::BlockingQueuedConnection);
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
    default:
        break;

    }
}

void VisionMainWindow::accept_leftImageDisp(QImage leftImage)
{
    ui->left_disp->setPixmap(QPixmap::fromImage(leftImage));
}

void VisionMainWindow::accept_rightImageDisp(QImage rightImage)
{
    ui->right_disp->setPixmap(QPixmap::fromImage(rightImage));
}

void VisionMainWindow::accept_allImageDisp(QImage leftImage, QImage rightImage)
{
    ui->left_disp->setPixmap(QPixmap::fromImage(leftImage));
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
        ui->openVisonbtn->setText(QString("视觉加载"));
    }
    else
    {
        send_ControlCamInfo(true,true,true);
        ui->openVisonbtn->setText(QString("视觉关闭"));
    }
}
