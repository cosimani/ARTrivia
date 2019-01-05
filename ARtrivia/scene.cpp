#include "scene.h"
#include "ui_scene.h"
#include <QQmlContext>
#include <QQuickItem>
#include <QDebug>

#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickImageProvider>
#include <QDebug>
#include <QDir>
#include <QFileInfo>

#include <QResource>
#include <QMessageBox>


Scene::Scene(QWidget *parent) : QWidget(parent),
                                ui(new Ui::Scene),
                                markerDetector( new MarkerDetector ),
                                cameraParameters( new CameraParameters )
{
    ui->setupUi(this);

#define CAMERA_PARAMETERS_FILE_RESOURCE ":/CameraParameters.yml"
#define CAMERA_PARAMETERS_FILE_LOCAL "./CameraParameters.yml"

    QResource yml( CAMERA_PARAMETERS_FILE_RESOURCE );

    QFile ymlFileResource(yml.absoluteFilePath());

    if (!ymlFileResource.open(QIODevice::ReadOnly | QIODevice::Text))  {
        qDebug() << "No se pudo iniciar camara 2 / Problema con parametros de la camara";
    }

    QTextStream in(&ymlFileResource);
    QString content = in.readAll();

        // Creo un archivo nuevo para almacenarlo
    QFile ymlFileLocal(CAMERA_PARAMETERS_FILE_LOCAL);
    if (!ymlFileLocal.open(QIODevice::WriteOnly | QIODevice::Text))  {
        qDebug() << "No se pudo iniciar camara / Problema con parametros de la camara";
    }

    QTextStream out(&ymlFileLocal);
    out << content;

    ymlFileLocal.close();

    cameraParameters->readFromXMLFile( CAMERA_PARAMETERS_FILE_LOCAL );


    if ( ! cameraParameters->isValid() )  {
        qDebug() << "Error con YML / No es valido. La App se cerrara";
    }

    ui->quickWidget->setSource(QUrl("qrc:///qmlcamera.qml"));

    QObject *rect = dynamic_cast<QObject*>(ui->quickWidget->rootObject());

    QObject::connect(rect, SIGNAL(signal_imageProcessing(QString)), this, SLOT(slot_imageProcessing(QString)));
    QObject::connect(rect, SIGNAL(signal_imagePath(QString)), this, SLOT(slot_imagePath(QString)));

//    qDebug() << "GL_SHADING_LANGUAGE_VERSION" << glGetString(GL_SHADING_LANGUAGE_VERSION);
}

Scene::~Scene()
{
    delete ui;
}

void Scene::slot_imageProcessing( const QString& path )  {

    QUrl imageUrl(path);
    QQmlEngine* engine = ui->quickWidget->engine();
//    QQmlEngine* engine = QQmlEngine::contextForObject(this)->engine();
    QQmlImageProviderBase* imageProviderBase = engine->imageProvider(imageUrl.host());
    QQuickImageProvider* imageProvider = static_cast<QQuickImageProvider*>(imageProviderBase);

    QSize imageSize;
    QString imageId = imageUrl.path().remove(0,1);
    QImage image = imageProvider->requestImage(imageId, &imageSize, imageSize);

    image = image.convertToFormat(QImage::Format_RGB888);

    image = image.scaled(480, 640, Qt::KeepAspectRatio);

//    qDebug() << image.save("/storage/sdcard0/Qt/qimage.jpg");

    cv::Mat frame(image.height(), image.width(),CV_8UC3,(uchar*)image.bits(),image.bytesPerLine());
    cv::cvtColor(frame, frame, CV_RGB2BGR);

    flip( frame, frame, 0 );
    flip( frame, frame, 1 );

//    qDebug() << cv::imwrite("/storage/sdcard0/Qt/mat.jpg", frame);

    if( !image.isNull())  {

        cameraParameters->resize( frame.size() );
        markerDetector->detect( frame, detectedMarkers, *cameraParameters, 0.57f );

        if ( detectedMarkers.size() == 0 )  {
            qDebug() << "No se capturo ningun marcador";

            emit newMarkerDetected( -1 );
        }

        for( unsigned int i = 0 ; i < detectedMarkers.size() ; i++ )
        {
            emit newMarkerDetected( detectedMarkers.at( i ).id );

            qDebug() << "markerCaptured en sceneeeEEEEEEEE" << detectedMarkers.at( i ).id;
//            QMessageBox::information(this, QString::number(detectedMarkers.at( i ).id),
//                                     QString::number(detectedMarkers.at( i ).id));

            break;  // Este break es para que solo detecte un marcador
        }

    }
}

void Scene::slot_imagePath(const QString &path)
{
    qDebug() << " Se borro la imagen " << path << QFile::remove(path);

}


