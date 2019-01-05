#ifndef GAME_HPP
#define GAME_HPP

#include <QWidget>
#include <QLabel>
#include <QList>

#include <QWidget>
#include <QDir>

#include <cstdlib>

#include "datamanager.hpp"
#include "question.hpp"
//#include "common.hpp"

namespace Ui
{
    class Game;
}

class Game : public QWidget
{
    Q_OBJECT

private:
    static Game *instance;
    explicit Game( QWidget *parent = NULL );
    Ui::Game *ui;

    int teamId;
    QVector< QStringList > zones;
    int currentZoneId;
    QTimer *placeTimer;

    void setNewZone();

public:
    static Game *getInstance();
    ~Game();

    int getTeamId() const;
    void setTeamId( int value );

    QVector< QStringList > getZones() const;
    void setZones( const QVector< QStringList > &value );

    int getCurrentZoneId() const;
    void setCurrentZoneId( int value );

private slots:

    void saveZones( QVector< QStringList > zones );

    void updateInfo( int teamId,
                     QString teamName,
                     QString member1Name,
                     QString member2Name,
                     QString member3Name,
                     int updatedScore,
                     int ranking,
                     int numberOfTeams );

    void updateInfoMail(int usuarioId,
                         QString mail,
                         int updatedScore,
                         int ranking,
                         int numberOfTeams );

    void updateInfoFacebook(int usuarioId,
                         QString nombre_facebook,
                         int updatedScore,
                         int ranking,
                         int numberOfTeams );


    void updateStats( int updatedScore, int ranking, int numberOfTeams );
    void registerDetection( int markerId );
    void registerDetectionFinished( bool isNew, int updatedScore, int ranking, int numberOfTeams );
    void showQuestion( int categoryId, QString categoryName, int questionId, QString questionText, QVector< QStringList > answers );
    void newGoal( int updatedScore, int ranking, int numberOfTeams );
    void activateMe();
    void showPlaceAgain();
};

#endif // GAME_HPP
