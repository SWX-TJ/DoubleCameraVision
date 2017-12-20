#ifndef VISIONMAINWINDOW_H
#define VISIONMAINWINDOW_H
#include <QMainWindow>
#include "camerasetting_dialog.h"
#include "facecollection.h"
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
    FaceCollection       *m_facecollect;
    ImageThread          *m_imgThread;
    QAction *facedetect_action;
    QAction *reset_action;
    //public member
public:
    bool isLoadCameraPressed;
    bool isfirstloadCameraThread;
    bool isprocFaceDetetion;
signals:
    void send_ControlCamInfo(bool,bool,bool);
    void send_CloseCamInfo(bool,bool,bool);
    void send_isFaceDetetionInof(bool);
public slots:
    void accept_leftImageDisp(QImage);
    void accept_rightImageDisp(QImage);
    void accept_allImageDisp(QImage,QImage);
    void process_facedetectionaction(bool);
private slots:
    void on_actionCamDevSet_triggered();
    void on_ExitBtn_clicked();
    void on_openVisonbtn_clicked();
    void on_actionFaceCollection_triggered();
private:
    Ui::VisionMainWindow *ui;
};

#endif // VISIONMAINWINDOW_H
