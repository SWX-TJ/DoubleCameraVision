#include "cameradevice.h"

CameraDevice::CameraDevice(QObject *parent) : QObject(parent)
{
    isOpen = false;
}

CameraDevice::~CameraDevice()
{

}

void CameraDevice::setCamName(int camindex)
{
   Camindex  = camindex;
}

void CameraDevice::setCamFrameSize(QSize camframesize)
{
  CamFrameSize  = camframesize;
}
//Camera is open or close
bool CameraDevice::isCamOpen()
{
    if(isOpen)
    {
        return true;
    }
    else
    {
        return false;
    }
}

QString CameraDevice::returnCamName()
{
   QString  camName_str;
   int camindex = Camindex;
   camName_str =QString("Camera %1").arg(camindex);
   return camName_str;
}

QSize CameraDevice::returnCamFrameSize()
{
 QSize framesize_str;
 framesize_str = CamFrameSize;
 return framesize_str;
}

int CameraDevice::returnCamdevIndex()
{
    int camindex = Camindex;
    return camindex;
}
