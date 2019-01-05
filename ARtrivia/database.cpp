#include "database.hpp"

#include <QSqlError>

Database *Database::instance = NULL;

Database::Database( QObject *parent ) :
    QObject( parent )
{
    database = QSqlDatabase::addDatabase( "QSQLITE" );
}

Database *Database::getInstance()
{
    if( !instance )
    {
        instance = new Database();
    }
    return instance;
}

Database::~Database()
{
    delete instance;
}

bool Database::connectDatabase()
{
    database.setDatabaseName( "db.sqlite" );
    return database.open();
}

void Database::disconnectDatabase()
{
    database.close();
}

void Database::borrarTablas()  {
    if( this->connectDatabase() )  {
        QSqlQuery query( database );

        QString queryBorrarTablas;

        queryBorrarTablas = "drop table if exists inicio_sesion;";
        query.exec( queryBorrarTablas );

        qDebug() << query.lastError() << query.lastError();

        queryBorrarTablas = "drop table if exists facebook_data;";
        query.exec( queryBorrarTablas );

        qDebug() << query.lastError() << query.lastError();

    }

    this->disconnectDatabase();
}

int Database::checkLogin()
{

    if( this->connectDatabase() )
    {
        if( database.tables().contains( "inicio_sesion" ) && database.tables().contains( "facebook_data" ) )
        {
            qDebug() << "Las tablas inicio_sesion y facebook_data ya estan creadas";

            QSqlQuery query( database );
            query.exec( "SELECT * FROM inicio_sesion" );

            while (query.next())  {
                int cantidadColumnas = query.size();

                for ( int i=0 ; i<cantidadColumnas ; i++ )  {
                    qDebug() << query.value(i).toString();
                }
            }

            query.exec( "SELECT * FROM facebook_data" );
            while (query.next())  {
                int cantidadColumnas = query.size();

                for ( int i=0 ; i<cantidadColumnas ; i++ )  {
                    qDebug() << query.value(i).toString();
                }
            }

            this->disconnectDatabase();
            return 1;
        }
        else
        {
            QSqlQuery query( database );

            QString queryBorrarTablas;

            queryBorrarTablas = "drop table if exists inicio_sesion;";
            query.exec( queryBorrarTablas );

            qDebug() << query.lastError() << query.lastError();

            queryBorrarTablas = "drop table if exists facebook_data;";
            query.exec( queryBorrarTablas );

            QString queryString;

            queryString = "create table inicio_sesion                       "
                          "(                                                "
                          "    mail             varchar(100)    not null,   "
                          "    clave            varchar(100)    not null    "
                          ");";

            bool ok = query.exec( queryString );

            if ( ! ok )  {
                this->disconnectDatabase();
                return -1;
            }

            queryString = "create table facebook_data                 "
                          "(                                          "
                          "    access_token   varchar(100)    null,   "
                          "    id             varchar(100)    null,   "
                          "    name           varchar(100)    null,   "
                          "    long_code      varchar(100)    null,   "
                          "    picture        varchar(200)    null    "
                          ");";

            ok = query.exec( queryString );

            this->disconnectDatabase();

            return ok ? 0 : -1;
        }
    }
    else
    {
        qDebug() << "acaaaaa";
        return -1;
    }
}

bool Database::saveData( QString teamName, QString password )
{
    if( this->connectDatabase() )
    {
        QSqlQuery query( database );

        if( !query.exec( "delete from inicio_sesion where 1=1;" ) )
        {
            return false;
        }

        QString queryString( QString( "insert into inicio_sesion ( usuario, clave ) values ( '") +
                             QString( teamName ) +
                             QString( "', '" ) +
                             QString( password ) +
                             QString( "' );" ) );

        bool ok = query.exec( queryString );

        this->disconnectDatabase();

        return ok;
    }
    else
    {
        return false;
    }
}

