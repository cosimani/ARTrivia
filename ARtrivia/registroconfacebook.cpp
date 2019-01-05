#include "registroconfacebook.h"
#include "ui_registroconfacebook.h"

#include "database.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QClipboard>
#include "datafacebook.hpp"
#include <QClipboard>

RegistroConFacebook::RegistroConFacebook(QWidget *parent) : QWidget(parent),
                                                            ui(new Ui::RegistroConFacebook),
                                                            timerEnlace( new QTimer( this ) )
{
    ui->setupUi(this);

    ui->logo->soyLogo();
    ui->background->setImage( "background.jpg", FILL );

    connect(ui->pbAutorizar, SIGNAL(pressed()), this, SLOT(slot_autorizar()));

    ui->lEnlace->setText("<a href=\"https://www.facebook.com/device\">https://www.facebook.com/device</a>");
    ui->lEnlace->setTextFormat(Qt::RichText);
    ui->lEnlace->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->lEnlace->setOpenExternalLinks(true);

    connect( DataFacebook::getInstance(), SIGNAL( signal_codigoEnClipboard(QString) ),
             this, SLOT(slot_codigoParaAutorizar(QString) ) );

    ui->pbAutorizar->setFixedHeight( 2 * ui->pbAutorizar->sizeHint().height() );

    connect( timerEnlace, SIGNAL( timeout() ), this, SLOT( slot_mostrarEnlace() ) );

    ui->lCodigo->hide();
    ui->lEnlace->hide();
    ui->lInfo->hide();

    // PAra que se cierre esta ventana cuando va a facebook.com/device. Por si vuelve el usuario y ya este cerrada
    connect(ui->lEnlace, SIGNAL( linkActivated(QString) ), this, SLOT( close() ) );

}

RegistroConFacebook::~RegistroConFacebook()
{
    delete ui;
}

void RegistroConFacebook::slot_autorizar()
{
    ui->pbAutorizar->setEnabled( false );

    QStringList datosFacebook = Database::getInstance()->readDataFacebook();  // Lee esto: access_token, name, picture
    QString access_token = datosFacebook.at(0);

    if ( access_token.isEmpty() )  {  // Comenzar el proceso de autorizacion

        DataFacebook::getInstance()->requestLogin(
                    "144814742712571|9c877294c73e5787b5c94bfea5739ac8",
                    "https://graph.facebook.com/v2.6/device/login");
    }
}



void RegistroConFacebook::slot_codigoParaAutorizar(QString code)
{
    ui->lCodigo->setText( "Código de autorización: " + code );

//    QFont f ( "Arial", 20, QFont::Bold );
//    ui->lCodigo->setFont( f );

    QString ritchText("<html><head><meta name=\"qrichtext\" content=\"1\" /></head>"
                   "<body style=\" white-space: pre-wrap; font-family:Sans Serif; "
                   "font-size:9pt; font-weight:400; font-style:normal; text-decoration:none;\">"
                   "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; "
                   "margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:25pt;\"> "
                   "Código de autorización<br>" + code + "</p>" "</body></html>");
    ui->lCodigo->setText(ritchText);
    ui->lCodigo->setWordWrap( true );
    ui->lCodigo->show();

    QString mensaje = "Utilice este código para dirigirse al siguiente link de facebook y colocarlo para autorizar "
                      "a esta aplicación para obtener su nombre y foto de perfil. Para "
                      "agilizar esto ya se encuentra copiado, sólo deberás pegarlo.";

//    QFont f ( "Arial", 20, QFont::Bold );
//    ui->lInfo->setFont( f );

    ritchText = "<html><head><meta name=\"qrichtext\" content=\"1\" /></head>"
                "<body style=\" white-space: pre-wrap; font-family:Sans Serif; "
                "font-size:9pt; font-weight:400; font-style:normal; text-decoration:none;\">"
                "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; "
                "margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:18pt;\"> "
                + mensaje + "</p>" "</body></html>";
    ui->lInfo->setText(ritchText);
//    ui->lInfo->setText( mensaje );
    ui->lInfo->setWordWrap( true );
    ui->lInfo->show();


    timerEnlace->setSingleShot( true );
    timerEnlace->start( 2000 );
}

void RegistroConFacebook::slot_mostrarEnlace()
{
    QString ritchText = "<html><head><meta name=\"qrichtext\" content=\"1\" /></head>"
                "<body style=\" white-space: pre-wrap; font-family:Sans Serif; "
                "font-size:9pt; font-weight:400; font-style:normal; text-decoration:none;\">"
                "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; "
                "margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:20pt;\"> "
                "Presionar aquí:<br>"
                "<a href=\"https://www.facebook.com/device\">https://www.facebook.com/device</a></p>"
                "</body></html>";
    ui->lEnlace->setText(ritchText);
//    ui->lInfo->setText( mensaje );
    ui->lEnlace->setWordWrap( true );
    ui->lEnlace->show();


//    ui->lEnlace->setText( "<a href=\"https://www.facebook.com/device\"> https://www.facebook.com/device </a>" );
}

