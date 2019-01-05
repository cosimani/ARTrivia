#include "datafacebook.hpp"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QClipboard>
#include <QApplication>

#include "database.hpp"
#include "datamanager.hpp"

DataFacebook *DataFacebook::instance = NULL;

DataFacebook::DataFacebook( QObject *parent ) :
                                                QObject( parent ),
                                                manager( new QNetworkAccessManager( this ) ),
                                                availableManager( true ),
                                                timer( new QTimer(this) ),
                                                cantidadDeRevisionDeAutorizacion( 0 )  // se setea antes de start()

{

    connect( this, SIGNAL( signal_codesObtenidos(QString,QString) ),
             this, SLOT( slot_codesObtenidos(QString,QString) ) );

    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_cada5segundos() ) );

}


bool DataFacebook::requestLogin( QString access_token, QString url )
{

//    QString access_token = "144814742712571|9c877294c73e5787b5c94bfea5739ac8";

//    QUrl qurl("https://graph.facebook.com/v2.6/device/login");

    QUrl qurl( url );

    QByteArray postData;
    postData.append("access_token=" + access_token);

    QNetworkRequest req(qurl);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    connect( manager, SIGNAL( finished( QNetworkReply* ) ), SLOT( responseLogin( QNetworkReply* ) ) );

    manager->post( req, postData );

    qDebug() << "POST Facebook" << qurl << "data" << postData;

    return true;
}

void DataFacebook::responseLogin( QNetworkReply *reply )
{
    qDebug() << "Respuesta HTTPS" << reply->readAll();
    if( ! reply->bytesAvailable() ) this->showErrorAndFinish();

    this->leaveManager();

    QByteArray ba = reply->readAll();

    qDebug() << ba;

    QJsonDocument d = QJsonDocument::fromJson(ba);
    QJsonObject sett2 = d.object();
    QJsonValue valueUserCode = sett2.value( QString("user_code") );
    QJsonValue valueLongCode = sett2.value( QString("code") );  // Este codigo dura 7 minutos / 420 segundos

    QString code = valueUserCode.toString();
    QString longCode = valueLongCode.toString();

    emit signal_codesObtenidos( code, longCode );

    // Guardamos el long_code por si el usuario va a facebook.com/device y despues cierra la App.
    Database::getInstance()->saveDataFacebook( "", "", "", "", longCode );



}

/**
 * @brief DataFacebook::slot_codeObtenido Al recibir el code se pide al usuario que vaya a facebook.com/device
 * @param code
 */
void DataFacebook::slot_codesObtenidos(QString code, QString longCode)
{
    QApplication::clipboard()->setText( code );
    qDebug() << "Se copia en el clipboard el code" << QApplication::clipboard()->text();
    emit signal_codigoEnClipboard( code );

    this->longCode = longCode;

    cantidadDeRevisionDeAutorizacion = 84;
    timer->start(5000);
}

void DataFacebook::slot_cada5segundos()
{
    // si se pidio 10 veces ya esta, borramos el long_code de la base para que cuando abra la app ya no
    // vuelva a preguntar si el usuario fue a facebook.com/device a autorizar
    if ( cantidadDeRevisionDeAutorizacion-- <= 0 )  {
        timer->stop();

        Database::getInstance()->saveDataFacebook( "", "", "", "", "" );

        return;
    }


    QString access_token = "144814742712571|9c877294c73e5787b5c94bfea5739ac8";

    QUrl url("https://graph.facebook.com/v2.6/device/login_status");

    QByteArray postData;
    postData.append("access_token=" + access_token);
    postData.append("&code=" + longCode);

    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    connect( manager, SIGNAL( finished( QNetworkReply* ) ), SLOT( responseAutorizacion( QNetworkReply* ) ) );
    manager->post(req, postData);
}

void DataFacebook::responseAutorizacion( QNetworkReply *reply )
{
    if( !reply->bytesAvailable() ) this->showErrorAndFinish();

    this->leaveManager();


    QByteArray ba = reply->readAll();

    qDebug() << ba;

    QJsonDocument d = QJsonDocument::fromJson(ba);
    QJsonObject sett2 = d.object();
    QJsonValue valueAccessToken = sett2.value( QString("access_token") );

    QString access_token_autorizacion = valueAccessToken.toString();

    if ( ! access_token_autorizacion.isEmpty() )  {
        qDebug() << "Autorizadoooooooooooooooo" << access_token_autorizacion;

        timer->stop();

        // Aca se almacena el access token, luego que se autoriza
        Database::getInstance()->saveDataFacebook( access_token_autorizacion, "", "", "", "" );


        // Ahora con el token ya podemos pedir foto de perfil

        QUrl url("https://graph.facebook.com/v2.3/me?fields=id,name,picture&access_token=" + access_token_autorizacion);

        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        connect( manager, SIGNAL( finished( QNetworkReply* ) ), SLOT( responsePerfil( QNetworkReply* ) ) );
        manager->get(req);

    }
}

