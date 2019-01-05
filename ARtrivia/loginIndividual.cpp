#include "loginIndividual.hpp"
#include "ui_loginIndividual.h"

#include <QMessageBox>

#include <QUrlQuery>
#include <QUrl>
#include <QList>
#include <QPair>

#include "registroconmail.h"
#include "registroconfacebook.h"
#include "abandonar.h"

#include "datafacebook.hpp"

#include <QScreen>
#include <QApplication>
#include <QDesktopWidget>

LoginIndividual::LoginIndividual( QWidget *parent ) : QWidget( parent ),
                                                      ui( new Ui::LoginIndividual ),
                                                      registroConMail( NULL ),
                                                      registroConFacebook( NULL ),
                                                      abandonar( NULL )


{
    ui->setupUi( this );

//    ui->logo->setImage( "ubp.png", ADJUST );  // La comento el Emi
    ui->logo->soyLogo();  // Emi
    ui->background->setImage( "background.jpg", FILL );

    QPalette palette = this->palette();
    palette.setColor( QPalette::WindowText, QColor( 255, 255, 255 ) );
    this->setPalette( palette );

    ui->startButton->setFixedHeight( 2 * ui->startButton->sizeHint().height() );
    ui->pbCrearCuenta->setFixedHeight( 2 * ui->pbCrearCuenta->sizeHint().height() );
    ui->pbIniciaConFacebook->setFixedHeight( 2 * ui->pbIniciaConFacebook->sizeHint().height() );

    ui->lePassword->setValidator(new QRegExpValidator( QRegExp( "[A-Za-z0-9_]+" ), this ) );
    ui->leMail->setValidator(new QRegExpValidator( QRegExp( "[A-Za-z0-9_@.]+" ), this ) );

    connect( ui->lePassword, SIGNAL( textChanged( QString ) ), SLOT( lineEditChanged( QString ) ) );
    connect( ui->leMail, SIGNAL( textChanged( QString ) ), SLOT( lineEditChanged( QString ) ) );
    connect( ui->startButton, SIGNAL( pressed() ), SLOT( start() ) );

    connect( DataManager::getInstance(), SIGNAL( readyLogin( bool ) ), SLOT( showLogin( bool ) ) );

    connect(ui->pbInstrucciones, SIGNAL(clicked(bool)), this, SLOT(slot_mostrarInstrucciones()));

    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_descargaFinalizada(QNetworkReply*)));

    connect(ui->pbIniciaConFacebook, SIGNAL(pressed()), this, SLOT(slot_iniciaSesionConFacebook()));
    connect(ui->pbCrearCuenta, SIGNAL(pressed()), this, SLOT(slot_crearCuentaConMail()));


//    this->setAttribute(Qt::WA_DeleteOnClose);  // Para que se destruya si se llama a close(), no se destruye con hide()

    connect( ui->pbAbandonar, SIGNAL( pressed() ), this, SLOT( slot_abrirWidgetAbandonar() ) );

    connect( DataFacebook::getInstance(), SIGNAL( signal_perfilObtenido() ), this, SLOT( slot_acondicionarInterfaz() ) );

    connect( DataManager::getInstance(), SIGNAL( signal_mailRegistrado() ), this, SLOT( slot_acondicionarInterfaz() ) );

#ifdef REGISTRO_CON_FACEBOOK_NOOO
    ui->pbIniciaConFacebook->hide();
#endif

    this->slot_acondicionarInterfaz();
}

LoginIndividual::~LoginIndividual()
{
    qDebug() << "Ojo que se destruyo LoginIndividual";
    delete ui;
}

void LoginIndividual::resizeEvent( QResizeEvent* )
{
    QImage logo;
    logo.load( ":/resources/images/ubp.png" );
    ui->logo->setFixedSize( this->width(), logo.height() * this->width() / ( float )logo.width() );
}

void LoginIndividual::start()
{    
    if ( Database::getInstance()->isFacebookLogin() )  {
        ui->lePassword->setEnabled( false );
        ui->leMail->setEnabled( false );
        ui->startButton->setEnabled( false );

        QString id_facebook = Database::getInstance()->readDataFacebook().at( 4 );

        if( !DataManager::getInstance()->requestLoginFacebook( id_facebook ) )
            qDebug() << "Solicitud inicial incorrecta Facebook";
        else
            qDebug() << "Solicitud inicial correcta Facebook";
    }
    else  {


        QString password( ui->lePassword->text() );
        QString mail( ui->leMail->text().toLower() );

        bool error = password.isEmpty() || mail.isEmpty();

        if( error )  {
            ui->errorLabel->setText( "Completa los campos" );
            return;
        }
        else
            ui->errorLabel->clear();

        if( ! DataManager::getInstance()->requestLoginMail( password, mail ) )  {
            qDebug() << "Solicitud inicial incorrecta Mail";
            ui->errorLabel->setText( "Mail o clave incorrectos" );
        }
        else  {
            qDebug() << "Solicitud inicial correcta Mail";
            ui->errorLabel->clear();
            Database::getInstance()->saveDataConMail( password, mail );
        }
    }

}

