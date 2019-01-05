#ifndef DATAFACEBOOK_HPP
#define DATAFACEBOOK_HPP

#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QVector>
#include <QStringList>

#include <QTimer>

#include "error.hpp"

class DataFacebook : public QObject {

    Q_OBJECT

private:
    static DataFacebook *instance;
    explicit DataFacebook( QObject *parent = NULL );

    QNetworkAccessManager *manager;
    bool availableManager;

    QTimer * timer;

    // Esta variable cuanta las veces que esta app pregunta para ver si el usuario fue a facebook.com/device para
    // colocar el codigo. Como el intervalo que sugiera facebook es preguntar cada 5 segundos, y como el codigo
    // dura 420 segundo (7 inutos), entonces, preguntaremos una cantidad de 84 veces.
    int cantidadDeRevisionDeAutorizacion;

    QString longCode;

    bool checkManager();
    bool doQuery( QVector< QStringList > params );
    void showErrorAndFinish();
    void showNoActiveAndFinish();
    void showFinishedAndFinish();

public:
    static DataFacebook *getInstance();
    ~DataFacebook();

    bool getAvailableManager() const;
    void setAvailableManager( bool value );
    bool applyForManager();
    void leaveManager();

    bool requestLogin( QString access_token, QString url );

    bool requestInit( QString teamName,
                      QString password,
                      QString member1Name,
                      int member1Dni,
                      QString member2Name,
                      int member2Dni,
                      QString member3Name,
                      int member3Dni );

    bool requestZones();
    bool requestQuestion( int teamId  );
    bool requestStats( int teamId );
    bool requestDetection( int teamId, int markerId );
    bool requestAnswer( int teamId, int categoryId, int questionId, int answerId );


public slots:
    void slot_codesObtenidos( QString code, QString longCode );

private slots:


    void slot_cada5segundos();

    void responseLogin( QNetworkReply *reply );
    void responseAutorizacion( QNetworkReply *reply );
    void responsePerfil( QNetworkReply *reply );


    void responseInit( QNetworkReply *reply );
    void responseZones( QNetworkReply *reply );
    void responseQuestion( QNetworkReply *reply );
    void responseStats( QNetworkReply *reply );
    void responseDetection( QNetworkReply *reply );
    void responseAnswer( QNetworkReply *reply );

signals:
    void signal_codigoEnClipboard( QString );

    // Es el codigo para colocar en facebook.com/device y el codigo largo
    void signal_codesObtenidos( QString, QString );

    void signal_perfilObtenido();

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

    void readyZones( QVector< QStringList > zones );
    void readyQuestion( int categoryId, QString categoryName, int questionId, QString questionText, QVector< QStringList > answers );
    void readyStats( int updatedScore, int ranking, int numberOfTeams );
    void readyDetection( bool isNew, int updatedScore, int ranking, int numberOfTeams );
    void readyAnswer( bool correct, int updatedScore, int ranking, int numberOfTeams );
};


#endif // DATAFACEBOOK_HPP
