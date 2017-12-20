#-------------------------------------------------
#
# Project created by QtCreator 2017-11-23T19:35:03
#
#-------------------------------------------------

QT       += core gui
QT       +=multimedia
QT       +=multimediawidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Double_Robotvision
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        visionmainwindow.cpp \
    camerasetting_dialog.cpp \
    cameradevice.cpp \
    imagethread.cpp \
    local_algorithm.cpp \
    calibrationdlg.cpp \
    facecollection.cpp

HEADERS += \
        visionmainwindow.h \
    camerasetting_dialog.h \
    cameradevice.h \
    imagethread.h \
    local_algorithm.h \
    calibrationdlg.h \
    face_detection.h \
    face_alignment.h \
    face_identification.h \
    facecollection.h

FORMS += \
        visionmainwindow.ui \
    camerasetting_dialog.ui \
    calibrationdlg.ui \
    facecollection.ui

#INCLUDEPATH += C:/mysoftware/opencvmingw/include
#INCLUDEPATH += C:/mysoftware/opencvmingw/include/opencv
#INCLUDEPATH += C:/mysoftware/opencvmingw/include/opencv2
#DEPENDPATH += C:/mysoftware/opencvmingw/include

#win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_core320.dll
#win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_highgui320.dll
#win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_imgproc320.dll
#win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_calib3d320.dll
#win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_features2d320.dll
#win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_video320.dll
#win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_videoio320.dll
#win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_videostab320.dll
#win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_flann320.dll
#win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_objdetect320.dll
#win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_photo320.dll
#win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_imgcodecs320.dll
#win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_superres320.dll
#win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_shape320.dll
#win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_stitching320.dll
#win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_ml320.dll





INCLUDEPATH += $$PWD/../opencv/build/include
INCLUDEPATH += $$PWD/../opencv/build/include/opencv
INCLUDEPATH += $$PWD/../opencv/build/include/opencv2
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../opencv/build/x64/vc14/lib/ -lopencv_world320
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../opencv/build/x64/vc14/lib/ -lopencv_world320d

INCLUDEPATH += $$PWD/../opencv/build/x64/vc14
DEPENDPATH += $$PWD/../opencv/build/x64/vc14


win32: LIBS += -L$$PWD/../seeta/build/lib/ -lFaceDetection

INCLUDEPATH += $$PWD/../seeta/build/include
DEPENDPATH += $$PWD/../seeta/build/include

win32: LIBS += -L$$PWD/../seeta/build/lib/ -lFaceAlignment

INCLUDEPATH += $$PWD/../seeta/build/include
DEPENDPATH += $$PWD/../seeta/build/include

win32: LIBS += -L$$PWD/../seeta/build/lib/ -lFaceIdentification

INCLUDEPATH += $$PWD/../seeta/build/include
DEPENDPATH += $$PWD/../seeta/build/include
