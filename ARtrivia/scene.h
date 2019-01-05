#ifndef SCENE_H
#define SCENE_H

#include <QWidget>

#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <aruco/aruco.h>

using namespace cv;
using namespace aruco;


namespace Ui {
class Scene;
}

class Scene : public QWidget
{
    Q_OBJECT

public:
    explicit Scene(QWidget *parent = 0);
    ~Scene();

private:
    Ui::Scene *ui;

    MarkerDetector *markerDetector;
    std::vector< Marker > detectedMarkers;
    CameraParameters *cameraParameters;

public slots:
    void slot_imageProcessing( const QString& image );
    void slot_imagePath( const QString& path );

signals:
    void newMarkerDetected( int markerId );

};

#endif // SCENE_H
