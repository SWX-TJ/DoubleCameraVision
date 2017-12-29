#include "facecollection.h"
#include "ui_facecollection.h"
#include <QMessageBox>
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
        facedabase.setFileName("facedabase.csv");
        if(facedabase.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text))
        {
            QTextStream infile(&facedabase);
            for(int i = 0;i<2048;i++)
            {
                infile<<Feature[i]<<",";
            }
            infile<<ui->NameLineEdit->text()<<endl;
            facedabase.close();
        }
        QMessageBox::information(this,QString::fromLocal8Bit("通知"),QString::fromLocal8Bit("一次采集完成"));
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
    if(isLoadCameraPressed)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("警告"),QString::fromLocal8Bit("请关闭相机再退出！"));
    }
    else
    {
        send_CloseCamInfo(true,true,true);
        m_facecollectThread->quit();
        m_facecollectThread->wait();
        returnSignal(2);
        send_ResetInfo();
    }
}

void FaceCollection::on_CameraRadBtn_clicked()
{
    isCameraCollect = !isCameraCollect;
    isPhotoCollect  = false;
    ui->SetBtn->setText(QString::fromLocal8Bit("打开相机"));
}

void FaceCollection::on_PhotoRadBtn_clicked()
{
    isCameraCollect = false;
    isPhotoCollect  = !isPhotoCollect;
    ui->SetBtn->setText(QString::fromLocal8Bit("打开照片"));
}

void FaceCollection::on_SetBtn_clicked()
{
    if(isCameraCollect)
    {

        ui->SetBtn->setText(QString::fromLocal8Bit("关闭相机"));
        isLoadCameraPressed = !isLoadCameraPressed;
        if(isLoadCameraPressed)
        {
            QMessageBox::information(this,QString::fromLocal8Bit("通知"),QString::fromLocal8Bit("相机已经打开"));
            if(isfirstloadCameraThread)
            {
                isfirstloadCameraThread = false;
                m_facecollectThread->start();

            }
            send_ControlCamInfo(false,false,false);
        }
        else
        {
            QMessageBox::information(this,QString::fromLocal8Bit("通知"),QString::fromLocal8Bit("相机已经关闭"));
            ui->SetBtn->setText(QString::fromLocal8Bit("打开相机"));
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
