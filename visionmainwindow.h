#ifndef VISIONMAINWINDOW_H
#define VISIONMAINWINDOW_H
#include <QMainWindow>
#include "camerasetting_dialog.h"
#include <imagethread.h>
namespace Ui {
class VisionMainWindow;
}

class VisionMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit VisionMainWindow(QWidget *parent = 0);
    ~VisionMainWindow();
 public slots:
    void shutSlaveWindowSlot(int);
   //Public User Class
public:
    CameraSetting_Dialog *m_camSet;
    ImageThread          *m_imgThread;
  //public member
public:
    bool isLoadCameraPressed;
    bool isfirstloadCameraThread;
signals:
    void send_ControlCamInfo(bool,bool,bool);
    void send_CloseCamInfo(bool,bool,bool);
public slots:
    void accept_leftImageDisp(QImage);
    void accept_rightImageDisp(QImage);
    void accept_allImageDisp(QImage,QImage);
private slots:
    void on_actionCamDevSet_triggered();
    void on_ExitBtn_clicked();
    void on_openVisonbtn_clicked();

private:
    Ui::VisionMainWindow *ui;
};

#endif // VISIONMAINWINDOW_H
