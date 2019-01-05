#include "datamanager.hpp"

#include <QRegularExpression>

DataManager *DataManager::instance = NULL;

DataManager::DataManager( QObject *parent ) :
    QObject( parent ),
    manager( new QNetworkAccessManager( this ) ),
    availableManager( true )
{

}

bool DataManager::doQuery( QVector< QStringList >params )
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

bool DataManager::doQuery_inv( QVector< QStringList >params )
{
    if( !this->applyForManager() ) return false;

    QString query( "http://www.vayra.com.ar/trivia_ubp/trivia_inv.php?" );
    query.append( "key=tr1v14ubp" );

    for( int i = 0; i < params.size(); i++ )
    {
        if( params.at( i ).size() < 2 )
        {
            continue;
        }
        query.append( "&" + params.at( i ).at( 0 ) + "=" + params.at( i ).at( 1 ) );
    }

    qDebug() << query;

    manager->get( QNetworkRequest( QUrl( query ) ) );
    return true;
}

void DataManager::showErrorAndFinish()
{
    Error::getInstance()->setDescription( "Verifica tu conexión a internet y reinicia la aplicación" );
}

void DataManager::showNoActiveAndFinish()
{
    Error::getInstance()->setDescription( "La competencia aun no está activa, consulte a los organizadores!" );
}

void DataManager::showFinishedAndFinish()
{
    Error::getInstance()->setDescription( "Felicitaciones! Completaste todas las preguntas, espera los resultados!" );
}

DataManager *DataManager::getInstance()
{
    if( !instance )
    {
        instance = new DataManager();
    }
    return instance;
}

DataManager::~DataManager()
{
    delete instance;
}

bool DataManager::getAvailableManager() const
{
    return availableManager;
}

void DataManager::setAvailableManager( bool value )
{
    availableManager = value;
}

bool DataManager::applyForManager()
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
void DataManager::leaveManager()
{
    disconnect( manager, SIGNAL( finished( QNetworkReply* ) ), NULL, NULL );
    this->setAvailableManager( true );
}

bool DataManager::requestLogin( QString teamName,
                                QString password )
{
    QVector< QStringList >params;
    params.append( QStringList() << "action" << "login" );
    params.append( QStringList() << "teamName" << teamName );
    params.append( QStringList() << "password" << password );

    if( !this->doQuery( params ) ) return false;
    connect( manager, SIGNAL( finished( QNetworkReply* ) ), SLOT( responseLogin( QNetworkReply* ) ) );
    return true;
}

bool DataManager::requestLoginMail( QString password,
                                    QString mail )
{
    QVector< QStringList >params;
    params.append( QStringList() << "action" << "loginMail" );
    params.append( QStringList() << "password" << password );
    params.append( QStringList() << "mail" << mail );

    connect( manager, SIGNAL( finished( QNetworkReply* ) ), SLOT( responseLogin( QNetworkReply* ) ) );
    if( !this->doQuery_inv( params ) ) return false;

    return true;
}