void DataFacebook::responsePerfil( QNetworkReply *reply )
{
    if( !reply->bytesAvailable() ) this->showErrorAndFinish();

    this->leaveManager();

    QByteArray ba = reply->readAll();

    qDebug() << ba;

    if ( ba.contains("Invalid OAuth access token") )  {  // Parece que el token ya no es valido

        // Borramos el token almacenado porque ya no sirve
        Database::getInstance()->saveDataFacebook( "", "", "", "", "" );

        // Para que vuelva a pedir la autorizacion
        DataFacebook::getInstance()->requestLogin(
                    "144814742712571|9c877294c73e5787b5c94bfea5739ac8",
                    "https://graph.facebook.com/v2.6/device/login");

        return;
    }

    QJsonDocument d = QJsonDocument::fromJson(ba);
    QJsonObject sett2 = d.object();

    QJsonValue valueId = sett2.value( QString("id") );

    QJsonValue valueName = sett2.value( QString("name") );

    QJsonValue valuePicture = sett2.value( QString("picture") );

    QJsonObject objectPicture = valuePicture.toObject();

    QJsonValue valueData = objectPicture.value( QString("data") );

    QJsonObject objectData = valueData.toObject();
    QJsonValue valueIsSilhouette = objectData.value( QString("is_silhouette") );
    QJsonValue valueUrl = objectData.value( QString("url") );

    qDebug() << "\n\n valueIsSilhouette" << valueIsSilhouette << "\n\n";

    QStringList datosFacebook = Database::getInstance()->readDataFacebook();
    QString access_token = datosFacebook.at(0);
//    QString name = datosFacebook.at(1);
//    QString picture = datosFacebook.at(2);

    Database::getInstance()->saveDataFacebook( access_token,
                                               valueId.toString(),
                                               valueName.toString(),
                                               valueUrl.toString()
                                               ,"" );

    qDebug() << "\n\nSe almaceno en la base:" << access_token
                                              << valueId.toString()
                                              << valueName.toString()
                                              << valueUrl.toString() << "\n\n";

    if( ! DataManager::getInstance()->requestInitFacebook( access_token,
                                                          valueId.toString(),
                                                          valueName.toString() ) )
//                                                          valueName.toString().toLower().simplified() ) )
    {
        qDebug() << "No se pudo iniciar el juego";
    }



    // Es porque ya esta almacenado en la base local sqlite
    emit signal_perfilObtenido();

}

bool DataFacebook::doQuery( QVector< QStringList >params )
{
    if( !this->applyForManager() ) return false;

    QString query( "http://www.vayra.com.ar/trivia_ubp/trivia.php?" );
    query.append( "key=tr1v14ubp" );

    for( int i = 0; i < params.size(); i++ )
    {
        if( params.at( i ).size() < 2 )
        {
            continue;
        }
        query.append( "&" + params.at( i ).at( 0 ) + "=" + params.at( i ).at( 1 ) );
    }
    manager->get( QNetworkRequest( QUrl( query ) ) );
    return true;
}

void DataFacebook::showErrorAndFinish()
{
    Error::getInstance()->setDescription( "No se pudo conectar con Facebook, verifica tu conexión a internet y reinicia la aplicación" );
}

void DataFacebook::showNoActiveAndFinish()
{
    Error::getInstance()->setDescription( "La competencia aun no está activa, consulte a los organizadores!" );
}

void DataFacebook::showFinishedAndFinish()
{
    Error::getInstance()->setDescription( "Felicitaciones! Completaste todas las preguntas, espera los resultados!" );
}

DataFacebook *DataFacebook::getInstance()
{
    if( !instance )
    {
        instance = new DataFacebook();
    }
    return instance;
}

DataFacebook::~DataFacebook()
{
    delete instance;
}



bool DataFacebook::getAvailableManager() const
{
    return availableManager;
}

void DataFacebook::setAvailableManager( bool value )
{
    availableManager = value;
}

bool DataFacebook::applyForManager()
{
    if( !this->getAvailableManager() )
    {
        qDebug() << "El manager tiene una solicitud en curso";
        return false;
    }
    this->setAvailableManager( false );
    return true;
}

/**
 * @brief DataManager::leaveManager Desconecta todo lo que tenga conectado
 */
void DataFacebook::leaveManager()
{
    disconnect( manager, SIGNAL( finished( QNetworkReply* ) ), NULL, NULL );
    this->setAvailableManager( true );
}


