#include "calibrationdlg.h"
#include "ui_calibrationdlg.h"

CalibrationDlg::CalibrationDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrationDlg)
{
    ui->setupUi(this);
    m_calithread = new ImageThread;
    isLoadCameraPressed = false;
    isfirstloadCameraThread = true;
    istmpboardWidthValueChanged  = false;
    istmpboardHeightValueChanged = false;
    istmpperboardWidthValueChanged = false;
    istmpperboardHeightValueChanged = false;
    ui->CalibraBtn->setEnabled(false);
    connect(m_calithread,SIGNAL(send_leftCaliImageDisp(QImage)),this,SLOT(accept_leftCaliImageDisp(QImage)),Qt::BlockingQueuedConnection);
    connect(m_calithread,SIGNAL(send_rightCaliImageDisp(QImage)),this,SLOT(accept_rightCaliImageDisp(QImage)),Qt::BlockingQueuedConnection);
    connect(m_calithread,SIGNAL(send_allCaliImageDisp(QImage,QImage)),this,SLOT(accept_allCaliImageDisp(QImage,QImage)),Qt::BlockingQueuedConnection);
    connect(this,SIGNAL(send_ControlCamInfo(bool,bool,bool)),m_calithread,SLOT(accept_ControlCaminfo(bool,bool,bool)));
    connect(this,SIGNAL(send_CloseCamInfo(bool,bool,bool)),m_calithread,SLOT(accept_CloseCaminfo(bool,bool,bool)));
    connect(this,SIGNAL(send_CalibraCamInfo(bool,int,int,int,int)),m_calithread,SLOT(accept_CaliCaminfo(bool,int,int,int,int)));
}

CalibrationDlg::~CalibrationDlg()
{
    delete ui;
}

void CalibrationDlg::on_openCamBtn_clicked()
{
    isLoadCameraPressed = !isLoadCameraPressed;
    if(isLoadCameraPressed)
    {
        if(isfirstloadCameraThread)
        {
            isfirstloadCameraThread = false;
            m_calithread->start();

        }
        send_ControlCamInfo(false,false,false);
        ui->openCamBtn->setText(QString("视觉加载"));
    }
    else
    {
        send_ControlCamInfo(true,true,true);
        ui->openCamBtn->setText(QString("视觉关闭"));
    }
}

void CalibrationDlg::on_saveBtn_clicked()
{
    send_CloseCamInfo(true,true,true);
    m_calithread->quit();
    m_calithread->wait();
    send_returnMainWinSignal(1);
}

void CalibrationDlg::accept_leftCaliImageDisp(QImage leftImg)
{
    ui->leftImage_disp->setPixmap(QPixmap::fromImage(leftImg));
}

void CalibrationDlg::accept_rightCaliImageDisp(QImage rightImg)
{
    ui->rightImage_disp->setPixmap(QPixmap::fromImage(rightImg));
}

void CalibrationDlg::accept_allCaliImageDisp(QImage leftImg, QImage rightImg)
{
    ui->leftImage_disp->setPixmap(QPixmap::fromImage(leftImg));
    ui->rightImage_disp->setPixmap(QPixmap::fromImage(rightImg));
}

void CalibrationDlg::on_CalibraBtn_clicked()
{
    if(istmpboardHeightValueChanged&&istmpboardWidthValueChanged&&istmpperboardWidthValueChanged&&istmpperboardHeightValueChanged)
        send_CalibraCamInfo(true,temp_board_width,temp_board_height,temp_perboard_width,temp_perboard_height);
}

void CalibrationDlg::on_rowCorPointNum_Let_textChanged(const QString &arg1)
{
    temp_board_width = arg1.toInt();
    istmpboardWidthValueChanged = true;
    if(istmpboardHeightValueChanged&&istmpboardWidthValueChanged&&istmpperboardWidthValueChanged&&istmpperboardHeightValueChanged)
    {
        ui->CalibraBtn->setEnabled(true);
    }
}

void CalibrationDlg::on_colCorPointNum_Let_textChanged(const QString &arg1)
{
    temp_board_height = arg1.toInt();
    istmpboardHeightValueChanged = true;
    if(istmpboardHeightValueChanged&&istmpboardWidthValueChanged&&istmpperboardWidthValueChanged&&istmpperboardHeightValueChanged)
    {
        ui->CalibraBtn->setEnabled(true);
    }
}


void CalibrationDlg::on_perBoardwidthEdt_textChanged(const QString &arg1)
{
    temp_perboard_width = arg1.toInt();
    istmpperboardWidthValueChanged = true;
    if(istmpboardHeightValueChanged&&istmpboardWidthValueChanged&&istmpperboardWidthValueChanged&&istmpperboardHeightValueChanged)
    {
        ui->CalibraBtn->setEnabled(true);
    }
}

void CalibrationDlg::on_perBoardheightEdt_textChanged(const QString &arg1)
{
    temp_perboard_height = arg1.toInt();
    istmpperboardHeightValueChanged = true;
    if(istmpboardHeightValueChanged&&istmpboardWidthValueChanged&&istmpperboardWidthValueChanged&&istmpperboardHeightValueChanged)
    {
        ui->CalibraBtn->setEnabled(true);
    }

}