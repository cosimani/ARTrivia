#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <QObject>
#include <QDebug>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class Database: public QObject
{
    Q_OBJECT

private:
    static Database *instance;
    explicit Database( QObject *parent = NULL );

    QSqlDatabase database;

    bool connectDatabase();
    void disconnectDatabase();

public:
    static Database *getInstance();
    ~Database();

    int checkLogin();

    bool saveData( QString teamName, QString password );
    bool saveDataFacebook( QString access_token,
                           QString idFacebook,
                           QString name,
                           QString picture,
                           QString long_code );

    QStringList readData();
    QStringList readDataFacebook();
    QStringList readDataConMail();

    bool saveDataConMail( QString password, QString mail );
    void borrarTablas();
    bool isLongCode();
    bool isFacebookLogin();
    bool isMailLogin();
};

#endif // DATABASE_HPP