/**
 * @brief DataManager::requestInit Registra los datos de un nuevo equipo con 1, 2 o 3 integrantes.
 * @param teamName
 * @param password
 * @param member1Name
 * @param member1Dni
 * @param member2Name
 * @param member2Dni
 * @param member3Name
 * @param member3Dni
 * @return
 */
bool DataFacebook::requestInit( QString teamName,
                               QString password,
                               QString member1Name,
                               int member1Dni,
                               QString member2Name,
                               int member2Dni,
                               QString member3Name,
                               int member3Dni )
{
    QVector< QStringList >params;
    params.append( QStringList() << "action" << "init" );
    params.append( QStringList() << "teamName" << teamName );
    params.append( QStringList() << "password" << password );
    params.append( QStringList() << "member1Name" << member1Name );
    params.append( QStringList() << "member1Dni" << QString::number( member1Dni ) );
    params.append( QStringList() << "member2Name" << member2Name );
    params.append( QStringList() << "member2Dni" << QString::number( member2Dni ) );
    params.append( QStringList() << "member3Name" << member3Name );
    params.append( QStringList() << "member3Dni" << QString::number( member3Dni ) );

    if( !this->doQuery( params ) ) return false;
    connect( manager, SIGNAL( finished( QNetworkReply* ) ), SLOT( responseInit( QNetworkReply* ) ) );
    return true;
}


bool DataFacebook::requestZones()
{
    QVector< QStringList >params;
    params.append( QStringList() << "action" << "zones" );

    if( !this->doQuery( params ) ) return false;
    connect( manager, SIGNAL( finished( QNetworkReply* ) ), SLOT( responseZones( QNetworkReply* ) ) );
    return true;
}


bool DataFacebook::requestQuestion( int teamId )
{
    QVector< QStringList >params;
    params.append( QStringList() << "action" << "question" );
    params.append( QStringList() << "teamId" << QString::number( teamId ) );

    if( !this->doQuery( params ) ) return false;
    connect( manager, SIGNAL( finished( QNetworkReply* ) ), SLOT( responseQuestion( QNetworkReply* ) ) );
    return true;
}


bool DataFacebook::requestStats( int teamId )
{
    QVector< QStringList >params;
    params.append( QStringList() << "action" << "stats" );
    params.append( QStringList() << "teamId" << QString::number( teamId ) );

    if( !this->doQuery( params ) ) return false;
    connect( manager, SIGNAL( finished( QNetworkReply* ) ), SLOT( responseStats( QNetworkReply* ) ) );
    return true;
}

bool DataFacebook::requestDetection( int teamId, int markerId )
{
    QVector< QStringList >params;
    params.append( QStringList() << "action" << "detection" );
    params.append( QStringList() << "teamId" << QString::number( teamId ) );
    params.append( QStringList() << "markerId" << QString::number( markerId ) );

    if( !this->doQuery( params ) ) return false;
    connect( manager, SIGNAL( finished( QNetworkReply* ) ), SLOT( responseDetection( QNetworkReply* ) ) );
    return true;
}



bool DataFacebook::requestAnswer( int teamId, int categoryId, int questionId, int answerId )
{

    QVector< QStringList >params;
    params.append( QStringList() << "action" << "answer" );
    params.append( QStringList() << "teamId" << QString::number( teamId ) );
    params.append( QStringList() << "categoryId" << QString::number( categoryId ) );
    params.append( QStringList() << "questionId" << QString::number( questionId ) );
    params.append( QStringList() << "answerId" << QString::number( answerId ) );

    if( !this->doQuery( params ) ) return false;
    connect( manager, SIGNAL( finished( QNetworkReply* ) ), SLOT( responseAnswer( QNetworkReply* ) ) );

    return true;
}




void DataFacebook::responseInit( QNetworkReply *reply )
{
    if( ! reply->bytesAvailable() ) this->showErrorAndFinish();

    this->leaveManager();  // Desconecta todas las conexiones del QNetworkAccessManager

    QString replyString( QString::fromLatin1( reply->readAll() ) );

    if( replyString == "juego_no_activo" ) this->showNoActiveAndFinish();

    if( replyString == "n" )
    {
        emit availableName( false );
        return;
    }
    else
    {

        QStringList results( replyString.split( "," ) );

        if( results.size() != 8 ||
            results.contains( "Database Error", Qt::CaseInsensitive ) )
        {
            qDebug() << "Error, algun error al guardar u obtener los datos del servidor";
            return;
        }

        emit availableName( true );
        emit readyInit( results.at( 0 ).toInt(),
                        results.at( 1 ),
                        results.at( 2 ),
                        results.at( 3 ),
                        results.at( 4 ),
                        results.at( 5 ).toInt(),
                        results.at( 6 ).toInt(),
                        results.at( 7 ).toInt() );
    }

}