void LoginIndividual::slot_descargaFinalizada(QNetworkReply *reply)
{
    qDebug() << reply->readAll();
}

void LoginIndividual::slot_iniciaSesionConFacebook()
{
    if ( Database::getInstance()->isMailLogin() )  {
        QMessageBox::information(this, "Información ARtrivia", "Ya tienes un usuario creado en esta App con tu cuenta "
                                 "de mail. Si no recuerdas tu mail y clave deberás pulsar el botón \"Abandonar\" para "
                                 "borrar tu usuario y registrarte nuevamente."
                                 );
        return;
    }


// Si no se hace close() ni hide(), entonces con el boto atras de celu, se vuelve a mostrar el widget anterior

//    this->close();  // Ojo que al estar seteado con this->setAttribute(Qt::WA_DeleteOnClose); se destruye con close()
//    this->hide();  // Se oculta pero no se destruye, y con el boton atras del celu, no vuelve sino que se cierra

    if ( registroConFacebook == NULL )  // Para crearlo solo la primera vez
        registroConFacebook = new RegistroConFacebook;

    registroConFacebook->show();


    // Identificador de la aplicación    "144814742712571"
    // Clave secreta de la aplicación    "496b9bf956cf4b6335160ecf1e4cdb87"

    // Las instrucciones estan en:
    // https://developers.facebook.com/docs/facebook-login/for-devices

//    QString access_token = "144814742712571|9c877294c73e5787b5c94bfea5739ac8";

//    QUrl url("https://graph.facebook.com/v2.6/device/login");

//    QByteArray postData;
//    postData.append("access_token=" + access_token);

//    QNetworkRequest req(url);

//    managerIniciaSesionConFacebook->post(req, postData);
}

void LoginIndividual::slot_crearCuentaConMail()
{
    if ( Database::getInstance()->isFacebookLogin() )  {
        QMessageBox::information(this, "Información ARtrivia", "Ya tienes un usuario creado en esta App con tu cuenta"
                                 " de Facebook. Si tienes problemas para acceder puede que se haya desautorizado"
                                 " a esta App para obtener tus datos públicos de Facebook. Nuestra recomendación es"
                                 " que pulses el botón \"Abandonar\" para borrar tu usuario y registrarte nuevamente."
                                 );
        return;
    }

    // Si no se hace close() ni hide(), entonces con el boto atras de celu, se vuelve a mostrar el widget anterior

//    this->close();  // Ojo que al estar seteado con this->setAttribute(Qt::WA_DeleteOnClose); se destruye con close()
//    this->hide();  // Se oculta pero no se destruye, y con el boton atras del celu, no vuelve sino que se cierra

    if ( registroConMail == NULL )  // Para crearlo solo la primera vez
        registroConMail = new RegistroConMail;

    registroConMail->show();
}

void LoginIndividual::slot_abrirWidgetAbandonar()
{
//    this->close();  // Ojo que al estar seteado con this->setAttribute(Qt::WA_DeleteOnClose); se destruye con close()
//    this->hide();  // Se oculta pero no se destruye, y con el boton atras del celu, no vuelve sino que se cierra

    if ( abandonar == NULL )  // Para crearlo solo la primera vez
        abandonar = new Abandonar;

    abandonar->show();

    // Bloqueamos este boton porque trae problemas si uno ingresa de nueva volviendo atras. Para no renegar lo
    // bloqueamos y listo. Si el usuario quiere borrar la cuenta, que cierre y abra de nuevo la App.
    ui->pbAbandonar->setEnabled( false );

}

