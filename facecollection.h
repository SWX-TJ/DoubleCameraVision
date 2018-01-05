#ifndef FACECOLLECTION_H
#define FACECOLLECTION_H
#include <QWidget>
#include <imagethread.h>
#include "local_algorithm.h"
#include <QFile>
#include <QDataStream>
#include <QTextStream>
namespace Ui {
class FaceCollection;
}

class FaceCollection : public QWidget
{
    Q_OBJECT

public:
    explicit FaceCollection(QWidget *parent = 0);
    ~FaceCollection();
signals:
    void returnSignal(int);
public:
    bool isCameraCollect;
    bool isPhotoCollect;
    bool isLoadCameraPressed;
    bool isfirstloadCameraThread;
    bool isFaceCollection;
    QImage leftImage;
    QFile facedabase;
public:
    ImageThread *m_facecollectThread;
    Local_Algorithm *m_algorim;
signals:
    void send_ControlCamInfo(bool,bool,bool);
    void send_CloseCamInfo(bool,bool,bool);
    void send_ResetInfo();
    void send_isneedCamCali(bool);
public slots:
    // accept disp func v1.0
     void new_accept_leftImageDisp(Mat);
private slots:
    void on_pushButton_clicked();
    void on_CameraRadBtn_clicked();
    void on_PhotoRadBtn_clicked();
    void on_SetBtn_clicked();
    void on_CollectPhotoBtn_clicked();
private:
    Ui::FaceCollection *ui;
};

#endif // FACECOLLECTION_H
