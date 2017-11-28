#include "camerasetting_dialog.h"
#include "ui_camerasetting_dialog.h"
#include <QDebug>
#include <QMessageBox>
CameraSetting_Dialog::CameraSetting_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraSetting_Dialog)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("摄像头设置"));
    this->setWindowFlag(Qt::WindowContextHelpButtonHint,false);
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
}

CameraSetting_Dialog::~CameraSetting_Dialog()
{
    delete ui;
}

void CameraSetting_Dialog::on_SettingBtn_clicked()
{
    switch (CameraUseNumFlag) {
    case 1:
        if(leftCameraEnable)
        {
            send_CamSetInfo(leftCameraIndex,leftCameraEnable,rightCameraIndex,rightCameraEnable,false);
        }
        else if(rightCameraEnable)
        {
            send_CamSetInfo(leftCameraIndex,leftCameraEnable,rightCameraIndex,rightCameraEnable,false);
        }
        leftCameraEnable = false;
        rightCameraEnable = false;
        break;
    case 2:
        send_CamSetInfo(leftCameraIndex,leftCameraEnable,rightCameraIndex,rightCameraEnable,true);
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
        CameraUseNumFlag--;
    }
}

