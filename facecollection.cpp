#include "facecollection.h"
#include "ui_facecollection.h"

FaceCollection::FaceCollection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FaceCollection)
{
    ui->setupUi(this);
    isCameraCollect = false;
    isPhotoCollect  = false;
}

FaceCollection::~FaceCollection()
{
    delete ui;
}

void FaceCollection::on_pushButton_clicked()
{
       returnSignal(2);
}

void FaceCollection::on_CameraRadBtn_clicked()
{
    isCameraCollect = !isCameraCollect;
    isPhotoCollect  = false;
}

void FaceCollection::on_PhotoRadBtn_clicked()
{
    isCameraCollect = false;
    isPhotoCollect  = !isPhotoCollect;
}
