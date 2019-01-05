#include "registroconmail.h"
#include "ui_registroconmail.h"
#include <QDebug>
#include <QRegExpValidator>
#include "database.hpp"
#include "datamanager.hpp"
#include "game.hpp"

RegistroConMail::RegistroConMail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegistroConMail)
{
    ui->setupUi(this);

    ui->logo->soyLogo();
    ui->background->setImage( "background.jpg", FILL );

    QPalette palette = this->palette();
    palette.setColor( QPalette::WindowText, QColor( 255, 255, 255 ) );
    this->setPalette( palette );

    ui->startButton->setFixedHeight( 2 * ui->startButton->sizeHint().height() );

    ui->leMail->setValidator(new QRegExpValidator( QRegExp( "[A-Za-z0-9_@.]+" ), this ) );
    ui->lePassword->setValidator(new QRegExpValidator( QRegExp( "[A-Za-z0-9_]+" ), this ) );
    ui->leRePassword->setValidator(new QRegExpValidator( QRegExp( "[A-Za-z0-9_]+" ), this ) );

    connect( ui->lePassword, SIGNAL( textChanged( QString ) ), SLOT( matchPasswords( QString ) ) );
    connect( ui->leRePassword, SIGNAL( textChanged( QString ) ), SLOT( matchPasswords( QString ) ) );

    connect( ui->showPasswordsCheckBox, SIGNAL( toggled( bool ) ), SLOT( changePasswordEchoMode( bool ) ) );

//    ui->leNombreCompleto->setValidator(new QRegExpValidator( QRegExp( "[A-Za-z ]+" ), this ) );

    connect( ui->leNombreCompleto, SIGNAL( textChanged( QString ) ), SLOT( lineEditChanged( QString ) ) );

    connect( ui->startButton, SIGNAL( pressed() ), SLOT( start() ) );

    connect( DataManager::getInstance(), SIGNAL( availableName( bool ) ), SLOT( showData( bool ) ) );

//    this->setAttribute(Qt::WA_DeleteOnClose);  // Para que se destruya si se llama a close(), no se destruye con hide()
}

RegistroConMail::~RegistroConMail()
{
    qDebug() << "Se destruye RegistroConMail ----------------------------------";
    delete ui;
}

bool RegistroConMail::matchPasswords( QString )
{
    bool ok = ( ui->lePassword->text() == ui->leRePassword->text() );
    if( !ok )
    {
        ui->errorLabel->setText( "Las contraseñas no coinciden" );
    }
    else
    {
        ui->errorLabel->clear();
    }
    return ok;
}

void RegistroConMail::lineEditChanged( QString )
{
    ui->errorLabel->clear();
}

void RegistroConMail::start()
{
//    qDebug() << "data.cpp / start()";
    if( !validateFields() ) return;
    if( !this->matchPasswords( "" ) ) return;

    ui->leNombreCompleto->setEnabled( false );
    ui->lePassword->setEnabled( false );
    ui->leRePassword->setEnabled( false );
    ui->leMail->setEnabled( false );
    ui->startButton->setEnabled( false );

    QString nombreCompleto( ui->leNombreCompleto->text().toLower().simplified() );
    QString password( ui->lePassword->text() );
    QString mail( ui->leMail->text().toLower().simplified() );

    Database::getInstance()->saveDataConMail( password, mail );

    if( !DataManager::getInstance()->requestInitMail( nombreCompleto,
                                                      password,
                                                      mail ) )
    {
        qDebug() << "No se pudo iniciar el juego";
    }
}

bool RegistroConMail::validateFields()
{
    QString password( ui->lePassword->text() );
    QString rePassword( ui->leRePassword->text() );

    QString nombreCompleto( ui->leNombreCompleto->text().toLower().simplified() );
    QString mail( ui->leMail->text().simplified() );

    bool error = ( password.isEmpty() ||
                   rePassword.isEmpty() ||
                   nombreCompleto.isEmpty() ||
                   mail.isEmpty() );

    if( error )
    {
        ui->errorLabel->setText( "Algún campo está incompleto" );
    }
    else
    {
        ui->errorLabel->clear();
    }

    return !error;
}

void RegistroConMail::changePasswordEchoMode( bool checked )
{
    ui->lePassword->setEchoMode( checked ? QLineEdit::Normal : QLineEdit::Password );
    ui->leRePassword->setEchoMode( checked ? QLineEdit::Normal : QLineEdit::Password );
}

void RegistroConMail::showData( bool available )
{
    if( available )
    {
//        this->hide();  // No se destruye
        this->close();  // Si se destruye este widget

//        Game::getInstance()->show();

        ui->leNombreCompleto->clear();
        ui->lePassword->clear();
        ui->leRePassword->clear();
        ui->leMail->clear();
        ui->errorLabel->clear();
    }
    else
    {
        ui->errorLabel->setText( "El mail no está disponible" );
    }

    ui->leNombreCompleto->setEnabled( true );
    ui->lePassword->setEnabled( true );
    ui->leRePassword->setEnabled( true );
    ui->leMail->setEnabled( true );
    ui->startButton->setEnabled( true );
}
