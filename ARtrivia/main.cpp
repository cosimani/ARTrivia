#include <QApplication>
#include <QSurfaceFormat>

#ifdef INDIVIDUAL
    #include "loginIndividual.hpp"
#else
    #include "login.hpp"
#endif

#include <QQuickView>
#include <QQmlContext>
#include <QQuickImageProvider>
#include <QDebug>
#include "squircle.h"
#include "database.hpp"

#include <QSslSocket>

#include <QTimer>

int main( int argc, char **argv )
{
    Q_INIT_RESOURCE( files );

    QApplication application( argc, argv );

    qDebug() << "QSslSocket::supportsSsl() " << QSslSocket::supportsSsl() ;

    // Esto es para limpiar los datos de Facebook de la base
//    Database::getInstance()->saveDataFacebook("");

//    Database::getInstance()->borrarTablas();

#ifdef INDIVIDUAL
    LoginIndividual * login = NULL;
#else
    Login * login = NULL;
#endif

#ifndef ABRIR_CAMARA_DIRECTO

    QQuickView * view = NULL;

    switch( Database::getInstance()->checkLogin() )
    {
    case 0:
//        QTimer::singleShot(30000, &application, SLOT(quit()));
        QTimer::singleShot(20000, &application, SLOT(quit()));  // Emmi

        qmlRegisterType<Squircle>("OpenGLUnderQML", 1, 0, "Squircle");

        view = new QQuickView;
        view->setResizeMode(QQuickView::SizeRootObjectToView);
        view->setSource(QUrl("qrc:///openglunderqml.qml"));
        view->show();

        break;
    case -1:
        qDebug() << "Si se devolvio un -1 es porque no anda la base. Seguimos y que sea lo que Dios quiera.";

    case 1:
        qDebug() << "Ya entro antes";

#ifdef INDIVIDUAL
        login = new LoginIndividual();
#else
        login = new Login();
#endif

        login->show();

        break;

    default:

        break;
    }

#else

    Game::getInstance()->show();

#endif


    return application.exec();
}