void DataFacebook::responseZones( QNetworkReply *reply )
{
    if( !reply->bytesAvailable() ) this->showErrorAndFinish();

    this->leaveManager();

    QString replyString( QString::fromLatin1( reply->readAll() ) );

    if( replyString == "juego_no_activo" ) this->showNoActiveAndFinish();

    QStringList data( replyString.split( "," ) );
    data.removeLast();

    if( data.isEmpty() ||
        data.size() % 2 != 0 ||
        data.contains( "Database Error", Qt::CaseInsensitive ) )
    {
        qDebug() << "Error, algun error al obtener los datos del servidor";
        return;
    }

    QVector< QStringList > zones;
    for( int i = 0; i < data.size(); i += 2 )
    {
        zones.append( QStringList() << data.at( i ) << data.at( i + 1 ) );
    }
    emit readyZones( zones );
}

void DataFacebook::responseQuestion( QNetworkReply *reply )
{
    if( !reply->bytesAvailable() ) this->showErrorAndFinish();

    this->leaveManager();

    QString replyString( QString::fromLatin1( reply->readAll() ) );

    if( replyString == "juego_no_activo" ) this->showNoActiveAndFinish();

    if( replyString == "juego_terminado" )
    {
        qDebug() << "Este equipo completo todas las preguntas";
        qDebug() << "De todos modos, puede seguir buscando marcadores";
        return;
    }

    QStringList data( replyString.split( "," ) );
    data.removeLast();

    if( data.isEmpty() ||
        data.size() < 4 ||
        ( data.size() - 4 ) % 2 != 0 ||
        data.contains( "Database Error", Qt::CaseInsensitive ) )
    {
        qDebug() << "Error, algun error al obtener los datos del servidor";
        return;
    }

    QVector< QStringList > answers;
    for( int i = 4; i < data.size(); i += 2 )
    {
        answers.append( QStringList() << data.at( i ) << data.at( i + 1 ) );
    }
    emit readyQuestion( data.at( 0 ).toInt(),
                        data.at( 1 ),
                        data.at( 2 ).toInt(),
                        data.at( 3 ),
                        answers );
}

void DataFacebook::responseStats( QNetworkReply *reply )
{
    if( !reply->bytesAvailable() ) this->showErrorAndFinish();

    this->leaveManager();

    QString replyString( QString::fromLatin1( reply->readAll() ) );

    if( replyString == "juego_no_activo" ) this->showNoActiveAndFinish();

    QStringList results( replyString.split( "," ) );

    if( results.size() != 3 ||
        results.contains( "Database Error", Qt::CaseInsensitive ) )
    {
        qDebug() << "Error, algun error al obtener los datos del servidor";
        return;
    }

    emit readyStats( results.at( 0 ).toInt(),
                     results.at( 1 ).toInt(),
                     results.at( 2 ).toInt() );
}

void DataFacebook::responseDetection( QNetworkReply *reply )
{
    if( !reply->bytesAvailable() ) this->showErrorAndFinish();

    this->leaveManager();

    QString replyString( QString::fromLatin1( reply->readAll() ) );

    if( replyString == "juego_no_activo" ) this->showNoActiveAndFinish();

    QStringList results( replyString.split( "," ) );

    if( results.size() != 4 ||
        results.contains( "Database Error", Qt::CaseInsensitive ) )
    {
        qDebug() << "Error, algun error al guardar u obtener los datos del servidor";
        return;
    }

    emit readyDetection( results.at( 0 ) == "s" ?  true : false,
                         results.at( 1 ).toInt(),
                         results.at( 2 ).toInt(),
                         results.at( 3 ).toInt() );
}

void DataFacebook::responseAnswer( QNetworkReply *reply )
{
    if( !reply->bytesAvailable() ) this->showErrorAndFinish();

    this->leaveManager();

    QString replyString( QString::fromLatin1( reply->readAll() ) );

    if( replyString == "juego_no_activo" ) this->showNoActiveAndFinish();

    QStringList results( replyString.split( "," ) );

    qDebug() << results;

    if( results.size() != 4 ||
        results.contains( "Database Error", Qt::CaseInsensitive ) )
    {
        qDebug() << "Error, algun error al guardar u obtener los datos del servidor";
        return;
    }

    emit readyAnswer( results.at( 0 ) == "s" ?  true : false,
                      results.at( 1 ).toInt(),
                      results.at( 2 ).toInt(),
                      results.at( 3 ).toInt() );
}
