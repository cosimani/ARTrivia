QT += widgets network multimedia quick qml quickwidgets sql

TEMPLATE = app

DEFINES += NO_DEBUG_ARUCO
DEFINES += INDIVIDUAL

DEFINES += REGISTRO_CON_FACEBOOK_NOOO

#DEFINES += ABRIR_CAMARA_DIRECTO


contains(ANDROID_TARGET_ARCH, armeabi-v7a)  {  # Para Android

###############################################################################################
    DIR_ANDROID_OPENCV = /home/cosimani/Proyecto/2017/OpenCV-3.2.0-android-sdk/sdk/native #####
###############################################################################################

    INCLUDEPATH += $$DIR_ANDROID_OPENCV/jni/include

    LIBS += $$DIR_ANDROID_OPENCV/libs/armeabi-v7a/libopencv_java3.so

    LIBS += -L"$$DIR_ANDROID_OPENCV/libs/armeabi-v7a" \
            -L"$$DIR_ANDROID_OPENCV/3rdparty/libs/armeabi-v7a"
    LIBS += \
        -lIlmImf\
        -llibjasper\
        -llibjpeg\
        -llibpng\
        -llibtiff\
        -llibwebp\
        -ltbb\
        -lopencv_core\
        -lopencv_imgproc\
        -lopencv_highgui\
        -lopencv_calib3d\
        -lopencv_features2d\
        -lopencv_ml

    ANDROID_EXTRA_LIBS = $$DIR_ANDROID_OPENCV/libs/armeabi-v7a/libopencv_java3.so

    DISTFILES += \
        android/AndroidManifest.xml \
        android/gradle/wrapper/gradle-wrapper.jar \
        android/gradlew \
        android/res/values/libs.xml \
        android/build.gradle \
        android/gradle/wrapper/gradle-wrapper.properties \
        android/gradlew.bat

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

}

! contains(ANDROID_TARGET_ARCH, armeabi-v7a)  {  # Si no es para Android entonces para Desktop

#    message( Compilacion para Desktop )

###############################################################################################
    unix:DIR_OPENCV_LIBS = /usr/local/lib  ####################################################
###############################################################################################

    unix:INCLUDEPATH += "/usr/include/GL/"                             # OpenGL
    unix:LIBS += "/usr/lib/x86_64-linux-gnu/libglut.so"                # OpenGL

    unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_core.so         # OpenCV
    unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_highgui.so      # OpenCV
    unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_imgproc.so      # OpenCV
    unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_objdetect.so    # OpenCV
    unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_calib3d.so      # OpenCV
    unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_ml.so           # OpenCV
    unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_features2d.so

    unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_features2d.so
    unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_video.so

    unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_flann.so
    unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_photo.so
    unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_stitching.so
    unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_superres.so
    unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_video.so
    unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_videostab.so
    unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_imgcodecs.so
    unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_videoio.so

    unix:LIBS += "/usr/lib/x86_64-linux-gnu/lib3ds.so"                 # Modelos 3D


    DEFINES += DESKTOP
}

SOURCES += \
    main.cpp \
           aruco/ar_omp.cpp \
           aruco/arucofidmarkers.cpp \
           aruco/board.cpp \
           aruco/boarddetector.cpp \
           aruco/cameraparameters.cpp \
           aruco/highlyreliablemarkers.cpp \
           aruco/marker.cpp \
           aruco/markerdetector.cpp \
           aruco/subpixelcorner.cpp \
    image.cpp \
    datamanager.cpp \
    game.cpp \
    indexedbutton.cpp \
    login.cpp \
    question.cpp \
    error.cpp \
    scene.cpp \
    squircle.cpp \
    data.cpp \
    database.cpp \
    registroconmail.cpp \
    registroconfacebook.cpp \
    loginIndividual.cpp \
    datafacebook.cpp \
    abandonar.cpp

HEADERS += \
           aruco/ar_omp.h \
           aruco/aruco.h \
           aruco/arucofidmarkers.h \
           aruco/board.h \
           aruco/boarddetector.h \
           aruco/cameraparameters.h \
           aruco/exports.h \
           aruco/highlyreliablemarkers.h \
           aruco/marker.h \
           aruco/markerdetector.h \
           aruco/subpixelcorner.h \
    image.hpp \
    datamanager.hpp \
    game.hpp \
    indexedbutton.hpp \
    login.hpp \
    question.hpp \
    error.hpp \
    scene.h \
    squircle.h \
    data.hpp \
    database.hpp \
    registroconmail.h \
    registroconfacebook.h \
    loginIndividual.hpp \
    datafacebook.hpp \
    abandonar.h

FORMS += \
    game.ui \
    login.ui \
    question.ui \
    error.ui \
    scene.ui \
    data.ui \
    registroconmail.ui \
    registroconfacebook.ui \
    loginIndividual.ui \
    abandonar.ui

RESOURCES += \
    resources.qrc \
    qmlcamera.qrc \
    files.qrc \
    openglunderqml.qrc


