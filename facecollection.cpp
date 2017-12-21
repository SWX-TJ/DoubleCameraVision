#include "facecollection.h"
#include "ui_facecollection.h"
#include <QDebug>
FaceCollection::FaceCollection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FaceCollection)
{
    ui->setupUi(this);
    isCameraCollect = false;
    isPhotoCollect  = false;
    isLoadCameraPressed = false;
    isFaceCollection = false;
    isfirstloadCameraThread = true;
    m_facecollectThread = new ImageThread;
    m_algorim = new Local_Algorithm;
    connect(m_facecollectThread,SIGNAL(new_send_leftImageDisp(Mat)),this,SLOT(new_accept_leftImageDisp(Mat)));
    connect(this,SIGNAL(send_ControlCamInfo(bool,bool,bool)),m_facecollectThread,SLOT(accept_ControlCaminfo(bool,bool,bool)));
    connect(this,SIGNAL(send_CloseCamInfo(bool,bool,bool)),m_facecollectThread,SLOT(accept_CloseCaminfo(bool,bool,bool)));
}

FaceCollection::~FaceCollection()
{
    delete ui;
}

void FaceCollection::new_accept_leftImageDisp(Mat leftMatImage)
{
    if(isFaceCollection)
    {
        isFaceCollection = false;
        Mat faceCollectionImage;
        float Feature[2048];
        m_algorim->FaceModule_FacePreTrain(leftMatImage,faceCollectionImage,Feature);
        leftImage = m_facecollectThread->convertMatToQImage(faceCollectionImage);
        ui->DispLabel->setPixmap(QPixmap::fromImage(leftImage));
    }
    else
    {
        leftImage = m_facecollectThread->convertMatToQImage(leftMatImage);
        ui->DispLabel->setPixmap(QPixmap::fromImage(leftImage));
    }

}


void FaceCollection::on_pushButton_clicked()
{
    send_CloseCamInfo(true,true,true);
    m_facecollectThread->quit();
    m_facecollectThread->wait();
    returnSignal(2);
}

void FaceCollection::on_CameraRadBtn_clicked()
{
    isCameraCollect = !isCameraCollect;
    isPhotoCollect  = false;
}

void FaceCollection::on_PhotoRadBtn_clicked()
{
    isCameraCollect = false;
    isPhotoCollect  = !isPhotoCollect;
}

void FaceCollection::on_SetBtn_clicked()
{
    if(isCameraCollect)
    {
        isLoadCameraPressed = !isLoadCameraPressed;
        if(isLoadCameraPressed)
        {
            if(isfirstloadCameraThread)
            {
                isfirstloadCameraThread = false;
                m_facecollectThread->start();

            }
            send_ControlCamInfo(false,false,false);
        }
        else
        {
            send_ControlCamInfo(true,true,true);
        }
    }
    else if(isPhotoCollect)
    {

    }
}

void FaceCollection::on_CollectPhotoBtn_clicked()
{
    isFaceCollection = true;
}
