#ifndef CALIBRATIONDLG_H
#define CALIBRATIONDLG_H
#include <QDialog>
#include <imagethread.h>
#include <QImage>
#include <QFile>
#include <QTextStream>
namespace Ui {
class CalibrationDlg;
}

class CalibrationDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CalibrationDlg(QWidget *parent = 0);
    ~CalibrationDlg();
signals:
    void send_returnMainWinSignal(int);
    void send_ControlCamInfo(bool,bool,bool);
    void send_CloseCamInfo(bool,bool,bool);
    void send_CalibraCamInfo(bool,int,int,int,int);
private slots:
    void on_openCamBtn_clicked();
    void on_saveBtn_clicked();
    void on_CalibraBtn_clicked();
    void on_rowCorPointNum_Let_textChanged(const QString &arg1);
    void on_colCorPointNum_Let_textChanged(const QString &arg1);
    void on_perBoardwidthEdt_textChanged(const QString &arg1);
    void on_perBoardheightEdt_textChanged(const QString &arg1);
public:
    //set a new calithread;
    ImageThread *m_calithread;
    bool isLoadCameraPressed;
    bool isfirstloadCameraThread;
    bool isfirstClikedCaliBtn;
    bool istmpboardWidthValueChanged;
    bool istmpboardHeightValueChanged;
    bool istmpperboardWidthValueChanged;
    bool istmpperboardHeightValueChanged;
    int temp_board_width;
    int temp_board_height;
    int temp_perboard_width;
    int temp_perboard_height;
    QImage leftImage;
    QImage rightImage;
public slots:
    void accept_isdownCalibration(bool);
   // accept disp func v1.0
    void new_accept_leftCaliImageDisp(Mat);
    void new_accept_rightCaliImageDisp(Mat);
    void new_accept_allCaliImageDisp(Mat,Mat);
private:
    Ui::CalibrationDlg *ui;
};

#endif // CALIBRATIONDLG_H
