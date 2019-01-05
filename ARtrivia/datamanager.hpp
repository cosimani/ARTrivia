#ifndef DATAMANAGER_HPP
#define DATAMANAGER_HPP

#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QVector>
#include <QStringList>

#include "error.hpp"

class DataManager : public QObject {

    Q_OBJECT

private:
    static DataManager *instance;
    explicit DataManager( QObject *parent = NULL );

    QNetworkAccessManager *manager;
    bool availableManager;

    bool checkManager();
    bool doQuery( QVector< QStringList > params );
    bool doQuery_inv( QVector< QStringList > params );
    void showErrorAndFinish();
    void showNoActiveAndFinish();
    void showFinishedAndFinish();

    QString eliminaAcentos(QString s);
public:
    static DataManager *getInstance();
    ~DataManager();

    bool getAvailableManager() const;
    void setAvailableManager( bool value );
    bool applyForManager();
    void leaveManager();

    bool requestLogin( QString teamName,
                       QString password );

    bool requestLoginMail( QString password,
                           QString mail );

    bool requestLoginFacebook( QString id_facebook );

    bool requestInit( QString teamName,
                      QString password,
                      QString member1Name,
                      int member1Dni,
                      QString member2Name,
                      int member2Dni,
                      QString member3Name,
                      int member3Dni );

    bool requestInitMail( QString name,
                          QString password,
                          QString mail );

    bool requestInitFacebook( QString name,
                              QString password,
                              QString mail );


    bool requestZones();
    bool requestZones_inv();
    bool requestQuestion( int teamId  );
    bool requestQuestion_inv( int teamId  );
    bool requestStats( int teamId );
    bool requestDetection( int teamId, int markerId );
    bool requestDetection_inv( int usuarioId, int markerId );
    bool requestAnswer( int teamId, int categoryId, int questionId, int answerId );


private slots:
    void responseLogin( QNetworkReply *reply );
    void responseInit( QNetworkReply *reply );
    void responseInitMail( QNetworkReply *reply );
    void responseInitFacebook( QNetworkReply *reply );
    void responseZones( QNetworkReply *reply );
    void responseQuestion( QNetworkReply *reply );
    void responseStats( QNetworkReply *reply );
    void responseDetection( QNetworkReply *reply );
    void responseAnswer( QNetworkReply *reply );

signals:
    void readyLogin( bool ok );
    void availableName( bool available );
    void readyInit( int teamId,
                    QString teamName,
                    QString member1Name,
                    QString member2Name,
                    QString member3Name,
                    int updatedScore,
                    int ranking,
                    int numberOfTeams );

    void readyInitMail( int usuarioId,
                        QString mail,
                        int updatedScore,
                        int ranking,
                        int numberOfTeams );

    void readyInitFacebook( int usuarioId,
                            QString nombre_facebook,
                            int updatedScore,
                            int ranking,
                            int numberOfTeams );


    void readyZones( QVector< QStringList > zones );
    void readyQuestion( int categoryId, QString categoryName, int questionId, QString questionText, QVector< QStringList > answers );
    void readyStats( int updatedScore, int ranking, int numberOfTeams );
    void readyDetection( bool isNew, int updatedScore, int ranking, int numberOfTeams );
    void readyAnswer( bool correct, int updatedScore, int ranking, int numberOfTeams );

    void signal_mailRegistrado();
};


#endif // DATAMANAGER_HPP
