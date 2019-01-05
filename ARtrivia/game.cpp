#include "game.hpp"
#include "ui_game.h"

Game *Game::instance = NULL;

Game::Game( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::Game ),
    teamId( -1 ),
    placeTimer( new QTimer )
{
    ui->setupUi( this );

    srand( time(NULL) );

    QPalette palette = this->palette();
    palette.setColor( QPalette::WindowText, QColor( 255, 255, 255 ) );
//    palette.setColor( QPalette::Background, QColor( "#A90230" ) );
    palette.setColor( QPalette::Background, QColor( "#ACB353" ) );
    this->setPalette( palette );

//    this->setFont( QFont( "Tahoma", 18 ) );


#ifdef INDIVIDUAL

    DataManager::getInstance()->requestZones_inv();

    connect( DataManager::getInstance(),
             SIGNAL( readyZones( QVector< QStringList > ) ),
             SLOT( saveZones( QVector< QStringList > ) ) );

    connect( DataManager::getInstance(),
             SIGNAL( readyInitMail(int,QString,int,int,int) ),
             SLOT( updateInfoMail(int,QString,int,int,int) ) );

    connect( DataManager::getInstance(),
             SIGNAL( readyInitFacebook(int,QString,int,int,int) ),
             SLOT( updateInfoMail(int,QString,int,int,int) ) );


    connect( ui->scene, SIGNAL( newMarkerDetected( int ) ), SLOT( registerDetection( int ) ) );

    connect( DataManager::getInstance(),
             SIGNAL( readyDetection( bool, int, int, int ) ),
             SLOT( registerDetectionFinished( bool, int, int, int ) ) );

    connect( DataManager::getInstance(),
             SIGNAL( readyQuestion( int, QString, int, QString, QVector< QStringList > ) ),
             SLOT( showQuestion( int, QString, int, QString, QVector< QStringList > ) ) );

    connect( Question::getInstance(),
             SIGNAL( questionFinished( int,int,int ) ),
             SLOT( newGoal( int, int, int ) ) );

    connect( Question::getInstance(),
             SIGNAL( activateGame() ),
             SLOT( activateMe() ) );

    connect( placeTimer, SIGNAL( timeout() ), SLOT( showPlaceAgain() ) );

#else

    DataManager::getInstance()->requestZones();

    connect( DataManager::getInstance(),
             SIGNAL( readyZones( QVector< QStringList > ) ),
             SLOT( saveZones( QVector< QStringList > ) ) );

    connect( DataManager::getInstance(),
             SIGNAL( readyInit( int, QString, QString, QString, QString, int, int, int ) ),
             SLOT( updateInfo( int, QString, QString, QString, QString, int, int, int ) ) );

    connect( ui->scene, SIGNAL( newMarkerDetected( int ) ), SLOT( registerDetection( int ) ) );

    connect( DataManager::getInstance(),
             SIGNAL( readyDetection( bool, int, int, int ) ),
             SLOT( registerDetectionFinished( bool, int, int, int ) ) );

    connect( DataManager::getInstance(),
             SIGNAL( readyQuestion( int, QString, int, QString, QVector< QStringList > ) ),
             SLOT( showQuestion( int, QString, int, QString, QVector< QStringList > ) ) );

    connect( Question::getInstance(),
             SIGNAL( questionFinished( int,int,int ) ),
             SLOT( newGoal( int, int, int ) ) );

    connect( Question::getInstance(),
             SIGNAL( activateGame() ),
             SLOT( activateMe() ) );

    connect( placeTimer, SIGNAL( timeout() ), SLOT( showPlaceAgain() ) );

#endif

}

Game::~Game()
{
    delete ui;
    delete instance;
}

void Game::setNewZone()
{
    int newZone = rand() % zones.size();
    this->setCurrentZoneId( newZone );

    ui->messageLabel->setText( "Próximo destino: " + zones.at( newZone ).at( 1 ) ) ;
}

Game *Game::getInstance()
{
    if( !instance )
    {
        instance = new Game();
    }
    return instance;
}

int Game::getTeamId() const
{
    return teamId;
}

void Game::setTeamId( int value )
{
    teamId = value;
}

QVector< QStringList > Game::getZones() const
{
    return zones;
}

void Game::setZones( const QVector< QStringList > &value )
{
    zones = value;
}

int Game::getCurrentZoneId() const
{
    return currentZoneId;
}

void Game::setCurrentZoneId( int value )
{
    currentZoneId = value;
}

void Game::saveZones( QVector< QStringList > zones )
{
    this->setZones( zones );
    this->setNewZone();
    qDebug() << "Zonas actualizadas" << zones;
}

void Game::updateInfo( int teamId,
                       QString teamName,
                       QString member1Name,
                       QString member2Name,
                       QString member3Name,
                       int updatedScore,
                       int ranking,
                       int numberOfTeams )
{
    this->setTeamId( teamId );

    // Aca borre lo que hacian los parametros
    Q_UNUSED( member1Name );
    Q_UNUSED( member2Name );
    Q_UNUSED( member3Name );

    ui->teamLabel->setText( teamName.toUpper() );
    this->updateStats( updatedScore, ranking, numberOfTeams );
}

/**
 * @brief Game::updateInfoMail Actualiza el titulo dentro de Game.
 * @param usuarioId
 * @param mail
 * @param updatedScore
 * @param ranking
 * @param numberOfTeams
 */
void Game::updateInfoMail(int usuarioId,
                           QString mail,
                           int updatedScore,
                           int ranking,
                           int numberOfTeams )
{
    this->setTeamId( usuarioId );

    ui->teamLabel->setText( mail );
    this->updateStats( updatedScore, ranking, numberOfTeams );
}