bool DataManager::requestLoginFacebook( QString id_facebook )
{
    QVector< QStringList >params;
    params.append( QStringList() << "action" << "loginFacebook" );
    params.append( QStringList() << "id_facebook" << id_facebook );

    connect( manager, SIGNAL( finished( QNetworkReply* ) ), SLOT( responseLogin( QNetworkReply* ) ) );
    if( !this->doQuery_inv( params ) ) return false;

    return true;
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
bool DataManager::requestInit( QString teamName,
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


bool DataManager::requestInitMail( QString name,
                                   QString password,
                                   QString mail )
{
    QVector< QStringList >params;
    params.append( QStringList() << "action" << "initMail" );
    params.append( QStringList() << "name" << eliminaAcentos(name) );
    params.append( QStringList() << "password" << password );
    params.append( QStringList() << "mail" << mail );

    // A esto lo cambie, se conectaba despues de doQueryMail. Lo cambie porque puede pasar que la consulta
    // del servidor llegue antes que se ejecuten las lines ultimas de doQueryMail. Aunque con este cambio
    // tambien puede pasar que si el doQueryMail devuelve false siempre se conecte y se conecte
    connect( manager, SIGNAL( finished( QNetworkReply* ) ), SLOT( responseInitMail( QNetworkReply* ) ) );

    if( !this->doQuery_inv( params ) ) return false;

    return true;
}

QString DataManager::eliminaAcentos( QString s )  {
    // Performance: Eliminate characters you do not wish to have.
    s.remove(QRegularExpression("[" + QRegularExpression::escape("'!*,?|¡¿") + "]"));
    qDebug().noquote() << "Before:\t" << s;

    // Performance: Check for characters
    if (s.contains(QRegularExpression("[" + QRegularExpression::escape("$/:ÀÁÄÙÛÜàáäçèéêëïñóöùûü") + "]")))
    {
        // Special Characters
        // Escape function is a safety measure in case you accidentally insert "^" in the square brackets.
        s.replace(QRegularExpression("[" + QRegularExpression::escape(":/") + "]"), "-");
        s.replace(QRegularExpression("[$]"), "s");

        // Upper Case
        s.replace(QRegularExpression("[ÁÀ]"),   "A");
        s.replace(QRegularExpression("[Ä]"),    "Ae");
        s.replace(QRegularExpression("[ÜÛÙ]"),  "U");

        // Lower Case
        s.replace(QRegularExpression("[áà]"),   "a");
        s.replace(QRegularExpression("[ä]"),    "ae");
        s.replace(QRegularExpression("[ç]"),    "c");
        s.replace(QRegularExpression("[ëêéè]"), "e");
        s.replace(QRegularExpression("[ï]"),    "i");
        s.replace(QRegularExpression("[ñ]"),    "n");
        s.replace(QRegularExpression("[óö]"),   "o");
        s.replace(QRegularExpression("[ûù]"),   "u");
        s.replace(QRegularExpression("[ü]"),    "ue");
    }

    return s;
}

bool DataManager::requestInitFacebook( QString access_token,
                                       QString id,
                                       QString nombre )
{
    QVector< QStringList >params;
    params.append( QStringList() << "action" << "initFacebook" );
    params.append( QStringList() << "access_token" << access_token );
    params.append( QStringList() << "id_facebook" << id );
    params.append( QStringList() << "nombre_facebook" << eliminaAcentos(nombre) );

    qDebug() << "nombreeeeeeeeeeeeeeeeeeeeeee" << eliminaAcentos(nombre);

    // A esto lo cambie, se conectaba despues de doQueryMail. Lo cambie porque puede pasar que la consulta
    // del servidor llegue antes que se ejecuten las lines ultimas de doQueryMail. Aunque con este cambio
    // tambien puede pasar que si el doQueryMail devuelve false siempre se conecte y se conecte
    connect( manager, SIGNAL( finished( QNetworkReply* ) ), SLOT( responseInitFacebook( QNetworkReply* ) ) );

    if( !this->doQuery_inv( params ) ) return false;

    return true;
}


bool DataManager::requestZones()
{
    QVector< QStringList >params;
    params.append( QStringList() << "action" << "zones" );

    if( !this->doQuery( params ) ) return false;
    connect( manager, SIGNAL( finished( QNetworkReply* ) ), SLOT( responseZones( QNetworkReply* ) ) );
    return true;
}

bool DataManager::requestZones_inv()
{
    QVector< QStringList >params;
    params.append( QStringList() << "action" << "zones" );

    if( !this->doQuery_inv( params ) ) return false;
    connect( manager, SIGNAL( finished( QNetworkReply* ) ), SLOT( responseZones( QNetworkReply* ) ) );
    return true;
}


bool DataManager::requestQuestion( int teamId )
{
    QVector< QStringList >params;
    params.append( QStringList() << "action" << "question" );
    params.append( QStringList() << "teamId" << QString::number( teamId ) );

    if( !this->doQuery( params ) ) return false;
    connect( manager, SIGNAL( finished( QNetworkReply* ) ), SLOT( responseQuestion( QNetworkReply* ) ) );
    return true;
}

bool DataManager::requestQuestion_inv( int teamId )
{
    QVector< QStringList >params;
    params.append( QStringList() << "action" << "question" );
    params.append( QStringList() << "usuarioId" << QString::number( teamId ) );

    if( !this->doQuery_inv( params ) ) return false;
    connect( manager, SIGNAL( finished( QNetworkReply* ) ), SLOT( responseQuestion( QNetworkReply* ) ) );
    return true;
}


bool DataManager::requestStats( int teamId )
{
    QVector< QStringList >params;
    params.append( QStringList() << "action" << "stats" );
    params.append( QStringList() << "teamId" << QString::number( teamId ) );

    if( !this->doQuery( params ) ) return false;
    connect( manager, SIGNAL( finished( QNetworkReply* ) ), SLOT( responseStats( QNetworkReply* ) ) );
    return true;
}

bool DataManager::requestDetection( int teamId, int markerId )
{
    QVector< QStringList >params;
    params.append( QStringList() << "action" << "detection" );
    params.append( QStringList() << "teamId" << QString::number( teamId ) );
    params.append( QStringList() << "markerId" << QString::number( markerId ) );

    if( !this->doQuery( params ) ) return false;
    connect( manager, SIGNAL( finished( QNetworkReply* ) ), SLOT( responseDetection( QNetworkReply* ) ) );
    return true;
}

bool DataManager::requestDetection_inv( int usuarioId, int markerId )
{
    QVector< QStringList >params;
    params.append( QStringList() << "action" << "detection" );
    params.append( QStringList() << "usuarioId" << QString::number( usuarioId ) );
    params.append( QStringList() << "markerId" << QString::number( markerId ) );

    if( !this->doQuery_inv( params ) ) return false;
    connect( manager, SIGNAL( finished( QNetworkReply* ) ), SLOT( responseDetection( QNetworkReply* ) ) );
    return true;
}

bool DataManager::requestAnswer( int teamId, int categoryId, int questionId, int answerId )
{

#ifdef INDIVIDUAL

    QVector< QStringList >params;
    params.append( QStringList() << "action" << "answer_inv" );
    params.append( QStringList() << "usuarioId" << QString::number( teamId ) );
    params.append( QStringList() << "categoryId" << QString::number( categoryId ) );
    params.append( QStringList() << "questionId" << QString::number( questionId ) );
    params.append( QStringList() << "answerId" << QString::number( answerId ) );

    if( !this->doQuery_inv( params ) ) return false;
    connect( manager, SIGNAL( finished( QNetworkReply* ) ), SLOT( responseAnswer( QNetworkReply* ) ) );

#else

    QVector< QStringList >params;
    params.append( QStringList() << "action" << "answer" );
    params.append( QStringList() << "teamId" << QString::number( teamId ) );
    params.append( QStringList() << "categoryId" << QString::number( categoryId ) );
    params.append( QStringList() << "questionId" << QString::number( questionId ) );
    params.append( QStringList() << "answerId" << QString::number( answerId ) );

    if( !this->doQuery( params ) ) return false;
    connect( manager, SIGNAL( finished( QNetworkReply* ) ), SLOT( responseAnswer( QNetworkReply* ) ) );

#endif

    return true;
}

void DataManager::responseLogin( QNetworkReply *reply )
{
    qDebug() << "entra a DataManager::responseLogin";

    if( !reply->bytesAvailable() )  {
        qDebug() << "void DataManager::responseLogin( QNetworkReply *reply ) showErrorAndFinish";
        this->showErrorAndFinish();
    }

    this->leaveManager();

    QString replyString( QString::fromLatin1( reply->readAll() ) );

    if( replyString == "juego_no_activo" ) this->showNoActiveAndFinish();

    if( replyString.isEmpty() )
    {
        qDebug() << "Error, algun error al guardar u obtener los datos del servidor";
        return;
    }

    if( replyString == "n" )
    {
        emit readyLogin( false );
    }
    else
    {
        QStringList results( replyString.split( "," ) );

#ifdef INDIVIDUAL

        if( results.size() != 5 ||
            results.contains( "Database Error", Qt::CaseInsensitive ) )
        {
            qDebug() << "Error, algun error al guardar u obtener los datos del servidor";
            return;
        }

        emit readyLogin( true );

        // Esta senal se emite cuando nos queremos logeues con mail o con facebook
        // Justo coincide que el segundo campor es del mail, o del nombre de facebook
        emit readyInitMail( results.at( 0 ).toInt(),
                            results.at( 1 ),
                            results.at( 2 ).toInt(),
                            results.at( 3 ).toInt(),
                            results.at( 4 ).toInt() );

#else

        if( results.size() != 8 ||
            results.contains( "Database Error", Qt::CaseInsensitive ) )
        {
            qDebug() << "Error, algun error al guardar u obtener los datos del servidor";
            return;
        }

        emit readyLogin( true );

        emit readyInit( results.at( 0 ).toInt(),
                        results.at( 1 ),
                        results.at( 2 ),
                        results.at( 3 ),
                        results.at( 4 ),
                        results.at( 5 ).toInt(),
                        results.at( 6 ).toInt(),
                        results.at( 7 ).toInt() );

#endif

    }
}

void DataManager::responseInit( QNetworkReply *reply )
{
    if( !reply->bytesAvailable() ) this->showErrorAndFinish();

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

/**
 * @brief DataManager::responseInitMail Procesa la info recibida al registro de un usuario con su mail
 * @param reply
 */
void DataManager::responseInitMail( QNetworkReply *reply )
{
    if( !reply->bytesAvailable() ) this->showErrorAndFinish();

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

        // Espera que la base responda con: echo $usuarioId.','.$usuarioName.','.$score.','.$ranking.','.$counter;
        if( results.size() != 5 ||
            results.contains( "Database Error", Qt::CaseInsensitive ) )
        {
            qDebug() << "Error, algun error al guardar u obtener los datos del servidor";
            return;
        }

        emit availableName( true );

        emit signal_mailRegistrado();

        emit readyInitMail( results.at( 0 ).toInt(),
                            results.at( 1 ),
                            results.at( 2 ).toInt(),
                            results.at( 3 ).toInt(),
                            results.at( 4 ).toInt() );
    }

}

void DataManager::responseInitFacebook( QNetworkReply *reply )
{
    if( !reply->bytesAvailable() ) this->showErrorAndFinish();

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

        // Espera que la base responda con: echo $usuarioId.','.$usuarioName.','.$score.','.$ranking.','.$counter;
        if( results.size() != 5 ||
            results.contains( "Database Error", Qt::CaseInsensitive ) )
        {
            qDebug() << "Error, algun error al guardar u obtener los datos del servidor";
            return;
        }

        emit availableName( true );
        emit readyInitFacebook( results.at( 0 ).toInt(),
                                results.at( 1 ),
                                results.at( 2 ).toInt(),
                                results.at( 3 ).toInt(),
                                results.at( 4 ).toInt() );
    }

}

void DataManager::responseZones( QNetworkReply *reply )
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

void DataManager::responseQuestion( QNetworkReply *reply )
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

void DataManager::responseStats( QNetworkReply *reply )
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

void DataManager::responseDetection( QNetworkReply *reply )
{
    if( !reply->bytesAvailable() )  {
        qDebug() << "this->showErrorAndFinish();";
        this->showErrorAndFinish();
    }

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

void DataManager::responseAnswer( QNetworkReply *reply )
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
