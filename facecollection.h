#ifndef FACECOLLECTION_H
#define FACECOLLECTION_H
#include <QWidget>
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
private slots:
    void on_pushButton_clicked();
    void on_CameraRadBtn_clicked();
    void on_PhotoRadBtn_clicked();

private:
    Ui::FaceCollection *ui;
};

#endif // FACECOLLECTION_H