void Game::updateInfoFacebook(int usuarioId,
                              QString nombre_facebook,
                              int updatedScore,
                              int ranking,
                              int numberOfTeams )
{
    this->setTeamId( usuarioId );

    ui->teamLabel->setText( nombre_facebook );
    this->updateStats( updatedScore, ranking, numberOfTeams );
}


void Game::updateStats( int updatedScore, int ranking, int numberOfTeams )
{
    ui->scoreLabel->setText( "Puntos: " + QString::number( updatedScore ) );
    ui->rankingLabel->setText( "Ranking " + QString::number( ranking ) + "/" + QString::number( numberOfTeams ) );
}

void Game::registerDetection( int markerId )
{
    qDebug() << "Se llamo a registerDetection " << markerId;

    if( this->getTeamId() == -1 )
    {
        // Datos no inicializados"
        return;
    }

    if ( markerId == -1 )  {
        qDebug() << "No se detecto ningun marcador." << zones.at( this->getCurrentZoneId() ).at( 1 );

        ui->messageLabel->setText( "Marcador no detectado en la foto. Intente de nuevo!!" ) ;
        placeTimer->start( 3000 );
        return;
    }

    int idZone = zones.at( this->getCurrentZoneId() ).at( 0 ).toInt();
    bool correctZone = false;

    if ( markerId >= 0 && markerId < 50 && idZone == 1 )   correctZone = true;
    if ( markerId >= 50 && markerId < 100 && idZone == 2 )   correctZone = true;
    if ( markerId >= 100 && markerId < 150 && idZone == 3 )   correctZone = true;
    if ( markerId >= 150 && markerId < 200 && idZone == 4 )   correctZone = true;
    if ( markerId >= 200 && markerId < 250 && idZone == 5 )   correctZone = true;
    if ( markerId >= 250 && markerId < 300 && idZone == 6 )   correctZone = true;
    if ( markerId >= 300 && markerId < 350 && idZone == 7 )   correctZone = true;
    if ( markerId >= 350 && markerId < 400 && idZone == 8 )   correctZone = true;
    if ( markerId >= 400 && markerId < 450 && idZone == 9 )   correctZone = true;
    if ( markerId >= 450 && markerId < 500 && idZone == 10 )  correctZone = true;
    if ( markerId >= 500 && markerId < 550 && idZone == 11 )  correctZone = true;
    if ( markerId >= 550 && markerId < 600 && idZone == 12 )  correctZone = true;
    if ( markerId >= 600 && markerId < 650 && idZone == 13 )  correctZone = true;
    if ( markerId >= 650 && markerId < 700 && idZone == 14 )  correctZone = true;
    if ( markerId >= 700 && markerId < 750 && idZone == 15 )  correctZone = true;

//    bool correctZone = ( markerId / 100 ) == zones.at( this->getCurrentZoneId() ).at( 0 ).toInt();

    /* Recordar: En la imagen de la camara si correctZone es false
     * Mostrar mensaje que diga: Zona incorrecta
     * Estas en zones.at( markerId / 100 ).at( 1 )
     * Pero debes ir a zones.at( this->getCurrentZoneId() ).at( 1 )
     * Cortar la funcion
     * Borrar el siguiente qDebug() */
    if( !correctZone )
    {
        qDebug() << "Zona incorrecta" << zones.at( this->getCurrentZoneId() ).at( 1 );
        ui->messageLabel->setText( "Zona incorrecta" ) ;
        placeTimer->start( 3000 );
        return;
    }

    ui->messageLabel->setText( "¡Código capturado!" ) ;

#ifdef INDIVIDUAL

    DataManager::getInstance()->requestDetection_inv( this->getTeamId(), markerId );
    qDebug() << "requestDetection_inv" << this->getTeamId() << markerId;

#else

    DataManager::getInstance()->requestDetection( this->getTeamId(), markerId );

#endif
}

void Game::registerDetectionFinished( bool isNew, int updatedScore, int ranking, int numberOfTeams )
{
    /* Recordar: En la imagen de la camara si isNew es true
     * Mostrar mensaje que diga: Nuevo Marcador! +200
     * Sino, no mostrar nada o mostrar Marcador Conocido
     * Borrar el siguiente qDebug() */
    if( isNew )
    {
        qDebug() << "¡NUEVO MARCADOR! Sumas 200 puntos.";
        ui->messageLabel->setText( "Sumás 200 puntos." ) ;
        placeTimer->start( 4000 );
    }

    this->updateStats( updatedScore, ranking, numberOfTeams );

#ifdef INDIVIDUAL

    // Ahora pedir la pregunta
    DataManager::getInstance()->requestQuestion_inv( this->getTeamId() );

#else

    // Ahora pedir la pregunta
    DataManager::getInstance()->requestQuestion( this->getTeamId() );

#endif
}

void Game::showQuestion( int categoryId, QString categoryName, int questionId, QString questionText, QVector< QStringList > answers )
{
    Question::getInstance()->initQuestion( questionText,
                                           answers,
                                           this->getTeamId(),
                                           categoryId,
                                           categoryName,
                                           questionId  );

    this->hide();
    Question::getInstance()->show();
}

void Game::newGoal( int updatedScore, int ranking, int numberOfTeams )
{
    this->updateStats( updatedScore, ranking, numberOfTeams );
    this->setNewZone();
}

void Game::activateMe()
{
    Question::getInstance()->hide();
    this->show();
}

void Game::showPlaceAgain()
{
    placeTimer->stop();
    ui->messageLabel->setText( "Próximo destino: " + zones.at( getCurrentZoneId() ).at( 1 ) ) ;
}

