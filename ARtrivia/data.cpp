#include "data.hpp"
#include "ui_data.h"

#include <QJsonDocument>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QClipboard>

Data::Data( QWidget *parent ) : QWidget( parent ),
                                ui( new Ui::Data ),
                                isAutorizado(false)
{
    ui->setupUi( this );

    ui->background->setImage( "background.jpg", FILL );

    QPalette palette = this->palette();
    palette.setColor( QPalette::WindowText, QColor( 255, 255, 255 ) );
    this->setPalette( palette );

    ui->startButton->setFixedHeight( 2 * ui->startButton->sizeHint().height() );

    ui->teamNameLineEdit->setValidator(new QRegExpValidator( QRegExp( "[A-Za-z0-9_]+" ), this ) );
    ui->passwordLineEdit->setValidator(new QRegExpValidator( QRegExp( "[A-Za-z0-9_]+" ), this ) );
    ui->rePasswordLineEdit->setValidator(new QRegExpValidator( QRegExp( "[A-Za-z0-9_]+" ), this ) );

    connect( ui->passwordLineEdit, SIGNAL( textChanged( QString ) ), SLOT( matchPasswords( QString ) ) );
    connect( ui->rePasswordLineEdit, SIGNAL( textChanged( QString ) ), SLOT( matchPasswords( QString ) ) );

    connect( ui->showPasswordsCheckBox, SIGNAL( toggled( bool ) ), SLOT( changePasswordEchoMode( bool ) ) );

    ui->member1NameLineEdit->setValidator(new QRegExpValidator( QRegExp( "[A-Za-z ]+" ), this ) );
    ui->member2NameLineEdit->setValidator(new QRegExpValidator( QRegExp( "[A-Za-z ]+" ), this ) );
    ui->member3NameLineEdit->setValidator(new QRegExpValidator( QRegExp( "[A-Za-z ]+" ), this ) );

    ui->member1DniLineEdit->setValidator(new QRegExpValidator( QRegExp( "[0-9]+" ), this ) );
    ui->member2DniLineEdit->setValidator(new QRegExpValidator( QRegExp( "[0-9]+" ), this ) );
    ui->member3DniLineEdit->setValidator(new QRegExpValidator( QRegExp( "[0-9]+" ), this ) );

    connect( ui->teamNameLineEdit, SIGNAL( textChanged( QString ) ), SLOT( lineEditChanged( QString ) ) );
    connect( ui->member1NameLineEdit, SIGNAL( textChanged( QString ) ), SLOT( lineEditChanged( QString ) ) );
    connect( ui->member1DniLineEdit, SIGNAL( textChanged( QString ) ), SLOT( lineEditChanged( QString ) ) );
    connect( ui->member2NameLineEdit, SIGNAL( textChanged( QString ) ), SLOT( lineEditChanged( QString ) ) );
    connect( ui->member2DniLineEdit, SIGNAL( textChanged( QString ) ), SLOT( lineEditChanged( QString ) ) );
    connect( ui->member3NameLineEdit, SIGNAL( textChanged( QString ) ), SLOT( lineEditChanged( QString ) ) );
    connect( ui->member3DniLineEdit, SIGNAL( textChanged( QString ) ), SLOT( lineEditChanged( QString ) ) );

    connect( ui->startButton, SIGNAL( pressed() ), SLOT( start() ) );

    connect( DataManager::getInstance(), SIGNAL( availableName( bool ) ), SLOT( showData( bool ) ) );

    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_descargaFinalizada(QNetworkReply*)));

    connect(ui->pbFacebook, SIGNAL(pressed()), this, SLOT(slot_inicioLoginFacebook()));

    ui->lEnlace->setText("<a href=\"https://www.facebook.com/device\">Click Here!</a>");
    ui->lEnlace->setTextFormat(Qt::RichText);
    ui->lEnlace->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->lEnlace->setOpenExternalLinks(true);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slot_consultaCada5seg()));

    managerCada5 = new QNetworkAccessManager(this);
    connect(managerCada5, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slot_descargaFinalizadaCada5seg(QNetworkReply*)));

    managerConAccessToken = new QNetworkAccessManager(this);
    connect(managerConAccessToken, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slot_descargaFinalizadaConAccessToken(QNetworkReply*)));
}

Data::~Data()
{
    delete ui;
}

