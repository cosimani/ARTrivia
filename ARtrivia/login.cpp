#include "login.hpp"
#include "ui_login.h"

#include <QMessageBox>

#include <QUrlQuery>
#include <QUrl>
#include <QList>
#include <QPair>

#include "registroconmail.h"
#include "registroconfacebook.h"

Login::Login( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::Login )
{
    ui->setupUi( this );

//    ui->logo->setImage( "ubp.png", ADJUST );  // La comento el Emi
    ui->logo->soyLogo();  // Emi
    ui->background->setImage( "background.jpg", FILL );

    QPalette palette = this->palette();
    palette.setColor( QPalette::WindowText, QColor( 255, 255, 255 ) );
    this->setPalette( palette );

    ui->startButton->setFixedHeight( 2 * ui->startButton->sizeHint().height() );
    ui->registerButton->setFixedHeight( 2 * ui->registerButton->sizeHint().height() );

    ui->teamNameLineEdit->setValidator(new QRegExpValidator( QRegExp( "[A-Za-z0-9_]+" ), this ) );
    ui->passwordLineEdit->setValidator(new QRegExpValidator( QRegExp( "[A-Za-z0-9_]+" ), this ) );

    connect( ui->teamNameLineEdit, SIGNAL( textChanged( QString ) ), SLOT( lineEditChanged( QString ) ) );
    connect( ui->passwordLineEdit, SIGNAL( textChanged( QString ) ), SLOT( lineEditChanged( QString ) ) );
    connect( ui->startButton, SIGNAL( pressed() ), SLOT( start() ) );

    connect( DataManager::getInstance(), SIGNAL( readyLogin( bool ) ), SLOT( showLogin( bool ) ) );

    connect( ui->registerButton, SIGNAL( pressed() ), SLOT( showRegister() ) );

    QStringList data( Database::getInstance()->readData() );
    if( data.size() == 2 )
    {
        ui->teamNameLineEdit->setText( data.at( 0 ) );
        ui->passwordLineEdit->setText( data.at( 1 ) );
    }

    connect(ui->pbInstrucciones, SIGNAL(clicked(bool)), this, SLOT(slot_mostrarInstrucciones()));

}

Login::~Login()
{
    delete ui;
}

void Login::resizeEvent( QResizeEvent* )
{
    QImage logo;
    logo.load( ":/resources/images/ubp.png" );
    ui->logo->setFixedSize( this->width(), logo.height() * this->width() / ( float )logo.width() );
}

void Login::start()
{
    if( !validateFields() )
    {
        return;
    }

    ui->teamNameLineEdit->setEnabled( false );
    ui->passwordLineEdit->setEnabled( false );
    ui->startButton->setEnabled( false );

    QString teamName( ui->teamNameLineEdit->text().toLower().simplified() );
    QString password( ui->passwordLineEdit->text() );

    Database::getInstance()->saveData( teamName, password );

    if( !DataManager::getInstance()->requestLogin( teamName, password ) )
    {
        qDebug() << "Solicitud inicial incorrecta";
    }
    else
    {
        qDebug() << "Solicitud inicial correcta";
    }
}


void Login::lineEditChanged( QString )
{
    ui->errorLabel->clear();
}

bool Login::validateFields()
{
    QString teamName( ui->teamNameLineEdit->text().toLower().simplified() );
    QString password( ui->passwordLineEdit->text() );

    bool error = teamName.isEmpty() || password.isEmpty();

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

void Login::showRegister()
{
    this->hide();

    Data *data = new Data();
    data->showFullScreen();

    ui->teamNameLineEdit->clear();
    ui->passwordLineEdit->clear();
    ui->errorLabel->clear();

    ui->teamNameLineEdit->setEnabled( true );
    ui->passwordLineEdit->setEnabled( true );
    ui->startButton->setEnabled( true );
}

void Login::showLogin( bool ok )
{
    if( ok )
    {
        this->hide();
        Game::getInstance()->show();

        ui->teamNameLineEdit->clear();
        ui->passwordLineEdit->clear();
        ui->errorLabel->clear();
    }
    else
    {
        ui->errorLabel->setText( "Datos incorrectos" );
    }

    ui->teamNameLineEdit->setEnabled( true );
    ui->passwordLineEdit->setEnabled( true );
    ui->startButton->setEnabled( true );
}

void Login::slot_mostrarInstrucciones()
{
    QMessageBox::information(this, "Instrucciones #SéUBP", "La finalidad de este juego es sumar la mayor cantidad de puntos"
                             " para lograr las primeras ubicaciones en el ranking. En distintas zonas del campus estarán"
                             " colocados muchos códigos (similares a los códigos QR) a los cuales tienes que capturar"
                             " tomándoles una foto, y de esta manera obtener 200 puntos. Luego, aparecerá un pregunta"
                             " con múltiples opciones para sumar 100 puntos extras. La App te guiará por todo el campus"
                             " para encontrar los códigos. En la parte superior podrás visualizar los puntos conseguidos"
                             " hasta el momento y tu ranking actual. \n\nEl campus está dividido en varias zonas y es importante"
                             " capturar códigos distintos en cada zona (ver mapa para ubicar cada zona). Si se captura"
                             " un código ya capturado no obtendrás los 200 puntos, pero igualmente podrás responder nuevas"
                             " preguntas. Se puede jugar de manera individual o en equipo de 2 o 3 jugadores. Es"
                             " importante que se registren con un nombre de equipo (de 1, 2 o 3 integrantes) y que los DNI"
                             " estén correctamente escritos. \n\nLos integrantes de cada equipo podrán jugar"
                             " cada uno en su propio celular si así lo desean. Se requiere disponer de internet, ya sea con"
                             " Wifi o datos.\n\nBuena suerte en el juego!!"
                             );
}


