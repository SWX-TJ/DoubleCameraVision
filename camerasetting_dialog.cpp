#include "camerasetting_dialog.h"
#include "ui_camerasetting_dialog.h"
#include <QDebug>
#include <QMessageBox>
CameraSetting_Dialog::CameraSetting_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraSetting_Dialog)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("摄像头设置"));
    this->setWindowFlag(Qt::WindowContextHelpButtonHint,false);
    m_calibra = new CalibrationDlg;
    device_num = 0;
    CameraUseNumFlag = 0;
    leftCameraEnable = false;
    rightCameraEnable = false;
    QString temp_device_num;
    m_camerainfo = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &Camera_device, m_camerainfo) {
        if(Camera_device.deviceName()!=temp_deviceName)
        {
            ui->LeftCamera_DeviceList->addItem(QString("device")+(temp_device_num.setNum(device_num)));
            ui->Right_CameraDeviceList->addItem(QString("device")+(temp_device_num.setNum(device_num)));
            device_num++;
            temp_deviceName = Camera_device.deviceName();
        }
    }
    connect(m_calibra,SIGNAL(send_returnMainWinSignal(int)),this,SLOT(accept_returnSlaveWindowsignal(int)));
    connect(this,SIGNAL(send_CamSetInfo(int,bool,int,bool,bool,int)),m_calibra->m_calithread,SLOT(accept_CamSetInfo(int,bool,int,bool,bool,int)));
}

CameraSetting_Dialog::~CameraSetting_Dialog()
{
    delete ui;
}

void CameraSetting_Dialog::accept_returnSlaveWindowsignal(int WinNum)
{
    if(WinNum==1)
    {
        m_calibra->close();
        device_num = 0;
        CameraUseNumFlag = 0;
        leftCameraEnable = false;
        rightCameraEnable = false;
        if(ui->leftEnable_btn->isChecked())
        {
            ui->leftEnable_btn->setChecked(false);
        }
        if(ui->rightEnable_btn->isChecked())
        {
            ui->rightEnable_btn->setChecked(false);
        }
    }
}

void CameraSetting_Dialog::on_SettingBtn_clicked()
{
    switch (CameraUseNumFlag) {
    case 1:
        if(leftCameraEnable)
        {
            send_CamSetInfo(leftCameraIndex,leftCameraEnable,rightCameraIndex,rightCameraEnable,false,1);
        }
        else if(rightCameraEnable)
        {
            send_CamSetInfo(leftCameraIndex,leftCameraEnable,rightCameraIndex,rightCameraEnable,false,1);
        }
        leftCameraEnable = false;
        rightCameraEnable = false;
        break;
    case 2:
        send_CamSetInfo(leftCameraIndex,leftCameraEnable,rightCameraIndex,rightCameraEnable,true,1);
        break;
    default:
        QMessageBox::information(this,"Setting ERROR","there is no device");
        break;
    }

}

void CameraSetting_Dialog::on_ReturnBtn_clicked()
{
    returnSignal(1);
}


void CameraSetting_Dialog::on_leftEnable_btn_clicked()
{

    if(!leftCameraEnable)
    {
        leftCameraEnable = true;
        leftCameraIndex = ui->LeftCamera_DeviceList->currentIndex();
        CameraUseNumFlag ++;

    }
    else
    {
        leftCameraEnable = false;
        leftCameraIndex = ui->LeftCamera_DeviceList->currentIndex();
        CameraUseNumFlag --;
    }
}

void CameraSetting_Dialog::on_rightEnable_btn_clicked()
{
    if(!rightCameraEnable)
    {
        rightCameraEnable = true;
        rightCameraIndex = ui->Right_CameraDeviceList->currentIndex();
        CameraUseNumFlag ++;
    }
    else
    {
        rightCameraEnable = false;
        rightCameraIndex = ui->Right_CameraDeviceList->currentIndex();
        CameraUseNumFlag--;
    }
}


void CameraSetting_Dialog::on_CalibrationSingleCamera_clicked()
{
    switch (CameraUseNumFlag) {
    case 1:
        if(leftCameraEnable)
        {
            send_CamSetInfo(leftCameraIndex,leftCameraEnable,rightCameraIndex,rightCameraEnable,false,0);
        }
        else if(rightCameraEnable)
        {
            send_CamSetInfo(leftCameraIndex,leftCameraEnable,rightCameraIndex,rightCameraEnable,false,0);
        }
        leftCameraEnable = false;
        rightCameraEnable = false;
        break;
    case 2:
        send_CamSetInfo(leftCameraIndex,leftCameraEnable,rightCameraIndex,rightCameraEnable,true,0);
        break;
    default:
        QMessageBox::information(this,"Setting ERROR","there is no device");
        break;
    }

    m_calibra->show();
}

void CameraSetting_Dialog::on_updateDevBtn_clicked()
{
    for(int i = 0;i<device_num;i++)
    {
        ui->LeftCamera_DeviceList->removeItem(i);
        ui->Right_CameraDeviceList->removeItem(i);
    }
    device_num = 0;
    QString temp_device_num;
    m_camerainfo = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &Camera_device, m_camerainfo) {
        if(Camera_device.deviceName()!=temp_deviceName)
        {
            ui->LeftCamera_DeviceList->addItem(QString("device")+(temp_device_num.setNum(device_num)));
            ui->Right_CameraDeviceList->addItem(QString("device")+(temp_device_num.setNum(device_num)));
            device_num++;
            temp_deviceName = Camera_device.deviceName();
        }
    }
}