bool Data::matchPasswords( QString )
{
    bool ok = ( ui->passwordLineEdit->text() == ui->rePasswordLineEdit->text() );
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

void Data::lineEditChanged( QString )
{
    ui->errorLabel->clear();
}

void Data::start()
{
    qDebug() << "data.cpp / start()";
    if( !validateFields() ) return;
    if( !this->matchPasswords( "" ) ) return;

    ui->teamNameLineEdit->setEnabled( false );
    ui->rePasswordLineEdit->setEnabled( false );
    ui->passwordLineEdit->setEnabled( false );
    ui->member1NameLineEdit->setEnabled( false );
    ui->member1DniLineEdit->setEnabled( false );
    ui->member2NameLineEdit->setEnabled( false );
    ui->member2DniLineEdit->setEnabled( false );
    ui->member3NameLineEdit->setEnabled( false );
    ui->member3DniLineEdit->setEnabled( false );
    ui->startButton->setEnabled( false );

    QString teamName( ui->teamNameLineEdit->text().toLower().simplified() );
    QString password( ui->passwordLineEdit->text() );

    QString member1Name( ui->member1NameLineEdit->text().toLower().simplified() );
    int member1Dni = ui->member1DniLineEdit->text().simplified().toInt();

    QString member2NameLineEditText( ui->member2NameLineEdit->text().toLower().simplified() );
    QString member2Name( member2NameLineEditText.isEmpty() ? "" : member2NameLineEditText );

    QString member2DniLineEditText( ui->member2DniLineEdit->text().simplified() );
    int member2Dni = member2DniLineEditText.isEmpty() ? 0 : member2DniLineEditText.toInt();

    QString member3NameLineEditText( ui->member3NameLineEdit->text().toLower().simplified() );
    QString member3Name( member3NameLineEditText.isEmpty() ? "" : member3NameLineEditText );

    QString member3DniLineEditText( ui->member3DniLineEdit->text().simplified() );
    int member3Dni = member3DniLineEditText.isEmpty() ? 0 : member3DniLineEditText.toInt();

    Database::getInstance()->saveData( teamName, password );

    if( !DataManager::getInstance()->requestInit( teamName,
                                                  password,
                                                  member1Name,
                                                  member1Dni,
                                                  member2Name,
                                                  member2Dni,
                                                  member3Name,
                                                  member3Dni ) )
    {
        qDebug() << "No se pudo iniciar el juego";
    }
}

bool Data::validateFields()
{
    QString teamName( ui->teamNameLineEdit->text().toLower().simplified() );
    QString password( ui->passwordLineEdit->text() );
    QString rePassword( ui->rePasswordLineEdit->text() );

    QString member1Name( ui->member1NameLineEdit->text().toLower().simplified() );
    QString member1Dni( ui->member1DniLineEdit->text().simplified() );

    bool error = ( teamName.isEmpty() ||
                   password.isEmpty() ||
                   rePassword.isEmpty() ||
                   member1Name.isEmpty() ||
                   member1Dni.isEmpty() );

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

void Data::changePasswordEchoMode( bool checked )
{
    ui->passwordLineEdit->setEchoMode( checked ? QLineEdit::Normal : QLineEdit::Password );
    ui->rePasswordLineEdit->setEchoMode( checked ? QLineEdit::Normal : QLineEdit::Password );
}

void Data::showData( bool available )
{
    if( available )
    {
        this->hide();
        Game::getInstance()->show();

        ui->teamNameLineEdit->clear();
        ui->passwordLineEdit->clear();
        ui->rePasswordLineEdit->clear();
        ui->member1NameLineEdit->clear();
        ui->member1DniLineEdit->clear();
        ui->member2NameLineEdit->clear();
        ui->member2DniLineEdit->clear();
        ui->member3NameLineEdit->clear();
        ui->member3DniLineEdit->clear();
        ui->errorLabel->clear();
    }
    else
    {
        ui->errorLabel->setText( "El nombre de equipo no está disponible" );
    }

    ui->teamNameLineEdit->setEnabled( true );
    ui->passwordLineEdit->setEnabled( true );
    ui->rePasswordLineEdit->setEnabled( true );
    ui->member1NameLineEdit->setEnabled( true );
    ui->member1DniLineEdit->setEnabled( true );
    ui->member2NameLineEdit->setEnabled( true );
    ui->member2DniLineEdit->setEnabled( true );
    ui->member3NameLineEdit->setEnabled( true );
    ui->member3DniLineEdit->setEnabled( true );
    ui->startButton->setEnabled( true );
}

void Data::slot_inicioLoginFacebook()
{
    QStringList datosFacebook = Database::getInstance()->readDataFacebook();
    QString access_token = datosFacebook.at(0);

    if ( ! access_token.isEmpty() )  {  // Cuando ya autorizamos y tenemos el token

        QUrl url("https://graph.facebook.com/v2.3/me?fields=name,picture&access_token=" + access_token);

        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        managerConAccessToken->get(req);

    }
    else  {  // Comenzar el proceso de autorizacion

        QString access_token = "144814742712571|9c877294c73e5787b5c94bfea5739ac8";

        QUrl url("https://graph.facebook.com/v2.6/device/login");

        QByteArray postData;
        postData.append("access_token=" + access_token);

        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        manager->post(req, postData);

    }

}

void Data::slot_consultaCada5seg()
{
    QString access_token = "144814742712571|9c877294c73e5787b5c94bfea5739ac8";

    QUrl url("https://graph.facebook.com/v2.6/device/login_status");

    QByteArray postData;
    postData.append("access_token=" + access_token);
    postData.append("&code=" + this->longCode);

    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    managerCada5->post(req, postData);
}

void Data::slot_descargaFinalizada(QNetworkReply *reply)
{
    QByteArray ba = reply->readAll();

    qDebug() << ba;

    QJsonDocument d = QJsonDocument::fromJson(ba);
    QJsonObject sett2 = d.object();
    QJsonValue valueUserCode = sett2.value( QString("user_code") );
    QJsonValue valueLongCode = sett2.value( QString("code") );

    qDebug() << valueUserCode.toString();

    ui->lCodigo->setText( valueUserCode.toString() );

    QApplication::clipboard()->setText( ui->lCodigo->text() );

    qDebug() << "Clipboard" << QApplication::clipboard()->text();

    this->longCode = valueLongCode.toString();

    timer->start(30000);

//    qWarning() << value;
//    QJsonObject item = value.toObject();
    //    qWarning() << tr("QJsonObject of description: ") << item;
}

void Data::slot_descargaFinalizadaCada5seg(QNetworkReply *reply)
{
    QByteArray ba = reply->readAll();

    qDebug() << ba;

    QJsonDocument d = QJsonDocument::fromJson(ba);
    QJsonObject sett2 = d.object();
    QJsonValue valueAccessToken = sett2.value( QString("access_token") );

    qDebug() << valueAccessToken.toString();

    // Aca se almacena el access token, luego que se autoriza
    Database::getInstance()->saveDataFacebook( valueAccessToken.toString(), "", "", "", "" );

    this->slot_inicioLoginFacebook();  // Aca para que se loguee ya con el access token y para mostrar la foto


}

void Data::slot_descargaFinalizadaConAccessToken(QNetworkReply *)
{
//    QByteArray ba = reply->readAll();

//    qDebug() << ba;

//    if ( ba.contains("Invalid OAuth access token") )  {  // Parece que el token ya no es valido

//        // Borramos el token almacenado porque ya no sirve
//        Database::getInstance()->saveDataFacebook("", "", "", "", "");

//        this->slot_inicioLoginFacebook();  // Para que vuelva a pedir la autorizacion

//        return;
//    }

//    QJsonDocument d = QJsonDocument::fromJson(ba);
//    QJsonObject sett2 = d.object();

//    QJsonValue valueName = sett2.value( QString("name") );

//    QJsonValue valuePicture = sett2.value( QString("picture") );

//    QJsonObject objectPicture = valuePicture.toObject();

//    QJsonValue valueData = objectPicture.value( QString("data") );

//    QJsonObject objectData = valueData.toObject();
//    QJsonValue valueIsSilhouette = objectData.value( QString("is_silhouette") );
//    QJsonValue valueUrl = objectData.value( QString("url") );

//    qDebug() << "\n\n valueIsSilhouette" << valueIsSilhouette << "\n\n";

//    QStringList datosFacebook = Database::getInstance()->readDataFacebook();
//    QString access_token = datosFacebook.at(0);
////    QString name = datosFacebook.at(1);
////    QString picture = datosFacebook.at(2);

//    Database::getInstance()->saveDataFacebook( access_token, valueName.toString(), valueUrl.toString() );

//    qDebug() << "\n\nSe almaceno en la base:" << access_token << valueName.toString() << valueUrl.toString() << "\n\n";

//    ui->imagenPerfil->setImageUrl( valueUrl.toString() );
}