bool Database::saveDataConMail( QString password, QString mail )
{
    if( this->connectDatabase() )
    {
        QSqlQuery query( database );

        if( !query.exec( "delete from inicio_sesion where 1=1;" ) )
        {
            return false;
        }

        QString queryString( QString( "insert into inicio_sesion ( clave, mail ) values ( '") +
                             QString( password ) +
                             QString( "', '" ) +
                             QString( mail ) +
                             QString( "' );" ) );

        bool ok = query.exec( queryString );

        this->disconnectDatabase();

        return ok;
    }
    else
    {
        return false;
    }
}

// insert into facebook_data ( access_token, id, name, long_code, picture )
bool Database::saveDataFacebook( QString access_token,
                                 QString idFacebook,
                                 QString name,
                                 QString picture,
                                 QString long_code )
{
    if( this->connectDatabase() )
    {
        QSqlQuery query( database );

        if( !query.exec( "delete from facebook_data where 1=1;" ) )
        {
            return false;
        }

        QString queryString( QString( "insert into facebook_data ( access_token, id, "
                                      "name, long_code, picture ) values ( '") +
                             QString( access_token ) +
                             QString( "', '" ) +
                             QString( idFacebook ) +
                             QString( "', '" ) +
                             QString( name ) +
                             QString( "', '" ) +
                             QString( long_code ) +
                             QString( "', '" ) +
                             QString( picture ) +
                             QString( "' );" ) );

        bool ok = query.exec( queryString );

        this->disconnectDatabase();

        return ok;
    }
    else
    {
        return false;
    }
}


QStringList Database::readData()
{
    QStringList results;

    if( this->connectDatabase() )
    {
        QSqlQuery query = database.exec( "select usuario, clave from inicio_sesion limit 1;" );

        if( query.next() )
        {
            results.append( query.value( 0 ).toString() );
            results.append( query.value( 1 ).toString() );
        }
        else
        {
            results.append( "" );
            results.append( "" );
        }

        this->disconnectDatabase();
    }
    else
    {
        results.append( "" );
        results.append( "" );
    }

    return results;
}

QStringList Database::readDataConMail()
{
    QStringList results;

    if( this->connectDatabase() )
    {
        QSqlQuery query = database.exec( "select clave, mail from inicio_sesion limit 1;" );

        if( query.next() )
        {
            results.append( query.value( 0 ).toString() );
            results.append( query.value( 1 ).toString() );
        }
        else
        {
            results.append( "" );
            results.append( "" );
        }

        this->disconnectDatabase();
    }
    else
    {
        results.append( "" );
        results.append( "" );
    }

    return results;
}

QStringList Database::readDataFacebook()
{
    QStringList results;

    if( this->connectDatabase() )
    {
        QSqlQuery query = database.exec( "select access_token, name, picture, long_code, id "
                                         "from facebook_data limit 1;" );

        if( query.next() )
        {
            results.append( query.value( 0 ).toString() );
            results.append( query.value( 1 ).toString() );
            results.append( query.value( 2 ).toString() );
            results.append( query.value( 3 ).toString() );
            results.append( query.value( 4 ).toString() );
        }
        else
        {
            results.append( "" );
            results.append( "" );
            results.append( "" );
            results.append( "" );
            results.append( "" );
        }

        this->disconnectDatabase();
    }
    else
    {
        results.append( "" );
        results.append( "" );
        results.append( "" );
        results.append( "" );
        results.append( "" );
    }

    return results;
}

/**
 * @brief Database::isLongCode Si esta el long_code es porque se pidio codo corto de autorizacion y quizas el
 * usuario fue a facebook.com/device pero despues cerro la App. En fin, si el long_code esta entonces vemos
 * si se puede autorizar, ya que este long_code dura unos pocos minutos activo
 * @return
 */
bool Database::isLongCode()
{
    if ( readDataFacebook().size() > 0 && ! readDataFacebook().at(3).isEmpty() )
        return true;
    return false;
}


bool Database::isFacebookLogin()  {
    if ( readDataFacebook().size() > 0 && ! readDataFacebook().at(0).isEmpty() )
        return true;
    return false;
}

bool Database::isMailLogin()  {
    if ( readDataConMail().size() > 0 && ! readDataConMail().at(0).isEmpty() && ! readDataConMail().at(1).isEmpty() )
        return true;
    return false;
}

