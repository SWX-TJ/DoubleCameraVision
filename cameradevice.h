#ifndef CAMERADEVICE_H
#define CAMERADEVICE_H
#include <QObject>
#include <QSize>
//Camera Class
class CameraDevice : public QObject
{
    Q_OBJECT
public:
    explicit CameraDevice(QObject *parent = nullptr);
    ~CameraDevice();
//Member
private:
    int     Camindex;
    QSize   CamFrameSize;
    bool    isOpen;

//Function
public:
    void    setCamName(int camindex);
    void    setCamFrameSize(QSize camframesize);
    bool    isCamOpen(void);
    QString returnCamName(void);
    QSize   returnCamFrameSize(void);
    int      returnCamdevIndex(void);
};

#endif // CAMERADEVICE_H