void LoginIndividual::slot_acondicionarInterfaz()
{
    if ( ! Database::getInstance()->isFacebookLogin() && ! Database::getInstance()->isMailLogin() )  {
        ui->imagenPerfil->hide();
        ui->lNombreFacebook->hide();
        ui->layoutFacebook->setEnabled( false );

        ui->lePassword->show();
        ui->leMail->show();
        ui->layoutMail->setEnabled( true );

        ui->startButton->setText( "Iniciar sesión" );
    }

    // Por si el usuario fue a facebook.com/device, puso el codigo pero despues se cerro la App. Entonces, si se abre
    // puede utilizar el long_code para continuar con la autorizacion.
    if ( Database::getInstance()->isLongCode() )  {

        QString long_code = Database::getInstance()->readDataFacebook().at(3);

        DataFacebook::getInstance()->slot_codesObtenidos( "", long_code );

//        QMessageBox::information(this, "Información #SéUBP", "Se está terminando de configurar y autorizar tu usuario"
//                                 " de Facebook para usar esta App. Recuerda que sólo tenemos acceso a tu información"
//                                 " pública y sólo necesitamos tu nombre y foto de perfil para poder identificarte."
//                                 );

        qDebug() << "habia un long_code " << long_code;

    }

    if ( Database::getInstance()->isMailLogin() )  {
        qDebug() << "Database::isMailLogin()";
        ui->pbCrearCuenta->setEnabled(false);
        ui->pbIniciaConFacebook->setEnabled(true);

        ui->imagenPerfil->hide();
        ui->lNombreFacebook->hide();
        ui->layoutFacebook->setEnabled( false );

        ui->startButton->setText( "Iniciar sesión con DNI" );

        ui->layoutFacebook->setEnabled( false );

        QStringList data( Database::getInstance()->readDataConMail() );

        qDebug() << "Datos almacenados en el sqlite" << data;

        if( data.size() == 2 )
        {
            ui->lePassword->setText( data.at( 0 ) );
            ui->leMail->setText( data.at( 1 ) );
        }

    }


    if ( Database::getInstance()->isFacebookLogin() )  {

        qDebug() << "Database::isFacebookLogin()";
        ui->pbCrearCuenta->setEnabled(true);
        ui->pbIniciaConFacebook->setEnabled(false);


        ui->layoutFacebook->setEnabled( true );
        ui->imagenPerfil->show();
        ui->lNombreFacebook->show();

        ui->layoutMail->setEnabled( false );
        ui->startButton->show();

        ui->lePassword->setText( "" );
        ui->lePassword->setEnabled( false );

        ui->leMail->setVisible( false );
        ui->verticalSpacerMail->invalidate();
        ui->lePassword->setVisible( false );


        ui->startButton->setText( "Iniciar sesión con Facebook" );
        QString nombre_facebook = Database::getInstance()->readDataFacebook().at(1);
//        ui->leMail->setText( nombre_facebook );
//        ui->leMail->setEnabled( false );

        ui->lNombreFacebook->setText( nombre_facebook );

        QString urlPicture = Database::getInstance()->readDataFacebook().at(2);

        int ladoPicture = QApplication::desktop()->screenGeometry().width() / 5;
        ui->imagenPerfil->setFixedSize( ladoPicture, ladoPicture );

        ui->imagenPerfil->setImageUrl( urlPicture );

    }

}


void LoginIndividual::lineEditChanged( QString )
{
    ui->errorLabel->clear();
}

bool LoginIndividual::validateFields()
{
    QString password( ui->lePassword->text() );
    QString mail( ui->leMail->text() );

    bool error = password.isEmpty() || mail.isEmpty();

    if( error )
    {
        ui->errorLabel->setText( "Completa los campos" );
    }
    else
    {
        ui->errorLabel->clear();
    }

    return !error;
}

void LoginIndividual::showLogin( bool ok )
{
    if( ok )
    {
        this->hide();
//        this->close();

        Game::getInstance()->show();

        QString mail = ui->leMail->text();
        QString password = ui->lePassword->text();

        Database::getInstance()->saveDataConMail( password, mail );

        ui->lePassword->clear();
        ui->leMail->clear();
        ui->errorLabel->clear();
    }
    else
    {
        ui->errorLabel->setText( "Mail o clave incorrectos" );
    }

    ui->lePassword->setEnabled( true );
    ui->leMail->setEnabled( true );
    ui->startButton->setEnabled( true );
}

void LoginIndividual::slot_mostrarInstrucciones()
{
    QMessageBox::information(this, "Instrucciones ARtrivia", "La finalidad de este juego es sumar la mayor cantidad de puntos."
                             " En distintas zonas estarán colocados códigos (similares a los códigos QR) para que puedas capturarlos"
                             " tomándoles una foto con esta App, y de esta manera obtener 200 puntos. Luego, aparecerá un pregunta"
                             " con múltiples opciones para sumar 100 puntos extras. La App te guiará por todas las zonas"
                             " para encontrar estos códigos. En la parte superior podrás visualizar los puntos conseguidos"
                             " hasta el momento y tu ranking actual. \n\nEs importante capturar códigos distintos en cada zona."
                             " Si se captura un código ya capturado no obtendrás los 200 puntos, pero igualmente podrás responder nuevas"
                             " preguntas. "
#ifdef INDIVIDUAL

#else
                             " Se puede jugar de manera individual o en equipo de 2 o 3 jugadores. Es"
                             " importante que se registren con un nombre de equipo (de 1, 2 o 3 integrantes) y que los DNI"
                           " estén correctamente escritos. \n\nLos integrantes de cada equipo podrán jugar"
                           " cada uno en su propio celular si así lo desean. "

#endif
                             "Se requiere disponer de internet, ya sea con"
                             " Wifi o datos.\n\nBuena suerte en el juego!!"
                             );
}


